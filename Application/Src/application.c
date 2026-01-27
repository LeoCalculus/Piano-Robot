#include <application.h>

uint8_t rx_buffer[128];
uint8_t DMA_target_location[128];

void pid_cycle(PID_t* target_system, float error, const float dt){
    float derivative = (error - target_system->last_error) / dt;
    
    target_system->Integral = error*dt;
    // bound the integral
    if (target_system->Integral > target_system->Integral_max){
        target_system->Integral = target_system->Integral_max;
    } else if (target_system->Integral < -target_system->Integral_max) {
        target_system->Integral = -target_system->Integral_max;
    }
    // controller output here
    target_system->output = target_system->Kp * error + target_system->Ki * target_system->Integral + target_system->Kd * derivative;
    target_system->last_error = error;
}

// do controller here
void controller_init(void){

}

void controller_step(const float dt){

}