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

typedef struct Thread
{
#if defined(_MSC_VER)
    HANDLE handle;
#else
    pthread_t thread;
#endif
} Thread;

typedef struct Mutex
{
#if defined(_MSC_VER)
    CRITICAL_SECTION critical_section;
#else
    pthread_mutex_t mutex;
#endif
} Mutex;

typedef struct Condition
{
#if defined(_MSC_VER)
    CONDITION_VARIABLE condition_variable;
#else
    pthread_cond_t condition;
#endif
} Condition;

int thread_init(Thread* thread, void*(*function)(void*), void* argument);
int thread_destroy(Thread* thread);
int thread_join(Thread* thread);
int mutex_init(Mutex* mutex);
int mutex_destroy(Mutex* mutex);
int mutex_lock(Mutex* mutex);
int mutex_unlock(Mutex* mutex);
int condition_init(Condition* condition);
int condition_destroy(Condition* condition);
int condition_signal(Condition* condition);
int condition_broadcast(Condition* condition);
int condition_wait(Condition* condition, Mutex* mutex);
int condition_timedwait(Condition* condition, Mutex* mutex, uint64_t ns);
int interlocked_compare_and_swap(int32_t old_value, int32_t new_value, int32_t* value);
