#pragma once

// typedef struct { int a; } sem_t;

typedef void * sem_t;

extern int sem_getvalue(sem_t *sem, int *value);

extern int sem_init(sem_t *sem, int pshared, unsigned int value);
extern int sem_post(sem_t *sem);
extern int sem_wait(sem_t *sem);
extern int sem_destroy(sem_t *sem);

