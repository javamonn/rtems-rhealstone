#include <rtems.h>

#include <stdio.h>
#include "tmacros.h"
#include "timesys.h"

/* configuration information */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 4
#define CONFIGURE_MAXIMUM_SEMAPHORES 2
#define CONFIGURE_INIT
#include <rtems/confdefs.h>

#define BENCHMARKS 50000

rtems_task Task01( rtems_task_argument ignored );
rtems_task Task02( rtems_task_argument ignored );
rtems_task Init( rtems_task_argument ignored );

rtems_id   Task_id[3];
rtems_name Task_name[3];
rtems_id    sem_id;
rtems_name  sem_name;

uint32_t    telapsed;
uint32_t    loop_overhead;
uint32_t    count;

rtems_task Init( rtems_task_argument ignored )
{
  rtems_status_code status;
  rtems_attribute sem_attr;

  sem_attr =  RTEMS_BINARY_SEMAPHORE | RTEMS_PRIORITY;

  sem_name = rtems_build_name( 'S','0',' ',' ');
  status = rtems_semaphore_create(
    sem_name,
    1,
    sem_attr,
    0,
    &sem_id
  );
  directive_failed( status, "rtems_semaphore_create of S0" );

  Task_name[0] = rtems_build_name( 'T','A','0','1' );
  status = rtems_task_create(
    Task_name[0],
    30,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[0]
  );
  directive_failed( status, "rtems_task_create of TA01" );

  Task_name[1] = rtems_build_name( 'T','A','0','2' );
  status = rtems_task_create(
    Task_name[1],
    28,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[1]
  );
  directive_failed( status , "rtems_task_create of TA02\n" );

  status = rtems_task_start( Task_id[0], Task01, 0 );
  directive_failed( status, "rtems_task_start of TA01" );

  status = rtems_task_delete( RTEMS_SELF );
  directive_failed( status, "rtems_task_delete of INIT");
}

rtems_task Task01( rtems_task_argument ignored )
{
  rtems_status_code status;

  rtems_semaphore_obtain( sem_id, RTEMS_WAIT, 0 );
  directive_failed( status, "rtems_semaphore_obtain of S0\n" );

  /* TA02 preempts, starts up */
  rtems_task_start( Task_id[1], Task02, 0 );

  /* Benchmark code */
  for ( ; count < BENCHMARKS ; ) {
    rtems_semaphore_release(sem_id);
  }

  /* Should never reach here */
  rtems_test_assert( false );
}

rtems_task Task02( rtems_task_argument ignored )
{

  /* Benchmark code */
  benchmark_timer_initialize();
  for ( count = 0; count < BENCHMARKS - 1; count++ ) {
    /* Block on semaphore, switch to TA01 */
    rtems_semaphore_obtain( sem_id, RTEMS_WAIT, 0 );
  }
  telapsed = benchmark_timer_read();

  put_time(
     "Rhealstone: Semaphore Shuffle",
     telapsed,
     BENCHMARKS,         /* BENCHMARKS times a "semaphore shuffle" occurs*/
     loop_overhead,      /* Overhead of loop */
     0                   /* No overhead, obtain/relase part of shuffle */
  );
}
