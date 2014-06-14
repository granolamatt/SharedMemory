/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sharedmemory;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 *
 * @author matt
 */
public class SharedMemory {

    private final String designator;
    private final String sync_designator;
    private final ByteBuffer waitSem;
    private final ByteBuffer syncSem;
    private final ByteBuffer memory;

    public SharedMemory(String designator, long size) {
        this.designator = designator;
        this.sync_designator = designator + "sync";
        if (size <= 0) {
            memory = map_existing_memory(designator);
        } else {
            memory = map_memory(designator, size);
        }
        memory.order(ByteOrder.LITTLE_ENDIAN);
        waitSem = open_sem(designator);
        syncSem = open_sem(sync_designator);
        int current = posix_notify(syncSem);
        System.out.println("Current is " + current);
        if (current == 0) {
            posix_unlock(syncSem);
        }
        current = posix_notify(syncSem);
        while (current > 1) {
            current = posix_notify(syncSem);
            posix_synchronized(syncSem);
            current = posix_notify(syncSem);
        }
        System.out.println("Current is now " + current);

    }

    private static final boolean noGCJ = true;

    static {
        if (noGCJ) {
            try {
                InputStream in = ClassLoader.getSystemResourceAsStream("sharedmemory/libsharedmemory.so");
                File f = File.createTempFile("libsharedmemory.", "so");
                OutputStream out = new FileOutputStream(f);

                byte[] buf = new byte[1024];
                int len;

                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                }
                in.close();
                out.close();

                System.load(f.getAbsolutePath());
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }
    }

    public ByteBuffer getMemory() {
        return memory;
    }

    public int closeShared() {
        return closeShared(designator);
    }

    public int unmapMemory() {
        if (memory != null) {
            return unmapMemory(memory);
        }
        return -1;
    }

    public int close_sem() {
        int ret = close_sem(designator);
        if (ret >= 0) {
            ret = close_sem(sync_designator);
        }
        return ret;
    }

    public int posix_synchronized() {
        if (syncSem != null) {
            return posix_synchronized(syncSem);
        }
        return -1;
    }

    public int posix_unlock() {
        if (syncSem != null) {
            return posix_unlock(syncSem);
        }
        return -1;
    }

    public int posix_wait() {
        if (waitSem != null) {
            return posix_wait(waitSem);
        }
        return -1;
    }

    public int posix_notifyAll() {
        if (waitSem != null) {
            return posix_notifyAll(waitSem);
        }
        return -1;
    }

    public int posix_notify() {
        if (waitSem != null) {
            return posix_notify(waitSem);
        }
        return -1;
    }

    public native long get_memory_size(String designator);

    private native ByteBuffer map_memory(String designator, long size);

    private native ByteBuffer map_existing_memory(String designator);

    private native int closeShared(String designator);

    private native int unmapMemory(ByteBuffer memory);

    final public native ByteBuffer open_sem(String designator);

    private native int close_sem(String designator);

    private native int posix_synchronized(ByteBuffer sem_id);

    private native int posix_unlock(ByteBuffer sem_id);

    private native int posix_wait(ByteBuffer sem_id);

    private native int posix_notifyAll(ByteBuffer sem_id);

    private native int posix_notify(ByteBuffer sem_id);

}
