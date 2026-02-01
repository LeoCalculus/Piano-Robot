#ifndef __USER_TIMER_ISR_H
#define __USER_TIMER_ISR_H

#include "main.h"

// Timer call back function 
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif
