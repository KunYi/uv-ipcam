#include "threads.h"

#ifdef WIN32

/******************************************************************************\
 *
 *	Emulation of POSIX condition variables for WIN32
 *
\******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

int emulate_pthread_mutex_lock(volatile MUTEX_TYPE *mx)
{
  if (*mx == NULL) /* static initializer? */
  {
    HANDLE p = CreateMutex(NULL, FALSE, NULL);

    if (InterlockedCompareExchangePointer((PVOID*)mx, (PVOID)p, NULL) != NULL)
      CloseHandle(p);
  }

  return WaitForSingleObject(*mx, INFINITE) == WAIT_FAILED;
}

int emulate_pthread_cond_init(COND_TYPE *cv)
{
  cv->waiters_count_ = 0;
  cv->signal_event_ = CreateEvent(NULL, FALSE, FALSE, NULL);
  InitializeCriticalSection(&cv->waiters_count_lock_);

  return 0;
}

int emulate_pthread_cond_destroy(COND_TYPE *cv)
{
  CloseHandle(cv->signal_event_);
  DeleteCriticalSection(&cv->waiters_count_lock_);

  return 0;
}

int emulate_pthread_cond_signal(COND_TYPE *cv)
{
  int have_waiters;

  EnterCriticalSection(&cv->waiters_count_lock_);
  have_waiters = cv->waiters_count_ > 0;
  LeaveCriticalSection(&cv->waiters_count_lock_);

  if (have_waiters)
    return SetEvent(cv->signal_event_) == 0;

  return 0;
}

int emulate_pthread_cond_wait(COND_TYPE *cv, MUTEX_TYPE *cs)
{
  int result;

  EnterCriticalSection(&cv->waiters_count_lock_);
  cv->waiters_count_++;
  LeaveCriticalSection(&cv->waiters_count_lock_);

  ReleaseMutex(*cs);

  result = (WaitForSingleObject(cv->signal_event_, INFINITE) == WAIT_FAILED);

  if (!result)
  {
    EnterCriticalSection(&cv->waiters_count_lock_);
    cv->waiters_count_--;
    LeaveCriticalSection(&cv->waiters_count_lock_);

    result = (WaitForSingleObject(*cs, INFINITE) == WAIT_FAILED);
  }

  return result;
}

#ifdef __cplusplus
}
#endif

#endif
