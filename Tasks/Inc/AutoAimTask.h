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

#define CURRENT_UINT16_T
#define AUTOAIM_MODE

#ifndef DEBUG_MODE
#ifdef	AUTOAIM_MODE

#define RX_ENEMY_Y_TAG		TX_GM_INFO[0]
#define RX_ENEMY_Y_SIGN		TX_GM_INFO[1]
#define RX_ENEMY_Y1 			TX_GM_INFO[2]
#define RX_ENEMY_Y2 			TX_GM_INFO[3]
#define RX_ENEMY_P_TAG		TX_GM_INFO[4]
#define RX_ENEMY_P_SIGN		TX_GM_INFO[5]
#define RX_ENEMY_P1 			TX_GM_INFO[6]
#define RX_ENEMY_P2 			TX_GM_INFO[7]

#define TX_CURRENT_Y_TAG		TX_GM_INFO[0]
#define TX_CURRENT_Y_SIGN		TX_GM_INFO[1]
#define TX_CURRENT_Y1 			TX_GM_INFO[2]
#define TX_CURRENT_Y2 			TX_GM_INFO[3]
#define TX_CURRENT_P_TAG		TX_GM_INFO[4]
#define TX_CURRENT_P_SIGN		TX_GM_INFO[5]
#define TX_CURRENT_P1 			TX_GM_INFO[6]
#define TX_CURRENT_P2 			TX_GM_INFO[7]

#define k_angle   1*360.0/65536.0

typedef struct GMINFO_t
{
	double y;
	double p;
}GMINFO_t;

extern GMINFO_t enemy,current;
extern uint8_t autoAimRxBuffer;

extern uint8_t TX_GM_INFO[8],Rx_enemy_INFO[8];
extern uint8_t enemy_INFO_cnt,find_enemy;

//void UARTdebug(void);
void autoAim(void);
void TxCurrentGMINFO(void);
void RXEnemyINFO(void);
void AutoAimRxCpltCallback(void);
void InitAutoAimUart(void);

#endif	/*AUTOAIM_MODE*/
#endif	/*DEBUG_MODE*/

#endif 	/*__AUTOAIMTASK_H*/
