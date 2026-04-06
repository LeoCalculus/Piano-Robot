#include <user_timer.h>
#include <application.h>
#include <command.h>

uint8_t timer2_tick_MENU;
uint16_t timer2_tick_test;
uint8_t timer5_tick;
uint8_t menu_update_flag;

volatile uint16_t wait_timer = 0;

void user_timer_tick_init(){
    timer2_tick_MENU = 0;
    timer2_tick_test = 0;
    timer5_tick = 0;
    menu_update_flag = 0;
}

void user_timer2_ISR(){
    
    timer2_tick_MENU++;
    timer2_tick_test++;
    wait_timer++;

    if(timer2_tick_MENU >= MENU_FRAME_RATE){
        menu_update_flag = 1;
        timer2_tick_MENU = 0;
    }

    if(timer2_tick_test >= 500){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        timer2_tick_test = 0;
    }
}

void user_timer5_ISR(){

    timer5_tick++;

    if(pwm_mode == PWM_PID){
        controller_step(0.001f);
    }

    else if(pwm_mode == PWM_STOP){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);
    }

    else if(pwm_mode == PWM_HOMING_LEFT_START){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 200);
    }

    else if(pwm_mode == PWM_HOMING_LEFT){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 270);
    }

    else if(pwm_mode == PWM_HOMING_RIGHT_START){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 200);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);
    }

    else if(pwm_mode == PWM_HOMING_RIGHT){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 270);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);
    }

    timer5_tick = 0;
}

void wait_ms(uint16_t ms){
    // reset the wait counter first:
    wait_timer = 0;
    while(wait_timer < ms){
        // busy wait
    }
}