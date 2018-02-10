//
//  gamepads
//

#pragma once

#include <stdint.h>
#if defined(_MSC_VER)
#include <Windows.h>
#else
#include <pthread.h>
#endif

struct Mutex
{
#if defined(_MSC_VER)
    CRITICAL_SECTION critical_section;
#else
    pthread_mutex_t mutex;
#endif
};

struct Condition
{
#if defined(_MSC_VER)
    CONDITION_VARIABLE condition_variable;
#else
    pthread_cond_t condition;
#endif
};

int mutex_init(struct Mutex* mutex);
int mutex_destroy(struct Mutex* mutex);
int mutex_lock(struct Mutex* mutex);
int mutex_unlock(struct Mutex* mutex);
int condition_init(struct Condition* condition);
int condition_destroy(struct Condition* condition);
int condition_signal(struct Condition* condition);
int condition_broadcast(struct Condition* condition);
int condition_wait(struct Condition* condition, struct Mutex* mutex);
int condition_timedwait(struct Condition* condition, struct Mutex* mutex, uint64_t ns);
int interlocked_compare_and_swap(int32_t old_value, int32_t new_value, int32_t* value);
