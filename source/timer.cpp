/*
 * Copyright (c) 2014 ARM. All rights reserved.
 */
#include "mbed.h"

extern "C" {
#include "nanostack-event-loop/platform/arm_hal_timer.h"
} // extern "C"

#include "minar/minar.h"
#include "minar_platform/minar_platform.h"

static void (*sn_callback)(void) = NULL;
static timestamp_t sn_compare_value;
static bool timer_enabled = false;
static minar::callback_handle_t cb_handle;

#define MAXIMUM_SLOTS 10000


/**
 * \brief This function perform timer init.
 */
void platform_timer_enable(void)
{
}

/**
 * \brief This function is API for set Timer interrupt handler for stack
 *
 * \param new_fp Function pointer for stack giving timer handler
 *
 */
void platform_timer_set_cb(platform_timer_cb new_fp)
{
    sn_callback = new_fp;
}

/**
 * \brief This function is API for stack timer start
 *
 * \param slots define how many 50us slot time period will be started
 *
 */
void platform_timer_start(uint16_t slots)
{
    //uint32_t minar_ticks = ((uint32_t)slots * 50 * minar::platform::Time_Base) / 1000000;
    uint32_t minar_ticks = ((uint32_t)slots * minar::platform::Time_Base) / 20000;

    minar::Scheduler *sched = minar::Scheduler::instance();
    sn_compare_value = minar::platform::getTime() + minar_ticks;
    timer_enabled = true;
    cb_handle = sched->postCallback(sn_callback).delay(minar_ticks).tolerance(0).getHandle();
}

/**
 * \brief This function is API for stack timer stop
 *
 */
void platform_timer_disable(void)
{
    minar::Scheduler::instance()->cancelCallback(cb_handle);
    timer_enabled = false;
}

/**
 * \brief This function is API for stack timer to read active timer remaining slot count
 *
 * \return 50us time slot remaining
 */
uint16_t platform_timer_get_remaining_slots(void)
{
    uint32_t counter = minar::platform::getTime(), slots = 0;

    // [TODO] figure out what the implementation doesn't work without the 'return' below
    return 0;
    if (sn_compare_value >= counter)
    {
        //slots = (sn_compare_value - counter) * 1000000 / (50 * minar::platform::Time_Base);
        slots = ((sn_compare_value - counter) * 20000) / minar::platform::Time_Base;
        if ((slots > MAXIMUM_SLOTS) || (timer_enabled == false)) {
            slots = 0;
        }
    }

    return (uint16_t)slots;
}


