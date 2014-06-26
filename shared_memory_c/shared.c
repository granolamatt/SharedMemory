#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <semaphore.h>
#include "shared.h"

long get_memory_size(const char *designator) {
    int md;
    struct stat file_stat;
    

    md = shm_open(designator, O_CREAT | O_RDWR, 0600);
    if (md == -1) {
       printf("Got an error opening shm\n");
       return -1;
    }
    
    if (fstat(md, &file_stat)) {
    	perror("fstat error\n");
    	return -1;
    }
    
    return file_stat.st_size;
}


char *map_memory(const char *designator, long size) {
    int md;

    md = shm_open(designator, O_CREAT | O_RDWR, 0600 );
    if (md == -1) {
       printf("Got an error opening shm\n");
       exit(1);
    }
    int ferror = ftruncate(md, size);
    
    if (ferror == -1) {
    	printf("Error in ftruncate\n");
    }
    
    char* ret = (char *) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);

    return ret;
}

int closeShared(const char *designator) {
    return shm_unlink(designator);
}

int unmapMemory(char* memory_address, int size) {
    return munmap(memory_address, size);
}

sem_t* open_sem(const char *designator) {
    sem_t* sem_id;
    sem_id = sem_open(designator, O_CREAT, 0600, 0);

    if (sem_id == SEM_FAILED) {
        perror("failed on sem_open");
        return NULL;
    }
    return sem_id;
}

int close_sem(const char *designator) {
/*    sem_t* sem_id;
    sem_id = sem_open(designator, O_CREAT, 0600, 0);

    if (sem_id == SEM_FAILED) {
        perror("failed on sem_open");
        return 1;
    }*/
   return sem_unlink(designator);
    
}

int posix_synchronized(sem_t *sem_id) {

    if (sem_id) {
        if (sem_wait(sem_id) < 0) {
        	perror("sem_wait");
            return -2;
        }
    } else {
        return -1;
    }
    return 0;
}

int posix_wait(sem_t *sem_id) {

//    printf("waiting for child\n");

    if (sem_id) {
    	while(sem_trywait(sem_id) == 0){
    	}
        if (sem_wait(sem_id) < 0) {
        	perror("sem_wait");
            return -2;
        }
    }
    return 0;
}

int unlock(sem_t *sem_id) {
    if (sem_id) {
		if (sem_post(sem_id) < 0) {
            perror("sem_wait");
            return -1;
        } 
    }
    return 0;
}

int notifyAll(sem_t *sem_id) {
	int sem_value;
    if (sem_id) {
    	if(sem_getvalue(sem_id, &sem_value)) {
    		perror("sem_getvalue");
    		return -1;
    	}
		
//		printf("sem value is %d \n", sem_value);
		if (sem_value == 0) { 
		while (sem_value == 0) {
		if (sem_post(sem_id) < 0) {
            perror("sem_wait");
            return -1;
        }
        if(sem_getvalue(sem_id, &sem_value)) {
    		perror("sem_getvalue");
    	}
        } 
         } else {         
//        printf("Nobody waiting on me\n");
return sem_value;
        }
    }
    return 0;
    
}

int notify(sem_t *sem_id) {
	int sem_value;
    if (sem_id) {
    	if(sem_getvalue(sem_id, &sem_value)) {
    		perror("sem_getvalue");
    	}
		
//		printf("sem value is %d \n", sem_value);
		if (sem_value == 0) {
		if (sem_post(sem_id) < 0) {
            perror("sem_wait");
            return -1;
        } 
        }else {
//        printf("Nobody waiting on me\n");
        return sem_value;
        }
    }
    return 0;
}

