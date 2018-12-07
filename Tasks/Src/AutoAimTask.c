/**
  ******************************************************************************
  *FileName				: AutoAimTask.c
  *Description		: �������
  *Author					: ���׺�
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

//enemy: Ŀ��λ�ã�yaw�ǡ�pitch�ǣ�		current����ǰλ�ã�yaw�ǡ�pitch�ǣ��ӵ���ش����ݻ�ȡ��
GMINFO_t enemy,current;
uint8_t autoAimRxBuffer;
//TX_GM_INFO�����ڷ��͵�ǰ��Ϣ		Rx_enemy_INFO�����ڽ�����λ�����ص�����
uint8_t TX_GM_INFO[8],Rx_enemy_INFO[8];
uint8_t enemy_INFO_cnt=0,find_enemy=0;
//Rx_error�������ã����ݸ�ʽ����ʱ����
uint8_t Rx_error[5]="error";

void RxEnemyINFO()
{
	Rx_enemy_INFO[enemy_INFO_cnt++]=autoAimRxBuffer;
	if(enemy_INFO_cnt++>=8)
	{
		enemy_INFO_cnt=0;
		if(RX_ENEMY_Y_TAG=='Y'&&RX_ENEMY_P_TAG=='P')
		{
			enemy.y=((RX_ENEMY_Y_SIGN=='-')?(-1.0):1.0)*((double)((RX_ENEMY_Y1<<8)|0x0000)+(double)((RX_ENEMY_Y2)|0x0000))*k_angle;
			enemy.p=((RX_ENEMY_P_SIGN=='-')?(-1.0):1.0)*((double)((RX_ENEMY_P1<<8)|0x0000)+(double)((RX_ENEMY_P2)|0x0000))*k_angle;
			find_enemy=1;
		}
		else
		{
			HAL_UART_Transmit(&AutoAim_UART,(uint8_t *)&Rx_error,sizeof(Rx_error),0xff);
		}
	}
	HAL_UART_Receive_IT(&AutoAim_UART,(uint8_t *)&autoAimRxBuffer,1);
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
	TX_CURRENT_Y_TAG	='Y';
	TX_CURRENT_Y_SIGN	=((Rx_enemy_INFO[1]=='-')?'-':'+');
	TX_CURRENT_Y1			=(uint8_t)(((uint16_t)(current.y/k_angle)>>8)&0x00ff);
	TX_CURRENT_Y2			=(uint8_t)(((uint16_t)(current.y/k_angle)		)&0x00ff);
	TX_CURRENT_P_TAG	='P';
	TX_CURRENT_P_SIGN	=((Rx_enemy_INFO[5]=='-')?'-':'+');
	TX_CURRENT_P1			=(uint8_t)(((uint16_t)(current.y/k_angle)>>8)&0x00ff);
	TX_CURRENT_P2			=(uint8_t)(((uint16_t)(current.y/k_angle)		)&0x00ff);
	
	if(find_enemy)
	{
		HAL_UART_Transmit_DMA(&AutoAim_UART,(uint8_t *)&TX_GM_INFO,8);
		find_enemy=0;
	}
	
	//***************************************************��λ�����ݴ���********************************************************************//
	//current_yaw		=((TX_CURRENT_Y_SIGN=='-')?(-1.0):1.0)*((double)((TX_CURRENT_Y1<<8)|0x0000)+(double)((TX_CURRENT_Y2)|0x0000))*k_angle;
	//current_pitch	=((TX_CURRENT_P_SIGN=='-')?(-1.0):1.0)*((double)((TX_CURRENT_P1<<8)|0x0000)+(double)((TX_CURRENT_P2)|0x0000))*k_angle;
	//*************************************************************************************************************************************//
	
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
