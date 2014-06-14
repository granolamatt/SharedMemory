#ifndef SHARED_MEM_FUNC_H_
#define SHARED_MEM_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <semaphore.h>

long get_memory_size(const char *designator);
char *map_memory(const char *designator, long size);
int closeShared(const char *designator);
int unmapMemory(char* memory_address, int size);
sem_t* open_sem(const char *designator);
int close_sem(const char *designator);
int posix_synchronized(sem_t *sem_id);
int unlock(sem_t *sem_id);
int posix_wait(sem_t *sem_id);
int notifyAll(sem_t *sem_id);
int notify(sem_t *sem_id);

#ifdef __cplusplus
}
#endif

#endif /* SHARED_MEM_FUNC_H_ */

