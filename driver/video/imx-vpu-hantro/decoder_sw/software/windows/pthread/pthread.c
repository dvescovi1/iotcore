/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include <dwl.h>
#include <pthread.h>
#include <winstd.h>
#include <semaphore.h>

#include <Windows.h>

volatile int g_hook = 0;
void debug_hook()
{
	g_hook = 1;
}

int pthread_mutex_init(pthread_mutex_t *mutex,
	const pthread_mutexattr_t *attr)
{
	if (*mutex != 0)
	{
		debug_hook();
	}

	*mutex = CreateMutex(NULL, FALSE, NULL);
	if (*mutex == 0)
	{
		debug_hook();
		return GetLastError();
	}
	return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	if (*mutex != 0)
	{
		CloseHandle(*mutex);
		*mutex = 0;
	}
	return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	HANDLE lock = *mutex;
	DWORD waitResult;

	if (lock == 0)
	{
		HANDLE newMutex = CreateMutex(NULL, FALSE, NULL);
		if (newMutex == 0)
		{
			debug_hook();
			return GetLastError();
		}
		lock = InterlockedCompareExchangePointer(mutex, newMutex, 0);
		if (lock != 0)
		{
			CloseHandle(newMutex);
		}
		else
		{
			lock = newMutex;
		}
	}

	waitResult = WaitForSingleObject(lock, INFINITE);
	if (waitResult == WAIT_FAILED)
	{
		debug_hook();
		return GetLastError();
	}
	else if (waitResult != WAIT_OBJECT_0)
	{
		debug_hook();
	}
	return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	HANDLE lock = *mutex;

	if (lock == 0)
	{
		debug_hook();
	}
	if (!ReleaseMutex(lock))
	{
		debug_hook();
		return GetLastError();
	}
	return 0;
}

// TODO: this is a very ill-advised implementation of condition variables.
// windows supports them, but uses CriticalSections and not the Mutexes we use above.
int pthread_cond_init(pthread_cond_t *cond,
	const pthread_condattr_t *attr)
{
	sem_init(cond, 0, 0);

	return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
	sem_destroy(cond);

	return 0;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
	if (*cond == 0)
	{
		debug_hook();
		return 0;
	}

	// TODO: this should check to see if a thread is waiting on the condition variable first
	// but we don't have that functionality.  Act dumb and carry on...
	int value = 0;
	sem_getvalue(cond, &value);
	if (value == 0)
	{
		sem_post(cond);
	}
	else
	{
//		debug_hook();
	}

	return 0;
}

int pthread_cond_wait(pthread_cond_t *cond,
	pthread_mutex_t *mutex)
{
	if (*cond == 0 || *mutex == 0)
	{
		debug_hook();
		return 0;
	}

	pthread_mutex_unlock(mutex);
	sem_wait(cond);
	pthread_mutex_lock(mutex);

	return 0;
}

int sched_yield()
{
	return 0;
}

int getpagesize(void)
{
	return 4096;
}

int usleep(unsigned long us)
{
	Sleep(us / 1000);
	return 0;
}


int sem_getvalue(sem_t *sem, int * value) {
	DWORD result;
	
	if (*sem == 0)
	{
		debug_hook();
	}

	result = WaitForSingleObject(*sem, 0);

	if (result == WAIT_TIMEOUT)
	{
		*value = 0;
		return 0;
	}
	else if (result == WAIT_OBJECT_0)
	{
		long count = 0;
		if (!ReleaseSemaphore(*sem, 1, &count))
		{
			debug_hook();
			return GetLastError();
		}
		*value = (int)(count + 1);
	}
	else
	{
		debug_hook();
		*value = 0;
	}

	return 0;
}

int sem_init(sem_t *sem, int pshared, unsigned int value) {
	if (*sem != 0)
	{
		debug_hook();
	}

	*sem = CreateSemaphore(NULL, (LONG)value, MAXLONG, NULL);
	if (*sem == 0)
	{
		debug_hook();
		return GetLastError();
	}
	return 0;
}

int sem_post(sem_t *sem) {
	if (*sem == 0)
	{
		debug_hook();
	}

	if (!ReleaseSemaphore(*sem, 1, NULL))
	{
		debug_hook();
		return GetLastError();
	}
	return 0;
}

int sem_wait(sem_t *sem) {
	DWORD waitResult;

	if (*sem == 0)
	{
		debug_hook();
	}

	waitResult = WaitForSingleObject(*sem, INFINITE);
	if (waitResult == WAIT_FAILED)
	{
		debug_hook();
		return GetLastError();
	}
	else if (waitResult != WAIT_OBJECT_0)
	{
		debug_hook();
	}

	return 0;
}

int sem_destroy(sem_t *sem) {
	if (*sem == 0)
	{
		debug_hook();
	}

	CloseHandle(*sem);
	*sem = 0;
	return 0;
}
