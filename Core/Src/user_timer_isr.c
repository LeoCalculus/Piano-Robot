#include "main.h"
#include "elec391.h"
#include "encoder.h"
#include "usart.h"
#include "VOFA.h"
#include "pid.h"
#include "tim.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

  // timer 2 for general timer count
  if (htim->Instance == TIM2){

    // limit button check
    if(limit_count_en == 1){
      msec_count_limit++;
    }

    // frame scaling
    msec_count_frame++;
    if(msec_count_frame >= FRAME_RATE){
        msec_count_frame = 0;
        main_update = 1;
    }
  }

  // timer 4 for measurement
  else if(htim->Instance == TIM4){

    static int16_t counter;
    // measure scaling 
    half_msec_count_measure++;
    if(half_msec_count_measure >= MEASURE_RATE * 2){
      half_msec_count_measure = 0;

      counter = get_encoder();
      counter_acc += counter;

      speed_10000 = 38961 * counter; // 81818 for deg/s, 13636 for rpm, 142800 for rad/s devide 35 for shaft
      rad_s_10000 = 142800 * counter;
      location_1000 = 234 * counter_acc; // 818 for deg, 2 for r

      pid_control();
    }
  }
}
