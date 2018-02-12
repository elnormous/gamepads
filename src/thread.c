//
//  gamepads
//

#include <stdlib.h>
#include <string.h>
#include "thread.h"

#if defined(_MSC_VER)
static DWORD WINAPI thread_function(LPVOID parameter)
#else
static void* thread_function(void* parameter)
#endif
{
    Thread* thread = (Thread*)parameter;

#ifdef __APPLE__
    if (thread->name) pthread_setname_np(thread->name);
#endif

    thread->function(thread->argument);

#if defined(_MSC_VER)
    return 0;
#else
    return NULL;
#endif
}

int thread_init(Thread* thread, void(*function)(void*), void* argument, const char* name)
{
    thread->function = function;
    thread->argument = argument;
    thread->name = name;

#if defined(_MSC_VER)
    DWORD thread_id;
    thread->handle = CreateThread(NULL, 0, thread_function, thread, 0, &thread_id);
    if (thread->handle == NULL) return 0;

    if (name)
    {
        THREADNAME_INFO info;
        info.dwType = 0x1000;
        info.szName = name;
        info.dwThreadID = thread_id;
        info.dwFlags = 0;

        __try
        {
            RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
        }
    }

    return 1;
#else
    if (pthread_create(&thread->thread, NULL, thread_function, thread) != 0) return 0;

#ifndef __APPLE__
    if (name) pthread_setname_np(thread->thread, name);
#endif

    return 1;
#endif
}

int thread_destroy(Thread* thread)
{
#if defined(_MSC_VER)
    return CloseHandle(thread->handle);
#else
    return 1;
#endif
}

int thread_join(Thread* thread)
{
#if defined(_MSC_VER)
    return WaitForSingleObject(thread->handle, INFINITE) != WAIT_FAILED;
#else
    return pthread_join(thread->thread, NULL) == 0;
#endif
}

int mutex_init(Mutex* mutex)
{
#if defined(_MSC_VER)
    InitializeCriticalSection(&mutex->critical_section);
    return 1;
#else
    return pthread_mutex_init(&mutex->mutex, NULL) == 0;
#endif
}

int mutex_destroy(Mutex* mutex)
{
#if defined(_MSC_VER)
    DeleteCriticalSection(&mutex->critical_section);
    return 1;
#else
    return pthread_mutex_destroy(&mutex->mutex) == 0;
#endif
}

int mutex_lock(Mutex* mutex)
{
#if defined(_MSC_VER)
    EnterCriticalSection(&mutex->critical_section);
    return 1;
#else
    return pthread_mutex_lock(&mutex->mutex) == 0;
#endif
}

int mutex_try_lock(Mutex* mutex)
{
#if defined(_MSC_VER)
    return TryEnterCriticalSection(&mutex->critical_section) != 0;
#else
    return pthread_mutex_trylock(&mutex->mutex) == 0;
#endif
}

int mutex_unlock(Mutex* mutex)
{
#if defined(_MSC_VER)
    LeaveCriticalSection(&mutex->critical_section);
    return 1;
#else
    return pthread_mutex_unlock(&mutex->mutex) == 0;
#endif
}

int condition_init(Condition* condition)
{
#if defined(_MSC_VER)
    InitializeConditionVariable(&condition->condition_variable);
    return 1;
#else
    return pthread_cond_init(&condition->condition, NULL) == 0;
#endif
}

int condition_destroy(Condition* condition)
{
#if defined(_MSC_VER)
    return 1;
#else
    return pthread_cond_destroy(&condition->condition) == 0;
#endif
}

int condition_signal(Condition* condition)
{
#if defined(_MSC_VER)
    WakeConditionVariable(&condition->condition_variable);
    return 1;
#else
    return pthread_cond_signal(&condition->condition) == 0;
#endif
}

int condition_broadcast(Condition* condition)
{
#if defined(_MSC_VER)
    WakeAllConditionVariable(&condition->condition_variable);
    return 1;
#else
    return pthread_cond_broadcast(&condition->condition) == 0;
#endif
}

int condition_wait(Condition* condition, Mutex* mutex)
{
#if defined(_MSC_VER)
    return SleepConditionVariableCS(&condition->condition_variable, &mutex->critical_section, INFINITE);
#else
    return pthread_cond_wait(&condition->condition, &mutex->mutex) == 0;
#endif
}

int condition_timedwait(Condition* condition, Mutex* mutex, uint64_t ns)
{
#if defined(_MSC_VER)
    return SleepConditionVariableCS(&condition->condition_variable, &mutex->critical_section, (DWORD)(ns / 1000000));
#else
    static const uint64_t NSEC_PER_SEC = 1000000000L;
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += ns;

    ts.tv_sec += (int32_t)(ts.tv_nsec / NSEC_PER_SEC);
    ts.tv_nsec %= NSEC_PER_SEC;

    return pthread_cond_timedwait(&condition->condition, &mutex->mutex, &ts) == 0;
#endif
}

int interlocked_compare_and_swap(int32_t old_value, int32_t new_value, int32_t* value)
{
#if defined(_MSC_VER)
    return InterlockedCompareExchange(value, new_value, old_value);
#else
    return __sync_val_compare_and_swap(value, old_value, new_value);
#endif
}
