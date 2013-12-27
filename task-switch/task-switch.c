#include <rtems.h>

#include <stdio.h>
#include "tmacros.h"
#include "timesys.h"

#define MAX_LOOPS 50000

rtems_task Tasks(rtems_task_argument ignored);
rtems_task Init(rtems_task_argument ignored);

rtems_id           Task_id[2];
rtems_name         Task_name[2];
unsigned long      overhead_time;
unsigned long      count;
rtems_status_code  status;

rtems_task Task02( rtems_task_argument ignored )
{
  float total_elapsed_time;

  /* All overhead accounted for now, we can begin benchmark */
  benchmark_timer_initialize();

  for ( count = 0; count < MAX_LOOPS - 1; count++ ) {
    rtems_task_wake_after( RTEMS_YIELD_PROCESSOR );
  }

  total_elapsed_time = benchmark_timer_read();
  put_time(
     "Rhealstone: Task switch",
     total_elapsed_time,
     (MAX_LOOPS * 2) - 1,               /* MAX_LOOP iterations in each task */
     overhead_time,                     /* Overhead time of loop */
     0                                  /* Rest of overhead has been accounted for */
  );

  rtems_test_exit( 0 );
}

rtems_task Task01( rtems_task_argument ignored )
{
  status = rtems_task_start( Task_id[1], Task02, 0 );
  directive_failed( status, "rtems_task_start of TA02");

  /* Yield processor so second task can startup and run */
  rtems_task_wake_after( RTEMS_YIELD_PROCESSOR );

  for ( count = 0; count < MAX_LOOPS; count++ ) {
    rtems_task_wake_after( RTEMS_YIELD_PROCESSOR );
  }

  /* Should never reach here */
  rtems_test_assert( false );
  
}

rtems_task Init( rtems_task_argument ignored )
{
  rtems_task_priority  pri;
  status = rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &pri);
  directive_failed( status, "rtems_task_set_priority");

  Task_name[0] = rtems_build_name( 'T','A','0','1');
  status = rtems_task_create(
    Task_name[0],
    (int) pri,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[0]
  );
  directive_failed( status, "rtems_task_create of TA01");

  Task_name[1] = rtems_build_name( 'T','A','0','2');
  status = rtems_task_create(
    Task_name[1],
    (int) pri,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[1]
  );
  directive_failed( status, "rtems_task_create of TA02");

  /* calculate overhead of routine (no task switches) */
  benchmark_timer_initialize();
  for ( count = 0; count < MAX_LOOPS * 2; count++ ) {
    /* rtems_task_wake_after( RTEMS_YIELD_PROCESSOR ) */
  }
  overhead_time = benchmark_timer_read();

  status = rtems_task_start( Task_id[0], Task01, 0);
  directive_failed( status, "rtems_task_start of TA01");

  status = rtems_task_delete( RTEMS_SELF);
  directive_failed( status, "rtems_task_delete of INIT");
}

/* configuration information */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 3
#define CONFIGURE_INIT
#include <rtems/confdefs.h>


