#include "main.h"

uint16_t capureVal;
uint16_t capureValDiff;
uint16_t prevCapureVal;

u16 rc_sginal[11]={0};
u8 rc_channel_cnt=1,rc_flag=0;


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