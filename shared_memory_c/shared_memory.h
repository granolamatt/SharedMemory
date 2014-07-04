#ifndef SHARED_MEM_CLASS_H_
#define SHARED_MEM_CLASS_H_

#include <semaphore.h>

class SharedMemory {
    long size;
    std::string designator;
    std::string sync_designator;
    sem_t* waitSem;
    sem_t* syncSem;
    void* memory;
//    char* get_char_string(string);
    char* map_existing_memory(std::string);
  public:
    SharedMemory (std::string,long);
//    SharedMemory(std::string desig) : SharedMemory(desig,0){}
    long getSize() {return size;}
    void* get_memory();
    int close_shared();
    int unmap_memory();
    int close_semephores();
    int shared_synchronized();
    int shared_unlock();
    int shared_wait();
    int shared_notifyAll();
    int shared_notify();
};

#endif /* SHARED_MEM_CLASS_H_ */

