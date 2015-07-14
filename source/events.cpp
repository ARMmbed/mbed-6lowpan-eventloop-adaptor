/*
 * Copyright (c) 2014-2015 ARM. All rights reserved.
 */
#include "mbed.h"

extern "C" {

#include "nanostack-event-loop/eventOS_scheduler.h"
#include "nanostack-event-loop/eventOS_scheduler.h"
#include "nanostack-event-loop/eventOS_event_timer.h"

#include "platform/arm_hal_interrupt.h"

} // extern "C"

/** Global Variables which indicate when interrupt are disabled */
volatile uint8_t sys_irq_disable_counter = 0;
volatile uint8_t sys_wait_signal = 0;

void platform_enter_critical(void)
{
     if (sys_irq_disable_counter++ == 0) {
         __disable_irq();
     }
}

void platform_exit_critical(void)
{
     if (--sys_irq_disable_counter == 0) {
         __enable_irq();
     }
}


/// - nanostack eventOS platform implementation:

/**
 * \brief Event schdeuler loop idle Callback which need to be port Every Application which use nanostack event scheduler
 */
void eventOS_scheduler_idle(void)
{
		eventOS_scheduler_wait();
}

/**
 * \brief This function will be called when stack enter idle state and start waiting signal.
 */
void eventOS_scheduler_wait(void)
{
     platform_enter_critical();
     sys_wait_signal = 1;
     platform_exit_critical();
     while (sys_wait_signal) {
        // !!! TODO mbed sleep?
        //sleep();
        //__WFI();
     }
}

/**
 * \brief This function will be called when stack receive event and could wake from idle.
 */
void eventOS_scheduler_signal(void)
{
    sys_wait_signal = 0;
}

/**
 * \brief This function will be called when stack can enter deep sleep state in detected time.
 *
 * \param sleep_time_ms Time in milliseconds to sleep
 *
 * \return sleeped time in milliseconds
 */
uint32_t eventOS_scheduler_sleep(uint32_t sleep_time_ms)
{
    (void) sleep_time_ms;
    // !!! TODO
    return 1;
}

