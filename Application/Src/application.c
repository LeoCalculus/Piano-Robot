#include <application.h>

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

void controllerUpdate(const float dt){
    // controller code with pid
}