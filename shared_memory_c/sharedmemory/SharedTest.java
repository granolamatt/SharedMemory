/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sharedmemory;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.logging.Level;
import java.util.logging.Logger;
import sharedmemory.getoptions.OptionListing;

/**
 *
 * @author matt
 */
public class SharedTest {

    private int size = 0;
    private String descriptor = null;
    private boolean read = false;

    public SharedTest(String[] args) {
        configureArgs(args);
    }

    private void configureArgs(String[] args) {

        final OptionListing.RetClass[] argv = OptionListing.parseAllOptions(args);
        boolean close = false;

        for (OptionListing.RetClass arg : argv) {
            try {
                switch (arg.index) {
                    case 0:
                        String sizeS = arg.args[0];
                        size = Integer.decode(sizeS);
                        break;
                    case 1:
                        descriptor = arg.args[0];
                        break;
                    case 2:
                        read = true;
                        break;
                    case 3:
                        close = true;
                        break;
                }
            } catch (NumberFormatException ex) {
                System.out.println("Number was off");
            }

        }
        System.out.println("Using descriptor " + descriptor);
        if (descriptor == null) {
            System.out.println("Use -n and give a shared memory name");
            System.exit(-1);
        }
        if (close) {
            final SharedMemory mem = new SharedMemory(descriptor, size);
            mem.close_sem();
            mem.unmapMemory();
            mem.closeShared();
            System.exit(0);
        }

    }

    public void doTest() {
        final SharedMemory mem = new SharedMemory(descriptor, size);
        final ByteBuffer memory = mem.getMemory();
        if (read) {
            for (int cnt = 0; cnt < memory.capacity(); cnt++) {
                int val = (memory.get(cnt) & 0xff);
                if (val != (cnt % 256)) {
                    System.out.println("Not the same " + val);
                }
            }
            mem.posix_notify();
        } else {
            for (int cnt = 0; cnt < memory.capacity(); cnt++) {
                memory.put(cnt, (byte) cnt);
            }
            mem.posix_wait();
        }

        for (int cnt = 0; cnt < 100; cnt++) {
            final byte tnumber = (byte) cnt;
            Thread t = new Thread() {
                @Override
                public void run() {
                    while (!Thread.interrupted()) {
                        System.out.println("Synchronizing mem");
                        mem.posix_synchronized();
                        System.out.println("Last thread " + memory.get(0) + " I am " + tnumber);
                        memory.put(0, tnumber);
                        mem.posix_unlock();
                    }
                }
            };

            t.setName("Check " + cnt);
            t.setDaemon(true);
            t.start();
        }
        try {
            System.out.println("Press a key to exit");
            System.in.read();
        } catch (IOException ex) {
            Logger.getLogger(SharedTest.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("Starting test");
        SharedTest test = new SharedTest(args);
        test.doTest();
    }
}
