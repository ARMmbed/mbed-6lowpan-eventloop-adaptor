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
#include "nanostack-event-loop/eventOS_scheduler.h"
#include "platform/arm_hal_interrupt.h"
#include "minar/minar.h"
#include "core-util/FunctionPointer.h"

using minar::Scheduler;
using namespace mbed::util;

/** Global variable which indicate when interrupt are disabled */
volatile uint8_t sys_irq_disable_counter = 0;
static volatile bool run_scheduled = false;

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

void platform_interrupts_disabled(void)
{
    ++sys_irq_disable_counter;
}

void platform_interrupts_enabling(void)
{
    --sys_irq_disable_counter;
}

static void run_until_idle()
{
    eventOS_scheduler_run_until_idle();
    run_scheduled = false;
}

/**
 * \brief This function will be called when stack receives an event.
 */
void eventOS_scheduler_signal(void)
{
    if (!run_scheduled) {
        Scheduler::postCallback(FunctionPointer0<void>(run_until_idle).bind()).tolerance(0);
        run_scheduled = true;
    }
}

void eventOS_scheduler_idle(void)
{
    /*
     * No need to port eventOS_scheduler_idle in mbedOS, use empty function to keep
     * compiler happy.
     */
}
