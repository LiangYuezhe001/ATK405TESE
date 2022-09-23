#include "kalman.h"
#include "math.h"
#include "ANO_DT.h"

//extern float pitch, roll;
int icc=0;
float Q_ROLL=0.001;
float Q_PITCH=0.001;

float Q_Yaw=0.01,kk=0.15,Compound_G=0;

float R_ROLL=0.1;//
float R_PITCH=0.1;

float R_Yaw=0.1;

float dt=0.0017;
float yaw_dt=0.01;
float oyaw,fyaw,dyaw,yaw_bias;
float DeltaPitch;
float DeltaRoll;
float DeltaYaw;

/* 说明： */
/* 1.陀螺仪y轴指向正方向，绕y轴的角是Roll角，绕x轴的角是Pitch角 */

float Pitch_Kalman,Roll_Kalman,Yaw_Kalman;                     //卡尔曼滤波最终输出的角度
float Pitch_tan,Roll_tan;

float Roll_z,Pitch_z,Yaw_z;                               //加速度计计算的角度值
float Roll_hat_pri=0,Pitch_hat_pri=0,Yaw_hat_pri=0;               // 角度先验估计
float Roll_hat_pos=0,Pitch_hat_pos=0,Yaw_hat_pos=0;               // 角度后验估计
float P_Roll_hat_pri=0,P_Pitch_hat_pri=0,P_Yaw_hat_pri=0;           // 方差先验估计
float P_Roll_hat_pos=0,P_Pitch_hat_pos=0,P_Yaw_hat_pos=0;           // 方差后验估计
float KalmanGain_Pitch,KalmanGain_Roll,KalmanGain_Yaw;             // 卡尔曼增益
float Roll_Diff_Q=0,Pitch_Diff_Q=0,Roll_Diff_R=0,Pitch_Diff_R=0,to_Roll_Diff_R=0,to_Pitch_Diff_R=0,to_Roll_Diff_Q=0,to_Pitch_Diff_Q=0;

