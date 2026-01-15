#ifndef __UTILS_H
#define __UTILS_H

#define PI (3.1415926f)

typedef struct PID_t{
    float P;
    float I;
    float D;
    const float integral_max; // avoid the integrate result too big
    float integral;
    float err_m1;
}PID_t;

float clamp_f(float val, float min, float max);
float pid_cycle(PID_t *sys, float err, const float delta_t);


#endif