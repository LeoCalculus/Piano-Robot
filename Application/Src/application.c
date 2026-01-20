#include <application.h>

VOFA_REPORT vofa;
MahonyAHRS ahrs;
controllerProperty cp;
uint8_t VOFA_SEND_BUFFER[64];

PID_t leftHandMotor={
    .P=0.005f,
    .I=0.0f,
    .D=0.0f,
    .integral_max=0.01f
};

PID_t rightHandMotor={
    .P=0.005f,
    .I=0.0f,
    .D=0.0f,
    .integral_max=0.01f
};

void init(){
    memset(&vofa,0,sizeof(vofa));
    vofa.vofaTail[0] = 0x00;
    vofa.vofaTail[1] = 0x00;
    vofa.vofaTail[2] = 0x80;
    vofa.vofaTail[3] = 0x7f;
    // DMA receive to idle toggle on
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, VOFA_SEND_BUFFER, sizeof(VOFA_SEND_BUFFER));

    //Mahony_Init(&ahrs, 3.5f, 0.02f); // initialize mahony

}

void smoothVelocity(float currentPostion, float dt){
    float rawVelocity = (currentPostion - cp.lastPosition) / dt; // find speed
    cp.velocityLPF = cp.velocityLPF * (1.0f - cp.velocityAlpha) + rawVelocity * cp.velocityAlpha; // lpf operation
    cp.lastPosition = currentPostion; // update position
}

void boostKp(float targetPos, float currentPos, float tolerance, PID_t target){
    float KpBase = target.P;
    if (abs(targetPos - currentPos) > tolerance) {
        target.P *= 2;
    } else {
        target.P = KpBase; // restore
    }
}


void controllerUpdate(const float dt){
    // readAcc();
    // Mahony_Update(&ahrs, accData, gyroData, dt);

    // controller code with pid
    //vofa.val[0] = ahrs.roll * 57.2958f; // convert to radius
    //vofa.val[1] = ahrs.pitch * 57.2958f;
    //vofa.val[2] = ahrs.yaw * 57.2958f;
    
    // vofa display those pid stuff
    vofa.val[0] = leftHandMotor.P;
    vofa.val[1] = leftHandMotor.I;
    vofa.val[2] = leftHandMotor.D;

    // 1. update target position
    // 2. find current position
    // 3. update error
    // 4. pid cycle


    HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&vofa, sizeof(vofa));
}