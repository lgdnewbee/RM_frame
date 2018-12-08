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
#ifdef	AUTOAIM_MODE

//enemy: 目标位置（yaw角、pitch角）		current：当前位置（yaw角、pitch角，从电机回传数据获取）
GMINFO_t enemy,current;
uint8_t autoAimRxBuffer;
//TX_GM_INFO：用于发送当前信息		Rx_enemy_INFO：用于接收上位机传回的数据
uint8_t Tx_GM_INFO[8],Rx_enemy_INFO[8];
uint8_t enemy_INFO_cnt=0,find_enemy=0;
double current_y_tmp,current_p_tmp;
//Rx_error：测试用，数据格式错误时发送
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
	RX_ENEMY_SIGNAL();
}

void autoAim()
{
	RxEnemyINFO();
		
	GMY.TargetAngle=enemy.y;
	GMP.TargetAngle=enemy.p;

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
	
	//*****************************************上位机数据处理************************************************//
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
	if(HAL_UART_Receive_IT(&AutoAim_UART, &autoAimRxBuffer, 1) != HAL_OK)
	{
		Error_Handler();
	}
}

#endif	/*AUTOAIM_MODE*/
#endif	/*DEBUG_MODE*/
