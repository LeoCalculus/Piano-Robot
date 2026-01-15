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

void Mahony_Init(MahonyAHRS *mahony, float Kp, float Ki) {
    (void)Kp; (void)Ki;

    mahony->q[0]=1.0f; mahony->q[1]=mahony->q[2]=mahony->q[3]=0.0f;
    mahony->integralFB[0]=mahony->integralFB[1]=mahony->integralFB[2]=0.0f;

    // Tuned values: somewhat aggressive but with HF suppression and smooth boost
    mahony->Kp = 3.5f;             // base proportional gain
    mahony->Kp_max = 30.0f;        // allowed boosted Kp (short)
    mahony->Ki = 0.02f;            // small integral
    mahony->Kd = 0.03f;            // small derivative damping

    mahony->roll = mahony->pitch = mahony->yaw = 0.0f;
}

void Mahony_Update(MahonyAHRS *mahony, const float acc[3], const float gyro[3], float dt){
    float normalizedValue;
    float copy_acc[3], copy_gyro[3];
    copy_acc[0] = acc[0]; // x
    copy_acc[1] = acc[1]; // y
    copy_acc[2] = acc[2]; // z
    copy_gyro[0] = gyro[0];
    copy_gyro[1] = gyro[1];
    copy_gyro[2] = gyro[2];
    float theory_gx, theory_gy, theory_gz;
    float q0 = mahony->q[0], q1 = mahony->q[1], q2 = mahony->q[2], q3 = mahony->q[3];
    float qa, qb, qc;
    float error_x, error_y, error_z;

    // only when the accleration data is valid then do calculation:
    if(!(acc[0]==0.0f && acc[1]==0.0f && acc[2]==0.0f)){
        // normalized the measured acceleration vector
        normalizedValue = 1/sqrt(acc[0]*acc[0] + acc[1]*acc[1] + acc[2]*acc[2]);
        copy_acc[0] *= normalizedValue; 
        copy_acc[1] *= normalizedValue;
        copy_acc[2] *= normalizedValue;

        // calculate theory gravity:
        theory_gx = (q1*q3-q0*q2);
        theory_gy = (q2*q3+q0*q1);
        theory_gz = (0.5f-q1*q1-q2*q2); // equvalent half form

        // find error with actual gravity by cross product and find the angle between them (error is angle between them):
        error_x = (copy_acc[1]*theory_gz-copy_acc[2]*theory_gy);
        error_y = (copy_acc[2]*theory_gx-copy_acc[0]*theory_gz);
        error_z = (copy_acc[0]*theory_gy-copy_acc[1]*theory_gx);

        // use PI controller: P and I in mahony struct already
        // here for KI fix
        if (mahony->Ki > 0.0f){
            mahony->integralFB[0] += mahony->Ki * (error_x) * dt;
            mahony->integralFB[1] += mahony->Ki * (error_y) * dt;
            mahony->integralFB[2] += mahony->Ki * (error_z) * dt;
            
            // apply gyro patch
            copy_gyro[0] += mahony->integralFB[0];
            copy_gyro[1] += mahony->integralFB[1];
            copy_gyro[2] += mahony->integralFB[2];
        } else {
            mahony->integralFB[0] = 0.0f;
            mahony->integralFB[1] = 0.0f;
            mahony->integralFB[2] = 0.0f;
        }

        // here for KP fix, recall PID is done by P,I and D superpostion:
        copy_gyro[0] += mahony->Kp * error_x;
        copy_gyro[1] += mahony->Kp * error_y;
        copy_gyro[2] += mahony->Kp * error_z;
    }

    // find the new quaternion:
    copy_gyro[0] *= (0.5f * dt);
    copy_gyro[1] *= (0.5f * dt);
    copy_gyro[2] *= (0.5f * dt);

    qa = q0;
    qb = q1;
    qc = q2;

    q0 += (-qb * copy_gyro[0] - qc * copy_gyro[1] - q3 * copy_gyro[2]); 
	q1 += (qa * copy_gyro[0] + qc * copy_gyro[2] - q3 * copy_gyro[1]);
	q2 += (qa * copy_gyro[1] - qb * copy_gyro[2] + q3 * copy_gyro[0]);
	q3 += (qa * copy_gyro[2] + qb * copy_gyro[1] - qc * copy_gyro[0]); 

    // normalized the updated quaternion:
    normalizedValue = 1.0f/sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 *= normalizedValue;
    q1 *= normalizedValue;
    q2 *= normalizedValue;
    q3 *= normalizedValue;

    // store them back to mahony struct
    mahony->q[0] = q0;
    mahony->q[1] = q1;
    mahony->q[2] = q2;
    mahony->q[3] = q3;

    // find the angle: math function out radius
    mahony->roll  = atan2f(2.0f*(q0*q1 + q2*q3), 1.0f - 2.0f*(q1*q1 + q2*q2));
    mahony->pitch = asinf(clamp_f(-2.0f*(q1*q3 - q0*q2), -1.0f, 1.0f));
    mahony->yaw   = atan2f(2.0f*(q0*q3 + q1*q2), 1.0f - 2.0f*(q2*q2 + q3*q3));

}


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