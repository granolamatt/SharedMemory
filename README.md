SharedMemory
============

This is a project written for java and c that allows two processes to share information between them across posix memory.

There is a c folder that also includes the jni bindings for the Java.  The Java is a Netbeans project.

To run, cd into the c directory and run the compile script.  You will probably have to edit the script to point to where your java is loaded.
At some point I will make this easier, this is still very early code.

Very important, after the shared object is built you have to copy it back into the java project so it gets packaged into the jar.  I like to 
load my jars this way instead of pointed the library to some folder.  I always thought the jar concept was a hack, that is what works best for me.
