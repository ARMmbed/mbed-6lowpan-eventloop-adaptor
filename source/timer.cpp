#include "mbed.h"

extern "C" {
#include "nanostack-event-loop/platform/arm_hal_timer.h"
} // extern "C"


/**
 * \brief This function perform timer init.
 */
void platform_timer_enable(void)
{
    // !!! TODO
}

/**
 * \brief This function is API for set Timer interrupt handler for stack
 *
 * \param new_fp Function pointer for stack giving timer handler
 *
 */
void platform_timer_set_cb(void (*new_fp)(void))
{
    // !!! TODO
}

/**
 * \brief This function is API for stack timer start
 *
 * \param slots define how many 50us slot time period will be started
 *
 */
void platform_timer_start(uint16_t slots)
{
    // !!! TODO
}

/**
 * \brief This function is API for stack timer stop
 *
 */
void platform_timer_disable(void)
{
    // !!! TODO
}

/**
 * \brief This function is API for stack timer to read active timer remaining slot count
 *
 * \return 50us time slot remaining
 */
uint16_t platform_timer_get_remaining_slots(void)
{
    // !!! TODO
}