float ABs(float a)
{
	return a < 0 ? -a : a;
}
float limit(float a)
{
	if(a>=3)a=3;
	if(a<=0.001f)a=0.001;
	return a;
}
float limit2(float a)
{
	if(a<=0.1f)a=0.1;
	return a;
}
void KalmanCalculation(axis* gyro ,axis* acc_real,float* pitch_out,float* roll_out)
{
    /*-----------------------------------------卡尔曼滤波--------------------------------------------------*/

    // 参数说明：A=1，B=dt，控制量=角速度，观测值直接测得角度H=1，
    Pitch_tan=acc_real->y/(sqrt(acc_real->x*acc_real->x+acc_real->z*acc_real->z));
    Roll_tan=acc_real->x/(sqrt(acc_real->y*acc_real->y+acc_real->z*acc_real->z));
	
    Pitch_z = (atan(Pitch_tan)) * 180 / 3.1415f;
    Roll_z =-(atan(Roll_tan)) * 180 / 3.1415f;
    Yaw_z += (gyro->z-yaw_bias)*yaw_dt;


    // 预测：本次角度先验估计 = 转移矩阵 * 上次角度后验估计 + 控制量
    Pitch_hat_pri = 1 * Pitch_hat_pos + gyro->x*dt;
    Roll_hat_pri = 1 * Roll_hat_pos +  gyro->y*dt;
    Yaw_hat_pri = 1 * Yaw_hat_pos + dyaw*yaw_dt ;

    // 预测：本次方差先验估计 = 转移矩阵 * 上次方差后验估计 * 转移矩阵转置 + Q
    P_Pitch_hat_pri = P_Pitch_hat_pos + Q_PITCH;
    P_Roll_hat_pri = P_Roll_hat_pos + Q_ROLL;
    P_Yaw_hat_pri = P_Yaw_hat_pos + Q_Yaw;

    // 更新：计算卡尔曼增益
    KalmanGain_Pitch = (P_Pitch_hat_pri)/(P_Pitch_hat_pri+R_PITCH);
    KalmanGain_Roll = (P_Roll_hat_pri)/(P_Roll_hat_pri+R_ROLL);
    KalmanGain_Yaw = (P_Yaw_hat_pri)/(P_Yaw_hat_pri+R_Yaw);

    // 更新：计算后验估计
		
    Pitch_hat_pos = Pitch_hat_pri + KalmanGain_Pitch*(Pitch_z-Pitch_hat_pri);
    Roll_hat_pos = Roll_hat_pri + KalmanGain_Roll*(Roll_z-Roll_hat_pri);
    Yaw_hat_pos = Yaw_hat_pri + KalmanGain_Yaw*(Yaw_z-Yaw_hat_pri);

    // 更新：计算新的方差后验估计
    P_Pitch_hat_pos = (1-KalmanGain_Pitch)*P_Pitch_hat_pri;
    P_Roll_hat_pos = (1-KalmanGain_Roll)*P_Roll_hat_pri;
    P_Yaw_hat_pos = (1-KalmanGain_Yaw)*P_Yaw_hat_pri;

    // 为了方便PID而进行的微分运算
    DeltaPitch = (-Pitch_hat_pos)-Pitch_Kalman;                         
    DeltaRoll = (-Roll_hat_pos)-Roll_Kalman;
    DeltaYaw= (-Yaw_hat_pos)-Yaw_Kalman;

    //输出
		fyaw = 0.8*oyaw+0.2*Pitch_hat_pos;
    oyaw=fyaw;
    *pitch_out = fyaw;
    *roll_out = Roll_hat_pos;
   // Yaw_Kalman =Yaw_hat_pos;
	 
//		Pitch_Diff_R=ABs(fyaw-Pitch_z);
//		Pitch_Diff_Q=ABs(fyaw-Pitch_hat_pri);
//		to_Pitch_Diff_Q+=Pitch_Diff_Q;
//		to_Pitch_Diff_R+=Pitch_Diff_R;
//		if(icc>=19)
//		{	to_Pitch_Diff_Q/=100;
//			to_Pitch_Diff_R/=20;
//			//Q_PITCH=0.9*Q_PITCH+0.1*limit(to_Pitch_Diff_Q*to_Pitch_Diff_Q);
//			R_PITCH=0.8*R_PITCH+0.2*limit(to_Pitch_Diff_R*to_Pitch_Diff_R);
//			to_Pitch_Diff_Q=0;
//			to_Pitch_Diff_R=0;
//			icc=0;
//		}
//		icc++;

}
void complementary_filter(axis* gyro ,axis* acc_real,float* pitch_out,float* roll_out)
{
	Compound_G = acc_real->x*acc_real->x + acc_real->y*acc_real->y + acc_real->z*acc_real->z;
	Pitch_tan=acc_real->y/(sqrt(acc_real->x*acc_real->x+acc_real->z*acc_real->z));
    Roll_tan=acc_real->x/(sqrt(acc_real->y*acc_real->y+acc_real->z*acc_real->z));
		if(Compound_G>=1.1)kk=0;
		else kk=0.15;
    Pitch_z = (atan(Pitch_tan)) * 180 / 3.1415f;
    Roll_z =-(atan(Roll_tan)) * 180 / 3.1415f;
	
    Pitch_hat_pri = 1 * Pitch_hat_pos + gyro->x*dt;
    Roll_hat_pri = 1 * Roll_hat_pos +  gyro->y*dt;
    Yaw_hat_pri = 1 * Yaw_hat_pos + dyaw*yaw_dt ;

		Pitch_hat_pos = Pitch_hat_pri + kk*(Pitch_z-Pitch_hat_pri);
    Roll_hat_pos = Roll_hat_pri + kk*(Roll_z-Roll_hat_pri);
	
		
		*pitch_out = Pitch_hat_pos;
    *roll_out = Roll_hat_pos;
	
}

t_fp_vector_def rotate(axis* acc, float pitch_in,float roll_in)
{
	fp_angles_t body_angle;
	t_fp_vector_def acc_vector;
	
		acc_vector.Y = acc->x;
    acc_vector.X = acc->y;
    acc_vector.Z = acc->z;
	
		body_angle.angles.pitch = pitch_in * DEG2RAD;
    body_angle.angles.yaw = 0;
    body_angle.angles.roll = roll_in * DEG2RAD;
	
	rotateV(&acc_vector, &body_angle);
	 return acc_vector;
	
}





