/**
  ******************************************************************************
  *FileName				: AutoAimTask.c
  *Description		: 自瞄程序
  *Author					: 管易恒
  ******************************************************************************
  *
  * Copyright (c) 2019 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
*/

#include "includes.h"

#ifndef DEBUG_MODE
#ifdef	USE_AUTOAIM

//enemy: 目标位置（yaw角、pitch角）		current：当前位置（yaw角、pitch角，从电机回传数据获取）
GMINFO_t enemy,current;
uint8_t autoAimRxBuffer;
uint8_t Tx_GM_INFO[8],Rx_enemy_INFO[8];
uint8_t enemy_INFO_cnt=0,find_enemy=0,aim_cnt=0;
double current_y_tmp,current_p_tmp;

void InitAutoAimUart()
{
	if(HAL_UART_Receive_DMA(&AUTOAIM_UART, &autoAimRxBuffer, 1) != HAL_OK)
	{
		Error_Handler();
	}
	if(HAL_UART_Transmit_DMA(&AUTOAIM_UART,(uint8_t *)&Tx_GM_INFO,8) != HAL_OK)
	{
		Error_Handler();
	}		
}

void GMGetCurrentPosition()
{
	current.y=GMY.RealAngle;
	current.p=GMP.RealAngle;
	current_y_tmp=current.y/k_angle;
	current_p_tmp=current.p/k_angle;
}

void autoAim()//稳定性待检测
{
	if(find_enemy)
	{
		if(aim_cnt<25)
		{
			GMY.TargetAngle+=enemy.y/25;
			GMP.TargetAngle-=enemy.p/25;
			aim_cnt++;
		}
		else
		{
			aim_cnt=0;
			find_enemy=0;
		}
	}
}

void AutoAimTxCpltCallback()
{
	TX_CURRENT_START	='s';
	TX_CURRENT_Y_SIGN	=((current.y<0)?'-':'+');
	TX_CURRENT_Y1			=(((uint16_t)current_y_tmp>>8)&0x00ff);
	TX_CURRENT_Y2			=(((uint16_t)current_y_tmp)&0x00ff);
	TX_CURRENT_P_SIGN	=((current.p<0)?'-':'+');
	TX_CURRENT_P1			=(((uint16_t)current_p_tmp>>8)&0x00ff);
	TX_CURRENT_P2			=(((uint16_t)current_p_tmp)&0x00ff);
	TX_CURRENT_END		='e';
	TX_CURRENT_GMINFO();
}

void AutoAimRxCpltCallback()
{
	Rx_enemy_INFO[enemy_INFO_cnt++]=autoAimRxBuffer;
	if(enemy_INFO_cnt>=8)
	{
		enemy_INFO_cnt=0;
		if(RX_ENEMY_START=='s'&&RX_ENEMY_END=='e')
		{
			enemy.y=((RX_ENEMY_Y_SIGN=='-')?(-1.0):1.0)*(double)(((uint16_t)RX_ENEMY_Y1<<8)|(uint16_t)RX_ENEMY_Y2)*k_angle;
			enemy.p=((RX_ENEMY_P_SIGN=='-')?(-1.0):1.0)*(double)(((uint16_t)RX_ENEMY_P1<<8)+(uint16_t)RX_ENEMY_P2)*k_angle;
			find_enemy=1;
		}
		else
		{
			enemy_INFO_cnt=0;
		}
	}
	else if(RX_ENEMY_START!='s')
	{
		enemy_INFO_cnt=0;
	}
	RX_ENEMY_SIGNAL();
}

#endif /*USE_AUTOAIM*/
#endif /*DEBUG_MODE*/
