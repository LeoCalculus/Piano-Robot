#ifndef __USER_TIMER_H
#define __USER_TIMER_H

#include <stdint.h>
#include "config.h"
#include "menu.h"

void wait_ms(uint16_t ms);
void user_timer_tick_init(void);
void user_timer2_ISR(void);
void user_timer5_ISR(void);
#endif