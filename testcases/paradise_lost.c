#include <syscall.h>
#include <stdlib.h>
#include <thread.h>
#include <mutex.h>
#include <cond.h>
#include <sem.h>
#include <assert.h>
#include "410_tests.h"
#include <report.h>
#include <test.h>

#define STACK_SIZE 4096
#define ERR REPORT_FAILOUT_ON_ERR

mutex_t little_lock; /* used to avoid spurious data race reports */
sem_t lock;
int num_in_section = 0;

// note: the mutex accesses in this function are ignored as PPs
// by use of without_function; they are here to prevent
// num_in_section acesses from being identified as data races.
void __attribute__((noinline)) critical_section() {
        mutex_lock(&little_lock);
        num_in_section++;
        mutex_unlock(&little_lock);
        yield(-1);
        mutex_lock(&little_lock);
        assert(num_in_section == 1);
        num_in_section--;
        mutex_unlock(&little_lock);
}
void *consumer(void *dummy) {
        ERR(swexn(NULL, NULL, NULL, NULL));
        sem_wait(&lock);
        critical_section();
        sem_signal(&lock);
        vanish();
        return NULL;
}
void producer() {
        ERR(mutex_init(&little_lock));
        ERR(sem_init(&lock, 0));
        ERR(thr_create(consumer, NULL));
        ERR(thr_create(consumer, NULL));
        sem_signal(&lock);
}
int main(void) {
        ERR(thr_init(STACK_SIZE));
        ERR(swexn(NULL, NULL, NULL, NULL));
        producer();
        vanish(); // bypass thr_exit
        return 0;
}
