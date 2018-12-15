#include "tim.hpp"

//setup--------------------------
Tim::Tim(){
}

Tim::Tim(TIM_TypeDef* tim,u16 period,u16 prescaler){
	mode = TIM_SET;
	timTim = tim;
	timPeriod = period;
	timPrescaler = prescaler;
}

void Tim::set(TIM_TypeDef* tim,u16 period,u16 prescaler){
	mode = TIM_SET;
	timTim = tim;
	timPeriod = period;
	timPrescaler = prescaler;
}

int Tim::setup(){
	bool i = 0;
	if(mode == TIM_STOP)return 1;	//設定抜けがあったらセットアップを実行しない
	i += configTim();

	if(!i)mode = TIM_RUN;
	return i;
}


//config--------------------------
int Tim::configTim(){
	bool i=0;

	//tim
	if(timTim == TIM1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	else if(timTim == TIM2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	else if(timTim == TIM3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	else if(timTim == TIM4)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = timPeriod;
	TimeBaseInitStructure.TIM_Prescaler = timPrescaler;
	TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(timTim,&TimeBaseInitStructure);

	//nvic
	NVIC_InitTypeDef NVIC_InitStructure;
	if(timTim == TIM1)NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	else if(timTim == TIM2)NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	else if(timTim == TIM3)NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	else if(timTim == TIM4)NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(timTim,TIM_IT_Update,ENABLE);

	//関連付け
	setTimInterruptMode(timTim,TIM_MODE_TIM);

	TIM_ARRPreloadConfig(timTim,ENABLE);//自動再リロードの許可
	TIM_Cmd(timTim,ENABLE);

	return i;
}

////////////////////////Interrupt////////////////////////////

void tim1TimInterrupt(){

}
void tim2TimInterrupt(){

}
void tim3TimInterrupt(){

}
void tim4TimInterrupt(){

}


