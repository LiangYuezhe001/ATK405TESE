#include "main.h"
#include "pca9685.h"
#define MIN_ANGLE_A 65
#define MAX_ANGLE_A 115
uint16_t capureVal;
uint16_t capureValDiff;
uint16_t prevCapureVal;

u16 rc_sginal[11]={0};
u8 rc_channel_cnt=1,rc_flag=0;
float rc_channel[10],angle[5]={90};


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
    capureVal=HAL_TIM_ReadCapturedValue(&htim9,TIM_CHANNEL_2);//获取当前的捕获值.
    		if(capureVal>2050)
		{
			rc_flag=1;
				rc_channel_cnt=0;
		}
	  if(rc_flag)
		{
			rc_sginal[rc_channel_cnt]=capureVal;
			rc_channel_cnt++;
			if(rc_channel_cnt>=10)rc_flag=0;
		}
	

    __HAL_TIM_SET_COUNTER(&htim9,0);


}

void ca()
{
	for(int cnt=0;cnt<5;cnt++)
		{
		rc_channel[cnt]=((float)rc_sginal[cnt+1]-1000.0f)/1000.0f;
	}
		for(int cnt=0;cnt<5;cnt++)
		{
		angle[cnt]=65+rc_channel[3]*50;
			if(cnt==0)angle[cnt]=angle[cnt]+rc_channel[1]*50-25;
			if(cnt==2)angle[cnt]=angle[cnt]-rc_channel[1]*50+25;
			if(cnt==1)angle[cnt]=angle[cnt]+rc_channel[0]*50-25;
			if(cnt==3)angle[cnt]=angle[cnt]-rc_channel[0]*50+25;
			
			
	}
	
		for(int cnt=0;cnt<5;cnt++)
	{
		if(angle[cnt]<=MIN_ANGLE_A)angle[cnt]=MIN_ANGLE_A;
		if(angle[cnt]>=MAX_ANGLE_A)angle[cnt]=MAX_ANGLE_A;
	}
	for(int cnt=0;cnt<5;cnt++){
		PCA9685_Superfast_SetServoAngle(cnt, angle[cnt]);
	}
	
}

