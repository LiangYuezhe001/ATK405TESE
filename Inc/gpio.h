/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for
  *                      the gpio
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO?ڵ?ַӳ??
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO?ڲ???,ֻ?Ե?һ??IO??!
//ȷ??n??ֵС??16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //???? 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //???? 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //???? 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //???? 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //???? 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //???? 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //???? 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //???? 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //???? 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //????

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //???? 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //????

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //???? 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //????

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //???? 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //????

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //???? 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //????
////////////////////////////////////////////////////////////////////////////////// 
#define GPIO_A 				0
#define GPIO_B 				1
#define GPIO_C				2
#define GPIO_D 				3
#define GPIO_E 				4
#define GPIO_F 				5
#define GPIO_G 				6 
#define GPIO_H 				7 
#define GPIO_I 				8 

#define FTIR   				1  		//?½??ش???
#define RTIR   				2  		//?????ش???

//GPIO????ר?ú궨??
#define GPIO_MODE_IN    	0		//??ͨ????ģʽ
#define GPIO_MODE_OUT		1		//??ͨ????ģʽ
#define GPIO_MODE_AF		2		//AF????ģʽ
#define GPIO_MODE_AIN		3		//ģ??????ģʽ

#define GPIO_SPEED_2M		0		//GPIO?ٶ?2Mhz
#define GPIO_SPEED_25M		1		//GPIO?ٶ?25Mhz
#define GPIO_SPEED_50M		2		//GPIO?ٶ?50Mhz
#define GPIO_SPEED_100M		3		//GPIO?ٶ?100Mhz

#define GPIO_PUPD_NONE		0		//??????????
#define GPIO_PUPD_PU		1		//????
#define GPIO_PUPD_PD		2		//????
#define GPIO_PUPD_RES		3		//???? 

#define GPIO_OTYPE_PP		0		//????????
#define GPIO_OTYPE_OD		1		//??©???? 

//GPIO???ű??Ŷ???
#define PIN0				1<<0
#define PIN1				1<<1
#define PIN2				1<<2
#define PIN3				1<<3
#define PIN4				1<<4
#define PIN5				1<<5
#define PIN6				1<<6
#define PIN7				1<<7
#define PIN8				1<<8
#define PIN9				1<<9
#define PIN10				1<<10
#define PIN11				1<<11
#define PIN12				1<<12
#define PIN13				1<<13
#define PIN14				1<<14
#define PIN15				1<<15 
////////////////////////////////////////////////////////////////////////////////// 
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
