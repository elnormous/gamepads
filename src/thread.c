//
//  gamepads
//

#include "thread.h"

int thread_init(Thread* thread, void*(*function)(void*), void* argument)
{
#if defined(_MSC_VER)
    thread->handle = CreateThread(NULL, 0, function, argument, 0, NULL);
    return thread->handle != NULL;
#else
    return pthread_create(&thread->thread, NULL, function, argument) == 0;
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
