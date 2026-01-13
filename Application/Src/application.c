#include <application.h>

VOFA_REPORT vofa;

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

}

void controllerUpdate(const float dt){

    // controller code with pid
    vofa.val[0] = accData[0];
    vofa.val[1] = accData[1];
    vofa.val[2] = accData[2];

    // Abort any stuck transfer and restart
    // HAL_UART_AbortTransmit(&huart2);
    HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&vofa, sizeof(vofa));
}