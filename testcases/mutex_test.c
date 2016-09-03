#include <syscall.h>
#include <stdlib.h>
#include <thread.h>
#include <mutex.h>
#include <cond.h>
#include <assert.h>
#include "410_tests.h"
#include <report.h>
#include <test.h>

#define STACK_SIZE 4096
#define ERR REPORT_FAILOUT_ON_ERR

mutex_t lock;
static int num_in_section = 0;

void critical_section()
{
        mutex_lock(&lock);
        num_in_section++;
        yield(-1);
        assert(num_in_section == 1);
        num_in_section--;
        mutex_unlock(&lock);
}

void *contender(void *dummy)
{
        ERR(swexn(NULL, NULL, NULL, NULL));
        critical_section();
        // Do it again, in case lock is correct but unlock corrupts something.
        critical_section();
        vanish();
        return NULL;
}

int main(void)
{
        ERR(thr_init(STACK_SIZE));
        ERR(mutex_init(&lock));
        ERR(thr_create(contender, NULL));
        ERR(swexn(NULL, NULL, NULL, NULL));
        critical_section();
        vanish(); // bypass thr_exit
        return 0;
}
