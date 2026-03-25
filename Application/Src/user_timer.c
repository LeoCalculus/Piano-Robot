#include <user_timer.h>

uint8_t timer2_tick_MENU;
uint16_t timer2_tick_test;
uint8_t timer5_tick;
uint8_t menu_update_flag;

void user_timer_tick_init(){
    timer2_tick_MENU = 0;
    timer2_tick_test = 0;
    timer5_tick = 0;
    menu_update_flag = 0;
}

void user_timer2_ISR(){
    
    timer2_tick_MENU++;
    timer2_tick_test++;

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
    
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, SET);
    controller_step(0.001f);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, RESET);
    timer5_tick = 0;
}