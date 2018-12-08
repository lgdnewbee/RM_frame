/**
  ******************************************************************************
  * File Name          : AutoAimtask.h
  * Description        : 
  ******************************************************************************
  *
  * Copyright (c) 2019 Team TPP-Shanghai Jiao Tong University
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef __AUTOAIMTASK_H
#define __AUTOAIMTASK_H

#include "includes.h"

#ifndef DEBUG_MODE
#ifdef	AUTOAIM_MODE

#define RX_ENEMY_ERROR()		HAL_UART_Transmit_DMA(&AutoAim_UART,(uint8_t *)&Rx_error,sizeof(Rx_error))
#define RX_ENEMY_SIGNAL()		HAL_UART_Receive_DMA(&AutoAim_UART,(uint8_t *)&autoAimRxBuffer,1)
#define TX_CURRENT_GMINFO()	HAL_UART_Transmit_DMA(&AutoAim_UART,(uint8_t *)&Tx_GM_INFO,8)

#define RX_ENEMY_Y_TAG		Rx_enemy_INFO[0]
#define RX_ENEMY_Y_SIGN		Rx_enemy_INFO[1]
#define RX_ENEMY_Y1 			Rx_enemy_INFO[2]
#define RX_ENEMY_Y2 			Rx_enemy_INFO[3]
#define RX_ENEMY_P_TAG		Rx_enemy_INFO[4]
#define RX_ENEMY_P_SIGN		Rx_enemy_INFO[5]
#define RX_ENEMY_P1 			Rx_enemy_INFO[6]
#define RX_ENEMY_P2 			Rx_enemy_INFO[7]

#define TX_CURRENT_Y_TAG		Tx_GM_INFO[0]
#define TX_CURRENT_Y_SIGN		Tx_GM_INFO[1]
#define TX_CURRENT_Y1 			Tx_GM_INFO[2]
#define TX_CURRENT_Y2 			Tx_GM_INFO[3]
#define TX_CURRENT_P_TAG		Tx_GM_INFO[4]
#define TX_CURRENT_P_SIGN		Tx_GM_INFO[5]
#define TX_CURRENT_P1 			Tx_GM_INFO[6]
#define TX_CURRENT_P2 			Tx_GM_INFO[7]

#define k_angle   (1*180.0/65536.0)

typedef struct GMINFO_t
{
	double y;
	double p;
}GMINFO_t;

extern GMINFO_t enemy,current;
extern uint8_t autoAimRxBuffer;

void autoAim(void);
void TxCurrentGMINFO(void);
void RXEnemyINFO(void);
void AutoAimRxCpltCallback(void);
void InitAutoAimUart(void);

#endif	/*AUTOAIM_MODE*/
#endif	/*DEBUG_MODE*/

#endif 	/*__AUTOAIMTASK_H*/
