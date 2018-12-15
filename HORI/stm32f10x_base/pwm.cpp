#include "pwm.hpp"

//setup--------------------------
void Pwm::init(){
	mode 		=PWM_STOP;
	pwmChannel	=0;
	pwmPeriod	=0;//カウントの最大値
	pwmFrequency=0;//周波数
	pwmPrescaler=0;//カウントスケール
	pwmOcMode	=0;
	pwmPulse	=0;//カウントの区切り
	pwmDuty		=0;
}

Pwm::Pwm(){
	init();
}

Pwm::Pwm(GPIO_TypeDef* port,u16 pin,TIM_TypeDef* tim,u16 channel,u16 frequency,u16 ocMode){
	init();
	mode 	= PWM_SET;
	pwmPort = port;
	pwmPin	= pin;
	pwmTim 	= tim;
	pwmChannel 	 = channel;
	pwmFrequency = frequency;
	pwmOcMode 	 = ocMode;
}

void Pwm::set(GPIO_TypeDef* port,u16 pin,TIM_TypeDef* tim,u16 channel,u16 frequency,u16 ocMode){
	mode 	= PWM_SET;
	pwmPort = port;
	pwmPin 	= pin;
	pwmTim	= tim;
	pwmChannel 	 = channel;
	pwmFrequency = frequency;
	pwmOcMode 	 = ocMode;
}

int Pwm::setup(){
	bool i = 0;
	if(mode == PWM_STOP)return 1;	//設定抜けがあったらセットアップを実行しない
	i += configPwm();

	if(!i)mode = PWM_RUN;
	duty(0.0);
	return i;
}

int Pwm::reset(u16 frequency,u16 ocMode){
	pwmFrequency = frequency;
	pwmOcMode = ocMode;
	return setup();
}

//function------------------------------------------------------
void Pwm::duty(float duty){
	if(mode!=PWM_RUN)return ;

	if(duty<0.0)duty = 0.0;
	else if(duty>1.0)duty = 1.0;

	pwmDuty = duty;
	pwmPulse = pwmDuty*pwmPeriod;

	if(pwmPulse > pwmPeriod)pwmPulse = pwmPeriod;
	if(pwmChannel==1)pwmTim->CCR1 = pwmPulse;
	else if(pwmChannel==2)pwmTim->CCR2 = pwmPulse;
	else if(pwmChannel==3)pwmTim->CCR3 = pwmPulse;
	else if(pwmChannel==4)pwmTim->CCR4 = pwmPulse;
}

void Pwm::pulse(u16 pulse){
	if(mode!=PWM_RUN)return ;

	if(pulse<0)pulse = 0;
	else if(pulse>pwmPeriod)pulse = pwmPeriod;

	pwmPulse = pulse;
	pwmDuty = ((float)pulse/(float)pwmPeriod);

	if(pwmChannel==1)pwmTim->CCR1 = pwmPulse;
	else if(pwmChannel==2)pwmTim->CCR2 = pwmPulse;
	else if(pwmChannel==3)pwmTim->CCR3 = pwmPulse;
	else if(pwmChannel==4)pwmTim->CCR4 = pwmPulse;
}

//config--------------------------------------------------
int Pwm::configPwm(){
	bool i=0;

	//Set prescaler and period
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	if(pwmTim==TIM1){
		pwmPrescaler=(uint16_t)(((((RCC_Clocks.PCLK2_Frequency)/pwmFrequency)/0xFFFF)))*2;
		pwmPeriod=(uint16_t)((RCC_Clocks.PCLK2_Frequency)/(pwmFrequency*(pwmPrescaler+1)));
	}else {
		pwmPrescaler=(uint16_t)(((((RCC_Clocks.PCLK1_Frequency*2)/pwmFrequency)/0xFFFF)))*2;
		pwmPeriod=(uint16_t)((RCC_Clocks.PCLK1_Frequency*2)/(pwmFrequency*(pwmPrescaler+1)));
	}

	//gpio
	if(pwmPort==GPIOA)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(pwmPort == GPIOB)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	else if(pwmPort == GPIOC)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	else if(pwmPort == GPIOD)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = pwmPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(pwmPort,&GPIO_InitStructure);

	//tim
	if(pwmTim == TIM1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	else if(pwmTim == TIM2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	else if(pwmTim == TIM3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	else if(pwmTim == TIM4)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = pwmPeriod;
	TimeBaseInitStructure.TIM_Prescaler = pwmPrescaler;
	TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(pwmTim,&TimeBaseInitStructure);

	//oc
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = pwmOcMode;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//duty
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	if(pwmChannel==1)TIM_OC1Init(pwmTim,&TIM_OCInitStructure);
	else if (pwmChannel==2)TIM_OC2Init(pwmTim,&TIM_OCInitStructure);
	else if (pwmChannel==3)TIM_OC3Init(pwmTim,&TIM_OCInitStructure);
	else if (pwmChannel==4)TIM_OC4Init(pwmTim,&TIM_OCInitStructure);

	if(pwmTim==TIM1)TIM_CtrlPWMOutputs(TIM1, ENABLE);//高性能タイマのみの設定

	//関連付け
	//setTimInterruptMode(pwmTim,MODE_PWM);//ハンドラそもそも呼ばないやんけ

	//動作許可
	TIM_ARRPreloadConfig(pwmTim,ENABLE);//自動再リロードの許可
	TIM_Cmd(pwmTim,ENABLE);

	return i;
}

////////////////////////Interrupt////////////////////////////

void tim1PwmInterrupt(){

}

void tim2PwmInterrupt(){

}

void tim3PwmInterrupt(){

}

void tim4PwmInterrupt(){

}
