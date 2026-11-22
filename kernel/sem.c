#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

#define NSEM 16

struct sem {
  struct spinlock lock;
  int used, id, value, refs, gen;
};

struct {
  struct spinlock lock;
  struct sem sem[NSEM];
} stable;

void
seminit(void)
{
  initlock(&stable.lock, "stable");
  for(int i = 0; i < NSEM; i++){
    initlock(&stable.sem[i].lock, "sem");
    stable.sem[i].used = 0;
    stable.sem[i].gen = 0;
  }
}

// El llamador debe tener stable.lock
static struct sem*
semfind(int id)
{
  for(struct sem *s = stable.sem; s < &stable.sem[NSEM]; s++)
    if(s->used && s->id == id)
      return s;
  return 0;
}

int
semopen(int id, int value)
{
  struct sem *s, *libre = 0;
  if(value < 0)
    return -1;
  acquire(&stable.lock);
  if((s = semfind(id)) != 0){
    s->refs++;
    release(&stable.lock);
    return 0;
  }
  for(s = stable.sem; s < &stable.sem[NSEM]; s++)
    if(!s->used){ libre = s; break; }
  if(libre == 0){
    release(&stable.lock);
    return -1;
  }
  libre->used = 1;
  libre->id = id;
  libre->value = value;
  libre->refs = 1;
  release(&stable.lock);
  return 0;
}

int
semwait(int id)
{
  struct sem *s;
  int gen;
  acquire(&stable.lock);
  if((s = semfind(id)) == 0){
    release(&stable.lock);
    return -1;
  }
  acquire(&s->lock);        // orden: stable.lock -> s->lock
  release(&stable.lock);
  gen = s->gen;
  for(;;){
    if(killed(myproc()) || s->gen != gen){
      release(&s->lock);
      return -1;
    }
    if(s->value > 0)
      break;
    sleep_prepare(s);       // registra el canal (con s->lock tomado)
    release(&s->lock);
    sleep();                // retorna sin dormir si ya hubo wakeup
    acquire(&s->lock);
  }
  s->value--;
  release(&s->lock);
  return 0;
}

int
sempost(int id)
{
  struct sem *s;
  acquire(&stable.lock);
  if((s = semfind(id)) == 0){
    release(&stable.lock);
    return -1;
  }
  acquire(&s->lock);
  release(&stable.lock);
  s->value++;
  wakeup(s);
  release(&s->lock);
  return 0;
}

int
semclose(int id)
{
  struct sem *s;
  acquire(&stable.lock);
  if((s = semfind(id)) == 0){
    release(&stable.lock);
    return -1;
  }
  if(--s->refs == 0){
    acquire(&s->lock);
    s->used = 0;
    s->gen++;               // invalida a los que esperan
    wakeup(s);
    release(&s->lock);
  }
  release(&stable.lock);
  return 0;
}
