#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

/********************* v ADDED THIS v **************************/

#define PGSIZE 4096

int thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2)
{
  printf(1, "Create thread\n");
  // alloc two pages of memory for stack and use
  void *stack = malloc(PGSIZE * 2);
  if (stack <= 0) {
    printf(1, "Failed to malloc memory for stack in thread_create.\n");
    exit();
  }
  printf(1, "thread_create stack: %d\n", stack);
  // make sure stack is page aligned
  if((uint)stack % PGSIZE) {
    printf(1, "malloced stack is not aligned in thread_create. Shift to next page.\n");
    stack = stack + (PGSIZE - (uint)stack % PGSIZE);
    printf(1, "new stack: %d\n", stack);
  }
  printf(1, "thread_create: clone() on stack %d.\n", stack);
  return clone(start_routine, arg1, arg2, stack);
}

int thread_join()
{
  void *stack;
  printf(1, "thread_join\n");

  // get id of thread
  int pid = join(&stack);
  printf(1, "join thread %d\n", pid);

  // if not parent process, free the thread's stack
  if (pid != -1) {
    free(stack);
  }
  printf(1, "stack freed\n");

  return pid;
}

/** copied from http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf **/
void lock_init(lock_t *lock)
{
  lock->ticket = 0;
  lock->turn = 0;
}

void lock_acquire(lock_t *lock){
  int myturn = fetch_and_add(&lock->ticket, 1);
  while(lock->turn != myturn)
    ; // spin
}

void lock_release(lock_t *lock) {
  lock->turn = lock->turn + 1;
}

/*********************** ^ ADDED THIS ^ ***************************/
