/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed-drivers/mbed.h"
#include "us_ticker_api.h"

extern "C" {
#include "nanostack-event-loop/platform/arm_hal_timer.h"
} // extern "C"


static void (*sn_callback)(void) = NULL;
static Ticker sn_timer;
static timestamp_t sn_compare_value;
static bool timer_enabled = false;

#define MAXIMUM_SLOTS 10000


/**
 * \brief This function perform timer init.
 */
void platform_timer_enable(void)
{
    //init in Ticker ctor
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
    sn_compare_value = (timestamp_t)slots * 50; // 1 slot = 50us
    sn_timer.attach_us(sn_callback, sn_compare_value);
    sn_compare_value += us_ticker_read();
    timer_enabled = true;
}

/**
 * \brief This function is API for stack timer stop
 *
 */
void platform_timer_disable(void)
{
    sn_timer.detach();
    timer_enabled = false;
}

/**
 * \brief This function is API for stack timer to read active timer remaining slot count
 *
 * \return 50us time slot remaining
 */
uint16_t platform_timer_get_remaining_slots(void)
{
    uint32_t counter = us_ticker_read(), slots = 0;

    if (sn_compare_value >= counter)
    {
        slots = (sn_compare_value - counter) / 50;
        if ((slots > MAXIMUM_SLOTS) || (timer_enabled == false)) {
            slots = 0;
        }
    }

    return (uint16_t)slots;
}


