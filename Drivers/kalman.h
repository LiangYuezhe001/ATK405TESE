#include "mpu6000.h"
#include "maths.h"
void KalmanCalculation(axis* gyro ,axis* acc_real,float* pitch_out,float* roll_out);
void complementary_filter(axis* gyro ,axis* acc_real,float* pitch_out,float* roll_out);
t_fp_vector_def rotate(axis* acc, float pitch_in,float roll_in);