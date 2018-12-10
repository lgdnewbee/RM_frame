/**
  ******************************************************************************
  *FileName				: AutoAimTask.c
  *Description		: ◊‘√È≥Ã–Ú
  *Author					: π‹“◊∫„
  ******************************************************************************
  *
  * Copyright (c) 2019 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
*/

#include "includes.h"

#ifdef	AUTOAIM_MODE

//enemy: ƒø±ÍŒª÷√£®yawΩ«°¢pitchΩ«£©		current£∫µ±«∞Œª÷√£®yawΩ«°¢pitchΩ«£¨¥”µÁª˙ªÿ¥´ ˝æ›ªÒ»°£©
GMINFO_t enemy,current;
uint8_t autoAimRxBuffer;
//TX_GM_INFO£∫”√”⁄∑¢ÀÕµ±«∞–≈œ¢		Rx_enemy_INFO£∫”√”⁄Ω” ’…œŒªª˙¥´ªÿµƒ ˝æ›
uint8_t Tx_GM_INFO[8],Rx_enemy_INFO[8];
uint8_t enemy_INFO_cnt=0,find_enemy=0;
double current_y_tmp,current_p_tmp;
//Rx_error£∫≤‚ ‘”√£¨ ˝æ›∏Ò Ω¥ÌŒÛ ±∑¢ÀÕ
uint8_t Rx_error[6]="error";

void RxEnemyINFO()
{
	Rx_enemy_INFO[enemy_INFO_cnt++]=autoAimRxBuffer;
	if(enemy_INFO_cnt>=8)
	{
		enemy_INFO_cnt=0;
		if(RX_ENEMY_Y_TAG=='Y'&&RX_ENEMY_P_TAG=='P')
		{
			enemy.y=((RX_ENEMY_Y_SIGN=='-')?(-1.0):1.0)*(double)(((uint16_t)RX_ENEMY_Y1<<8)|(uint16_t)RX_ENEMY_Y2)*k_angle;
			enemy.p=((RX_ENEMY_P_SIGN=='-')?(-1.0):1.0)*(double)(((uint16_t)RX_ENEMY_P1<<8)+(uint16_t)RX_ENEMY_P2)*k_angle;
			find_enemy=1;
		}
		else
		{
			RX_ENEMY_ERROR();
			enemy_INFO_cnt=0;
		}
	}
<<<<<<< HEAD
	else if((enemy_INFO_cnt>0&&RX_ENEMY_Y_TAG!='Y')||(enemy_INFO_cnt>4&&RX_ENEMY_P_TAG!='P'))
	{
		RX_ENEMY_ERROR();
		enemy_INFO_cnt=0;
	}
=======
>>>>>>> parent of cea3756... Ëá™ÁûÑÁ¨¨‰∫åÁâàÁ®ãÂ∫è
	RX_ENEMY_SIGNAL();
}

void autoAim()
{
	RxEnemyINFO();
		
	//GMY.TargetAngle=enemy.y;
	//GMP.TargetAngle=enemy.p;

	current.y=GMY.RealAngle;
	current.p=GMP.RealAngle;
	
	TxCurrentGMINFO();
}

void TxCurrentGMINFO()
{
	current_y_tmp=current.y/k_angle;
	current_p_tmp=current.p/k_angle;
	
	TX_CURRENT_Y_TAG	='Y';
	TX_CURRENT_Y_SIGN	=((current.y<0)?'-':'+');
	TX_CURRENT_Y1			=(((uint16_t)current_y_tmp>>8)&0x00ff);
	TX_CURRENT_Y2			=(((uint16_t)current_y_tmp)&0x00ff);
	TX_CURRENT_P_TAG	='P';
	TX_CURRENT_P_SIGN	=((current.p<0)?'-':'+');
	TX_CURRENT_P1			=(((uint16_t)current_p_tmp>>8)&0x00ff);
	TX_CURRENT_P2			=(((uint16_t)current_p_tmp)&0x00ff);
	
	if(find_enemy)
	{
		TX_CURRENT_GMINFO();
		find_enemy=0;
	}
	
	//*****************************************…œŒªª˙ ˝æ›¥¶¿Ì************************************************//
	//current_yaw		=((TX_CURRENT_Y_SIGN=='-')?(-1.0):1.0)*\
	//							 (double)(((uint16_t)TX_CURRENT_Y1<<8)|((uint16_t)TX_CURRENT_Y2))*k_angle;
	//current_pitch	=((TX_CURRENT_P_SIGN=='-')?(-1.0):1.0)*\
	//							 (double)(((uint16_t)TX_CURRENT_P1<<8)|((uint16_t)TX_CURRENT_P2))*k_angle;
	//*******************************************************************************************************//
	
}

void AutoAimRxCpltCallback()
{
	autoAim();
}

void InitAutoAimUart()
{
	if(HAL_UART_Receive_IT(&AUTOAIM_UART, &autoAimRxBuffer, 1) != HAL_OK)
	{
		Error_Handler();
	}
}

#endif	/*AUTOAIM_MODE*/
