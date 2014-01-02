RTEMS Waf
~~~~~~~~~

You can find the Waf project here:

 http://code.google.com/p/waf/

Simple instructions on How to set up Waf is here:

 http://www.rtems.org/ftp/pub/rtems/people/chrisj/rtl/rtems-linker/waf.html

Steps
-----

1. Build or install the tools. In this example the path is
   /opt/rtems-4.11/bin/tools.

2. Build and install the RTEMS Board Support Packages you want to use. In this
   example the path is /opt/rtems-4.11/bin/build.

3. Unpack this package somewhere, anywhere on your disk and change into the top
   level directory.

4. Populate the git submodule:

    $ git submodule init
    $ git submodule update

5. Configure with your specific settings. In this case the path to the tools
   and RTEMS and provided on the command line and so do not need to be in your
   path or environment [1] and we limit the build to 'sparc/sis' BSP:

    $ waf configure --rtems=/opt/rtems-4.11/bin/build \
                    --rtems-tools=/opt/rtems-4.11/bin/tools \
                    --rtems-bsps=sparc/sis

   You can use '--rtems-archs=sparc,i386' or
   '--rtems-bsps=sparc/sis,i386/pc586' to build for more than BSP at a time.

6. Build:

   $ waf

7. Run the benchmarks using the simulator:

   $ /opt/rtems-4.11/bin/build/sparc-rtems4.11-run \ 
     ./build/sparc-rtems4.11-sis/task-switch/task-switch.exe

   $ /opt/rtems-4.11/bin/build/sparc-rtems4.11-run \
     ./build/sparc-rtems4.11-sis/task-preempt/task-preempt.exe

   $ /opt/rtems-4.11/bin/build/sparc-rtems4.11-run \
     ./build/sparc-rtems4.11-sis/interrupt-latency/interrupt-latency.exe

   $ /opt/rtems-4.11/bin/build/sparc-rtems4.11-run \
     ./build/sparc-rtems4.11-sis/semaphore-shuffle/semaphore-shuffle.exe

   $ /opt/rtems-4.11/bin/build/sparc-rtems4.11-run \
     ./build/sparc-rtems4.11-sis/deadlock-break/deadlock-break.exe

   $ /opt/rtems-4.11/bin/build/sparc-rtems4.11-run \
     ./build/sparc-rtems4.11-sis/message-latency/message-latency.exe

