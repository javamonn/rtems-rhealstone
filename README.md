## Rhealstone Benchmark For RTEMS

This is an implementation of the Rhealstone benchmarks for RTEMS.

The Rhealstone metric is a set of benchmarks that aim to provide an independently
verifiable objective measure of real-time performance in different systems. The 
Rhealstone metric is composed of six independent benchmarks, each of which measure
an activity that is critical in real time systems:
+ [Task switching time](/task-switch/task-switch.c)
+ [Task preemption time](/task-preempt/task-preempt.c)
+ [Interrupt latency time](/interrupt-latency/interrupt-latency.c)
+ [Semaphore shuffling time](/semaphore-shuffle/semaphore-shuffle.c)
+ [Deadlock breaking time](/deadlock-break/deadlock-break.c)
+ [Intertask message latency](/message-latency/message-latency.c)

See the [build](/INSTALL.md) instructions for directions on building and running
the Rhealstone benchmarks.

The original proposal outlining the Rhealstone benchmark can be found [here](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/DDJ/1989/8902/8902a/8902a.htm).

Some other implementations can be seen on [FreeRTOS](http://timsengineeringblog.weebly.com/masters-thesis.html) and [iRMX](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/DDJ/1990/9004/9004d/9004d.htm).
