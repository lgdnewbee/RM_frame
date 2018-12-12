/**
  ******************************************************************************
  * File Name          : FunctionTask.c
  * Description        : 用于记录机器人独有的功能
  ******************************************************************************
  *
  * Copyright (c) 2018 Team TPP-Shanghai Jiao Tong University
  * All rights reserved.
  *
  ******************************************************************************
  */
#include "includes.h"

KeyboardMode_e KeyboardMode = NO_CHANGE;
RampGen_t LRSpeedRamp = RAMP_GEN_DAFAULT;   	//斜坡函数
RampGen_t FBSpeedRamp = RAMP_GEN_DAFAULT;
ChassisSpeed_Ref_t ChassisSpeedRef; 

int32_t auto_counter=0;		//用于准确延时的完成某事件

int16_t channelrrow = 0;
int16_t channelrcol = 0;
int16_t channellrow = 0;
int16_t channellcol = 0;
int16_t testIntensity = 0;

//初始化
void FunctionTaskInit()
{
	LRSpeedRamp.SetScale(&LRSpeedRamp, MOUSE_LR_RAMP_TICK_COUNT);
	FBSpeedRamp.SetScale(&FBSpeedRamp, MOUSR_FB_RAMP_TICK_COUNT);
	LRSpeedRamp.ResetCounter(&LRSpeedRamp);
	FBSpeedRamp.ResetCounter(&FBSpeedRamp);
	
	ChassisSpeedRef.forward_back_ref = 0.0f;
	ChassisSpeedRef.left_right_ref = 0.0f;
	ChassisSpeedRef.rotate_ref = 0.0f;
	
	KeyboardMode=NO_CHANGE;
}

void OptionalFunction()
{
	Cap_Control();
	PowerLimitation();
}

void Limit_and_Synchronization()
{
	//demo
	//MINMAX(UD1.TargetAngle,-900,270);//limit
	MINMAX(GMP.TargetAngle,-35,20);
	MINMAX(GMY.TargetAngle,-35,35);
	//UD2.TargetAngle=-UD1.TargetAngle;//sychronization
	//demo end
}
//******************
//遥控器模式功能编写
//******************

int gateState=0,stirDirection=1,gateStep=1;
int pwm=800;

void RemoteControlProcess(Remote *rc)
{
	if(WorkState <= 0) return;
	//max=660
	channelrrow = (rc->ch0 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET); 
	channelrcol = (rc->ch1 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET); 
	channellrow = (rc->ch2 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET); 
	channellcol = (rc->ch3 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET); 
	if(WorkState == NORMAL_STATE)
	{	
		#ifdef GM_TEST
		HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_SET);
		
		ChassisSpeedRef.forward_back_ref = channelrcol * RC_CHASSIS_SPEED_REF;
		ChassisSpeedRef.left_right_ref   = channelrrow * RC_CHASSIS_SPEED_REF/2;
		#ifdef USE_CHASSIS_FOLLOW
		GMY.TargetAngle += channellrow * RC_GIMBAL_SPEED_REF * 3;
		GMP.TargetAngle -= channellcol * RC_GIMBAL_SPEED_REF * 3;
		#else
		ChassisSpeedRef.rotate_ref = channellrow * RC_ROTATE_SPEED_REF;
		#endif
		#ifdef USE_AUTOAIM
		autoAim();
		#endif /*USE_AUTOAIM*/
		FRICL.TargetAngle = 0;
		FRICR.TargetAngle = 0;
		STIR.TargetAngle=0;
		STIR.RealAngle=0;
		GATE.TargetAngle=0;
		GATE.RealAngle=0;
		gateStep=1;
		
		#ifdef FRIC_PWM_MODE
		pwm=800;
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,800);
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,800);
		#endif /*FRIC_PWM_MODE*/
		#endif /*GM_TEST*/
	}
	if(WorkState == ADDITIONAL_STATE_ONE)
	{
		#ifdef GM_TEST
		HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_SET);
		
		GMY.TargetAngle += channellrow * RC_GIMBAL_SPEED_REF;
		GMP.TargetAngle -= channellcol * RC_GIMBAL_SPEED_REF;
		
		if((pwm<1100)&&(auto_counter/5==0)){
			pwm=pwm+3;
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,pwm);
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,pwm);
		}
		
		if(STIR.TargetAngle-STIR.RealAngle>10){stirDirection=0;STIR.RealAngle=0;STIR.TargetAngle=-5;}
		if(STIR.TargetAngle-STIR.RealAngle<-10){stirDirection=0;STIR.RealAngle=0;STIR.TargetAngle=5;}
		STIR.TargetAngle+=3*stirDirection;
		
		if(gateState==0)
		{
			if(gateStep==1){GATE.TargetAngle=300;gateStep=2;}
			gateState=1;
		}
		if(gateStep==2&&GATE.RealAngle>120){GATE.TargetAngle=0;GATE.RealAngle=0;gateStep=1;stirDirection=1;}
		/*
		if(gateStep==2&&GATE.RealAngle<-110){GATE.TargetAngle=300;gateStep=3;}
		if(gateStep==3&&GATE.RealAngle>0){GATE.TargetAngle=-300;gateStep=4;}
		if(gateStep==4&&GATE.RealAngle<-110){GATE.TargetAngle=0;GATE.RealAngle=0;gateStep=1;stirDirection=1;}
		*/
		#endif /*GM_TEST*/
	}
	if(WorkState == ADDITIONAL_STATE_TWO)
	{
		#ifdef GM_TEST
		GMY.TargetAngle += channellrow * RC_GIMBAL_SPEED_REF;
		GMP.TargetAngle -= channellcol * RC_GIMBAL_SPEED_REF;
		
		HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_SET);
		
		if(STIR.TargetAngle-STIR.RealAngle>10){stirDirection=0;STIR.RealAngle=0;STIR.TargetAngle=-5;}
		if(STIR.TargetAngle-STIR.RealAngle<-10){stirDirection=0;STIR.RealAngle=0;STIR.TargetAngle=5;}
		STIR.TargetAngle += 3*stirDirection;
		if(gateState==1)
		{
			if(gateStep==1){GATE.TargetAngle=300;gateStep=2;}
			gateState=0;
		}
		if(gateStep==2&&GATE.RealAngle>120){GATE.TargetAngle=0;GATE.RealAngle=0;gateStep=1;stirDirection=1;}
		/*
		if(gateStep==2&&GATE.RealAngle<-110){GATE.TargetAngle=300;gateStep=3;}
		if(gateStep==3&&GATE.RealAngle>0){GATE.TargetAngle=-300;gateStep=4;}
		if(gateStep==4&&GATE.RealAngle<-110){GATE.TargetAngle=0;GATE.RealAngle=0;gateStep=1;stirDirection=1;}*/
		#endif /*GM_TEST*/
	}
	Limit_and_Synchronization();
}


