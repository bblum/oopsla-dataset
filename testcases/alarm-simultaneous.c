#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

struct semaphore done_sema;

// semaphore PPs here are ignored using without_function
static __attribute__((noinline)) void child_done() {
        sema_up(&done_sema);
}
// semaphore PPs here are ignored using without_function
static __attribute__((noinline)) void parent_done() {
        sema_down(&done_sema);
        sema_down(&done_sema);
}

static void sleeper (void *hax) {
        timer_sleep ((int)hax);
        child_done();
        thread_yield();
}
void test_alarm_simultaneous() {
        sema_init(&done_sema, 0); 
        thread_create ("Patrice Godefroid", PRI_DEFAULT, sleeper, (void *)3);
        thread_create ("Cormac Flanagan", PRI_DEFAULT, sleeper, (void *)4);
        thread_yield();
        timer_sleep (2);
        parent_done();
        thread_yield();
}

