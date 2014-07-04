// classes example
#include <iostream>
#include <string.h>
#include <semaphore.h>
#include "shared_memory.h"
#include "shared.h"

using namespace std;

SharedMemory::SharedMemory(string desig, long size) {
	designator = desig + '\0';
	sync_designator = desig + "sync" + '\0';
	if (size <= 0) {
      memory = map_existing_memory(designator);
      
   } else {
      memory = map_memory(designator.c_str(), size);
   }
   waitSem = open_sem(designator.c_str());
   syncSem = open_sem(sync_designator.c_str());

   if (size <= 0) {
      int current = notify(syncSem);
      cout<< "Current is " << current << endl;
      if (current == 0) {
          unlock(syncSem);
      }
      current = notify(syncSem);
      while (current > 1) {
         current = notify(syncSem);
         posix_synchronized(syncSem);
         current = notify(syncSem);
      }
      cout << "Current is now " << current << endl;
   }
}

char* SharedMemory::map_existing_memory(string desc) {
  	long size = 0;
  	
  	size = get_memory_size(desc.c_str());  	
  	return map_memory(desc.c_str(), size);
}

//char* SharedMemory::get_char_string(string my_string) {
//   std::string str;
//   // C++ string not guaranteed to be null terminated
//   string retstring = my_string + '\0';
////   std::vector<char> writable(str.begin(), str.end());
////   writable.push_back('\0');
//	// Do not forget to free this memory when done!!!!
//   return strdup(retstring.c_str());
//}

void* SharedMemory::get_memory() {
    return memory;
}

int SharedMemory::close_shared() {
    return closeShared(designator.c_str());
}

int SharedMemory::unmap_memory() {
    if (memory) {
        return unmapMemory((char*)memory, size);
    }
    return -1;
}

int SharedMemory::close_semephores() {
    int ret = close_sem(designator.c_str());
    if (ret) {
        ret = close_sem(sync_designator.c_str());
    }
    return ret;
}

int SharedMemory::shared_synchronized() {
    if (syncSem) {
        return posix_synchronized(syncSem);
    }
    return -1;
}

int SharedMemory::shared_unlock() {
    if (syncSem) {
        return unlock(syncSem);
    }
    return -1;
}

int SharedMemory::shared_wait() {
    if (waitSem) {
        return posix_wait(waitSem);
    }
    return -1;
}

int SharedMemory::shared_notifyAll() {
    if (waitSem) {
        return notifyAll(waitSem);
    }
    return -1;
}

int SharedMemory::shared_notify() {
    if (waitSem) {
        return notify(waitSem);
    }
    return -1;
}


int main () {
  SharedMemory* rect = new SharedMemory("test", 0);

  return 0;
}
