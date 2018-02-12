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
    void(*function)(void*);
    void* argument;
    const char* name;
#if defined(_MSC_VER)
    HANDLE handle;
#else
    pthread_t thread;
#endif
} Thread;

typedef struct Mutex
{
#if defined(_MSC_VER)
    CRITICAL_SECTION criticalSection;
#else
    pthread_mutex_t mutex;
#endif
} Mutex;

typedef struct Condition
{
#if defined(_MSC_VER)
    CONDITION_VARIABLE conditionVariable;
#else
    pthread_cond_t condition;
#endif
} Condition;

int gpThreadInit(Thread* thread, void(*function)(void*), void* argument, const char* name);
int gpThreadDestroy(Thread* thread);
int gpThreadJoin(Thread* thread);
int gpMutexInit(Mutex* mutex);
int gpMutexDestroy(Mutex* mutex);
int gpMutexLock(Mutex* mutex);
int gpMutexTryLock(Mutex* mutex);
int gpMutexUnlock(Mutex* mutex);
int gpConditionInit(Condition* condition);
int gpConditionDestroy(Condition* condition);
int gpConditionSignal(Condition* condition);
int gpConditionBroadcast(Condition* condition);
int gpConditionWait(Condition* condition, Mutex* mutex);
int gpConditionTimedWait(Condition* condition, Mutex* mutex, uint64_t ns);
int gpInterlockedCompareAndSwap(int32_t oldValue, int32_t newValue, int32_t* value);
