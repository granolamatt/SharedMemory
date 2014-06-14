#include <semaphore.h>
#include "sharedmemory_SharedMemory.h"
#include "shared.h"

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    get_memory_size
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_sharedmemory_SharedMemory_get_1memory_1size
  (JNIEnv *env, jobject obj, jstring descriptor) {
  	 long ret = -1;
    const char* desc = (*env)->GetStringUTFChars( env, descriptor , NULL ) ;
    ret = get_memory_size(desc);
    (*env)->ReleaseStringUTFChars(env, descriptor ,desc);
    return ret;
}

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    map_memory
 * Signature: (Ljava/lang/String;J)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_sharedmemory_SharedMemory_map_1memory
  (JNIEnv *env, jobject obj, jstring descriptor, jlong size) {
  	jobject ret = NULL;
  	char* memory;
  	
  	const char* desc = (*env)->GetStringUTFChars( env, descriptor , NULL ) ;
  	memory = map_memory(desc, size);
  	ret = (*env)->NewDirectByteBuffer(env, (void*) memory, size);
  	(*env)->ReleaseStringUTFChars(env, descriptor ,desc);
  	return ret;
}


/*
 * Class:     sharedmemory_SharedMemory
 * Method:    map_existing_memory
 * Signature: (Ljava/lang/String;)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_sharedmemory_SharedMemory_map_1existing_1memory
  (JNIEnv *env, jobject obj, jstring descriptor) {
  	jobject ret = NULL;
  	char* memory;
  	long size = 0;
  	
  	const char* desc = (*env)->GetStringUTFChars( env, descriptor , NULL ) ;
  	size = get_memory_size(desc);  	
  	memory = map_memory(desc, size);
  	ret = (*env)->NewDirectByteBuffer(env, (void*) memory, size);
  	(*env)->ReleaseStringUTFChars(env, descriptor ,desc);
  	return ret;
}

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    closeShared
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_sharedmemory_SharedMemory_closeShared
  (JNIEnv *env, jobject obj, jstring descriptor){
  int ret = -1;
  const char* desc = (*env)->GetStringUTFChars( env, descriptor , NULL ) ;
  ret = closeShared(desc);
  (*env)->ReleaseStringUTFChars(env, descriptor ,desc);
  return ret;
}
  

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    unmapMemory
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_sharedmemory_SharedMemory_unmapMemory
  (JNIEnv *env, jobject obj, jobject memory) {
  int ret = -1;
  char* memory_address = (char*) (*env)->GetDirectBufferAddress(env, memory);
  int size = (*env)->GetDirectBufferCapacity(env, memory);
  ret = unmapMemory(memory_address, size);
  return ret;
}

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    open_sem
 * Signature: (Ljava/lang/String;)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_sharedmemory_SharedMemory_open_1sem
  (JNIEnv *env, jobject obj, jstring descriptor) {
  jobject ret = NULL;
  const char* desc = (*env)->GetStringUTFChars( env, descriptor , NULL ) ;
  sem_t* sem = open_sem(desc);
  ret = (*env)->NewDirectByteBuffer(env, (void*) sem, sizeof(sem_t*));
  (*env)->ReleaseStringUTFChars(env, descriptor ,desc);
  return ret;
}

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    close_sem
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_sharedmemory_SharedMemory_close_1sem
  (JNIEnv *env, jobject obj, jstring descriptor) {
  jint ret = -1;
  const char* desc = (*env)->GetStringUTFChars( env, descriptor , NULL ) ;
  ret = close_sem(desc);
  (*env)->ReleaseStringUTFChars(env, descriptor ,desc);
  return ret;
}

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    posix_synchronized
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_sharedmemory_SharedMemory_posix_1synchronized
  (JNIEnv *env, jobject obj, jobject sem) {
  sem_t* sem_id = (sem_t*) (*env)->GetDirectBufferAddress(env, sem);
  return posix_synchronized(sem_id);  
}

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    posix_unlock
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_sharedmemory_SharedMemory_posix_1unlock
  (JNIEnv *env, jobject obj, jobject sem) {
  sem_t* sem_id = (sem_t*) (*env)->GetDirectBufferAddress(env, sem);
  return unlock(sem_id);  
}

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    posix_wait
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_sharedmemory_SharedMemory_posix_1wait
  (JNIEnv *env, jobject obj, jobject sem) {
  sem_t* sem_id = (sem_t*) (*env)->GetDirectBufferAddress(env, sem);
  return posix_wait(sem_id);  
}

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    posix_notifyAll
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_sharedmemory_SharedMemory_posix_1notifyAll
  (JNIEnv *env, jobject obj, jobject sem) {
  sem_t* sem_id = (sem_t*) (*env)->GetDirectBufferAddress(env, sem);
  return notifyAll(sem_id);  
}

/*
 * Class:     sharedmemory_SharedMemory
 * Method:    posix_notify
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_sharedmemory_SharedMemory_posix_1notify
  (JNIEnv *env, jobject obj, jobject sem) {
  sem_t* sem_id = (sem_t*) (*env)->GetDirectBufferAddress(env, sem);
  return notify(sem_id);  
}

