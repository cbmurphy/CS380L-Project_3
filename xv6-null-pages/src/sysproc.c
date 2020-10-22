#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_mprotect(void) {
  int addr;
  int len;

  // first make sure we can even read the arguments
  if (argint(0, &addr) < 0) {
    cprintf("mprotect: Could not get arg addr.\n");
    return -1;
  }
  if ((uint)argint(1, &len) < 0) {
    cprintf("mprotect: Could not get arg len.\n");
    return -1;
  }

  // then check that the arguments are valid
  if (addr % PGSIZE != 0) {
    cprintf("Bad arguments to mprotect: addr not aligned\n");
    return -1;
  }
  if (len <= 0) {
    cprintf("Bad arguments to mprotect: len %d <= 0.\n", len);
    return -1;
  }
  if ((uint)addr + (len * PGSIZE) > myproc()->sz || addr < 0) {
    cprintf("Bad arguments to munprotect: addr outside of process address space.\n", len);
    return -1;
  }

  cprintf("mprotect arguments valid.\n");
  return mprotect((void *)addr, len);
}

int
sys_munprotect(void) {
  int addr;
  int len;

  // first make sure we can even read the arguments
  if (argint(0, &addr) < 0) {
    cprintf("munprotect: Could not get arg addr.\n");
    return -1;
  }
  if ((uint)argint(1, &len) < 0) {
    cprintf("munprotect: Could not get arg len.\n");
    return -1;
  }

  // then check that the arguments are valid
  if (addr % PGSIZE != 0) {
    cprintf("Bad arguments to munprotect: addr not aligned\n");
    return -1;
  }
  if (len <= 0) {
    cprintf("Bad arguments to mprotect: len %d <= 0.\n", len);
    return -1;
  }
  if ((uint)addr + (len * PGSIZE) > myproc()->sz || addr < 0) {
    cprintf("Bad arguments to mprotect: addr outside of process address space.\n", len);
    return -1;
  }

  cprintf("munprotect arguments valid.\n");
  return munprotect((void *)addr, len);
}