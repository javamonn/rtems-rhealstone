#include <rtems.h>

#include <stdio.h>
#include "tmacros.h"
#include "timesys.h"

#define BENCHMARKS 50000   /* Number of benchmarks to run and average over */

rtems_task Task02( rtems_task_argument ignored );
rtems_task Task01( rtems_task_argument ignored );
rtems_task Init( rtems_task_argument ignored );

rtems_id           Task_id[2];
rtems_name         Task_name[2];

unsigned long      telapsed;          /* total time elapsed during benchmark */
unsigned long      tloop_overhead;    /* overhead of loops */
unsigned long      tresume_overhead;  /* overhead of rtems_task_resume  */
unsigned long      tsuspend_overhead; /* overhead of rtems_task_suspend */
unsigned long      tswitch_overhead;  /* overhead of time it takes to switch 
                                       * from TA02 to TA01, includes rtems_suspend
                                       * overhead
                                       */
unsigned long      count1, count2;
rtems_status_code  status;

rtems_task Task01( rtems_task_argument ignored )
{
  /* Start up TA02, get preempted */
  status = rtems_task_start( Task_id[1], Task02, 0);
  directive_failed( status, "rtems_task_start of TA02");

  tswitch_overhead = benchmark_timer_read();

  benchmark_timer_initialize();
  /* Benchmark code */
  for ( count1 = 0; count1 < BENCHMARKS; count1++ ) {
    rtems_task_resume( Task_id[1] );  /* Awaken TA02, preemption occurs */
  }

  /* Should never reach here */
  rtems_test_assert( false );
}

rtems_task Task02( rtems_task_argument ignored )
{
  /* Find overhead of task switch back to TA01 (not a preemption) */
  benchmark_timer_initialize();
  rtems_task_suspend( RTEMS_SELF );

  /* Benchmark code */
  for ( count2 = 0; count2 < BENCHMARKS - 1; count2++ ) {
    rtems_task_suspend( RTEMS_SELF );
  }

  telapsed = benchmark_timer_read();
  put_time(
     "Rhealstone: Preempt Task",
     telapsed,                            /* Total time of all benchmarks */
     count1,                              /* count1 benchmarks (only count1 preemptions) */
     tloop_overhead,                      /* Overhead of loops */
     tresume_overhead + tswitch_overhead  /* Overhead per benchmark */
  );

  rtems_test_exit( 0 );
}

rtems_task Init( rtems_task_argument ignored )
{
  Task_name[0] = rtems_build_name( 'T','A','0','1');
  status = rtems_task_create(
    Task_name[0],
    30,               /* TA01 is low priority task */
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[0]
  );
  directive_failed( status, "rtems_task_create of TA01");

  Task_name[1] = rtems_build_name( 'T','A','0','2');
  status = rtems_task_create(
    Task_name[1],
    28,               /* TA02 is high priority task */
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[1]
  );
  directive_failed( status, "rtems_task_create of TA02");

  /* Find loop overhead */
  benchmark_timer_initialize();
  for ( count1 = 0; count1 < BENCHMARKS; count1++ ); {
     /* rtems_task_resume( Task_id[1] ); */
  }
  for (count2 = 0; count2 < BENCHMARKS - 1; count2++ ); {
     /* rtems_task_suspend( RTEMS_SELF ); */
  }
  tloop_overhead = benchmark_timer_read();

  status = rtems_task_start( Task_id[0], Task01, 0);
  directive_failed( status, "rtems_task_start of TA01");

  /* Find overhead of resuming task */
  rtems_task_suspend( Task_id[0] );
  benchmark_timer_initialize();
  rtems_task_resume( Task_id[0] );
  tresume_overhead = benchmark_timer_read(); 
  
  status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of INIT");
}

/* configuration information */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 3
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
