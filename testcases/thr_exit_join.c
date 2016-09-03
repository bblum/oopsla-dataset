#include <stdio.h>
#include <thread.h>
#include <syscall.h> /* for PAGE_SIZE */

void *waiter(void *p) {
  int status;
  thr_join((int)p, (void **)&status);
  thr_exit((void *) 0); 
}

int main() {
  thr_init(16 * PAGE_SIZE);
  (void) thr_create(waiter, (void *) thr_getid());
  thr_exit((void *)'!');
}

