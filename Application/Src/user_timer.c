#include <user_timer.h>

uint8_t timer2_tick;
uint8_t timer5_tick;
uint8_t menu_update_flag;

void user_timer_tick_init(){
    timer2_tick = 0;
    timer5_tick = 0;
    menu_update_flag = 0;
}

void user_timer2_ISR(){
    
    timer2_tick++;

    if(timer2_tick >= MENU_FRAME_RATE){
        menu_update_flag = 1;
        timer2_tick = 0;
    }
}

void user_timer5_ISR(){

    timer5_tick++;
    
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, SET);
    controller_step(0.001f);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, RESET);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
    timer5_tick = 0;
}