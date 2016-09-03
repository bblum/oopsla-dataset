#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

static struct semaphore sema;

static void priority_sema_thread (void *aux UNUSED) 
{
  sema_down (&sema);
}

void test_priority_sema (void) 
{
  int i;

  sema_init (&sema, 0); 
  thread_set_priority (PRI_DEFAULT);
  for (i = 0; i < 2; i++)
    {   
      int priority = PRI_DEFAULT;
      char name[16];
      snprintf (name, sizeof name, "priority %d", priority);
      thread_create (name, priority, priority_sema_thread, NULL);
    }   

  for (i = 0; i < 2; i++)
    {   
      sema_up (&sema);
    }   
}
