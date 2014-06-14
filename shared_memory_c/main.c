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

#include "getopts.h"
#include "shared.h"

int shared_size = 0;
char* des_name = NULL;
int test_memory = 0;

struct options opts[] = {
    { 1, "size", "Amount of shared memory", "s", 1},
    { 2, "name", "Shared Name to attach to", "n", 1},
    { 3, "read", "Read the memory", "r", 0},
    { 0, NULL, NULL, NULL, 0}
};

void parse_args(int argc, char *argv[]) {
    int c;
    char *args;

    while ((c = getopts(argc, argv, opts, &args)) != 0) {
        switch (c) {
            case -1:
                fprintf(stderr, "Unable to allocate memory for getopts.\n");
                exit(-1);
                break;
                            case 1:
                shared_size = atoi(args);
                printf("Using test size %d \n", shared_size);
                break;
            case 2:
                des_name = strdup(args);
                printf("File %s\n", des_name);
                break;
            case 3:
                test_memory = 1;
                printf("Testing already created memory\n");
                break;
            default:
                fprintf(stderr, "Unknown command line argument %i\n", c);
        }
        free(args);
    }
    if (!des_name) {
        printf("Use -f and give me a file\n");
        exit(1);
    }
    if (!shared_size) {
        printf("Use -s and give me a size to test\n");
        exit(2);
    }

}

int main(int argc, char *argv[]) {
    int cnt, t = 0;
    char *shared_memory;
    sem_t* sem_id;

    parse_args(argc, argv);
    
    long sizer = get_memory_size(des_name);
    
    if (sizer < 0) {
    	perror("Something is wrong or memory does not exits\n");
    } else {
    	printf("Size of memory is %ld\n", sizer);
    }

    printf("open memory\n");
    shared_memory = map_memory(des_name, shared_size);
    printf("open sem\n");
    sem_id = open_sem(des_name);

    if (test_memory) {
        printf("Testing memory \n");
        t = 1;
        for (cnt = 0; cnt < shared_size; cnt++) {
            char mem = shared_memory[cnt];
            if (mem != (char) cnt) {
                t = 0;
            }
        }
        if (t) {
            printf("Memory is equal\n");
        } else {
            printf("Memory is not equal\n");
        }
        printf("notify\n");
//        notifyAll(sem_id);
        notify(sem_id);
    } else {
        
        printf("Writing memory \n");
        for (cnt = 0; cnt < shared_size; cnt++) {
            shared_memory[cnt] = (char) cnt;
        }
        posix_wait(sem_id);
/*        printf("unmap\n");
        unmapMemory(shared_memory, shared_size);
        printf("close\n");
        closeShared(des_name);
        printf("close sem\n");
        close_sem(des_name);
        exit(0);*/
    }
    if (des_name) {
        free(des_name);
    }
    return 0;
}