uint16_t KM_FORWORD_BACK_SPEED 	= NORMAL_FORWARD_BACK_SPEED;
uint16_t KM_LEFT_RIGHT_SPEED  	= NORMAL_LEFT_RIGHT_SPEED;
void KeyboardModeFSM(Key *key);

//****************
//键鼠模式功能编写
//****************
void MouseKeyControlProcess(Mouse *mouse, Key *key)
{	
	if(WorkState <= 0) return;
	
	MINMAX(mouse->x, -150, 150); 
	MINMAX(mouse->y, -150, 150); 

	KeyboardModeFSM(key);
	
	switch (KeyboardMode)
	{
		case SHIFT_CTRL:		//State control
		{
			
			break;
		}
		case CTRL:				//slow
		{
			
		}//DO NOT NEED TO BREAK
		case SHIFT:				//quick
		{
			
		}//DO NOT NEED TO BREAK
		case NO_CHANGE:			//normal
		{//CM Movement Process
			if(key->v & KEY_W)  		//key: w
				ChassisSpeedRef.forward_back_ref =  KM_FORWORD_BACK_SPEED* FBSpeedRamp.Calc(&FBSpeedRamp);
			else if(key->v & KEY_S) 	//key: s
				ChassisSpeedRef.forward_back_ref = -KM_FORWORD_BACK_SPEED* FBSpeedRamp.Calc(&FBSpeedRamp);
			else
			{
				ChassisSpeedRef.forward_back_ref = 0;
				FBSpeedRamp.ResetCounter(&FBSpeedRamp);
			}
			if(key->v & KEY_D)  		//key: d
				ChassisSpeedRef.left_right_ref =  KM_LEFT_RIGHT_SPEED * LRSpeedRamp.Calc(&LRSpeedRamp);
			else if(key->v & KEY_A) 	//key: a
				ChassisSpeedRef.left_right_ref = -KM_LEFT_RIGHT_SPEED * LRSpeedRamp.Calc(&LRSpeedRamp);
			else
			{
				ChassisSpeedRef.left_right_ref = 0;
				LRSpeedRamp.ResetCounter(&LRSpeedRamp);
			}
		}
	}
	Limit_and_Synchronization();
}

void KeyboardModeFSM(Key *key)
{
	if((key->v & 0x30) == 0x30)//Shift_Ctrl
	{
		KM_FORWORD_BACK_SPEED=  LOW_FORWARD_BACK_SPEED;
		KM_LEFT_RIGHT_SPEED = LOW_LEFT_RIGHT_SPEED;
		KeyboardMode=SHIFT_CTRL;
	}
	else if(key->v & KEY_SHIFT)//Shift
	{
		KM_FORWORD_BACK_SPEED=  HIGH_FORWARD_BACK_SPEED;
		KM_LEFT_RIGHT_SPEED = HIGH_LEFT_RIGHT_SPEED;
		KeyboardMode=SHIFT;
	}
	else if(key->v & KEY_CTRL)//Ctrl
	{
		KM_FORWORD_BACK_SPEED=  LOW_FORWARD_BACK_SPEED;
		KM_LEFT_RIGHT_SPEED = LOW_LEFT_RIGHT_SPEED;
		KeyboardMode=CTRL;
	}
	else
	{
		KM_FORWORD_BACK_SPEED=  NORMAL_FORWARD_BACK_SPEED;
		KM_LEFT_RIGHT_SPEED = NORMAL_LEFT_RIGHT_SPEED;
		KeyboardMode=NO_CHANGE;
	}	
}
