#include "enc.hpp"
#include "usart.hpp"

//interrupt--------------------------
static s32* tim1Cnt;
static s32* tim2Cnt;
static s32* tim3Cnt;
static s32* tim4Cnt;

//setup--------------------------
Enc::Enc(){
	mode=ENC_STOP;
}

Enc::Enc(GPIO_TypeDef* port1,u16 pin1,GPIO_TypeDef* port2,u16 pin2,TIM_TypeDef *tim){
	mode=ENC_SET;
	encPort1=port1;
	encPin1=pin1;
	encPort2=port2;
	encPin2=pin2;
	encTim=tim;
}

void Enc::set(GPIO_TypeDef* port1,u16 pin1,GPIO_TypeDef* port2,u16 pin2,TIM_TypeDef *tim){
	mode=ENC_SET;
	encPort1=port1;
	encPin1=pin1;
	encPort2=port2;
	encPin2=pin2;
	encTim=tim;
}

int Enc::setup(){
	bool i = 0;
	if(mode == ENC_STOP)return 1;	//設定抜けがあったらセットアップを実行しない
	i += configEnc();

	if(!i)mode = ENC_RUN;
	return i;
}

void Enc::direction(){
	if(dir=1)dir=0;
	else dir=1;
}


//read--------------------------
s32 Enc::readCount(){
	if(mode==ENC_RUN) return (encTim->CNT+count*65536);
	else return 0;
}

//config--------------------------
int Enc::configEnc(){
	bool i=0;

	//gpio
	if(encPort1 == GPIOA||encPort2 == GPIOA)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(encPort1 == GPIOB||encPort2 == GPIOB)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	else if(encPort1 == GPIOC||encPort2 == GPIOC)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	else if(encPort1 == GPIOD||encPort2 == GPIOD)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	////pin1
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = encPin1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(encPort1,&GPIO_InitStructure);
	////pin2
	GPIO_InitStructure.GPIO_Pin = encPin2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(encPort2,&GPIO_InitStructure);

	//tim
	if(encTim == TIM1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	else if(encTim == TIM2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	else if(encTim == TIM3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	else if(encTim == TIM4)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	//nvic
	NVIC_InitTypeDef NVIC_InitStructure;
	if(encTim == TIM1)NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	else if(encTim == TIM2)NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	else if(encTim == TIM3)NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	else if(encTim == TIM4)NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(encTim,TIM_IT_Update,ENABLE);

	//enc
	TIM_EncoderInterfaceConfig(encTim,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

	//関連付け
	if(encTim == TIM1)tim1Cnt = &count;
	else if(encTim == TIM2)tim2Cnt = &count;
	else if(encTim == TIM3)tim3Cnt = &count;
	else if(encTim == TIM4)tim4Cnt = &count;

	setTimInterruptMode(encTim,TIM_MODE_ENC);

	//動作許可
	TIM_Cmd(encTim,ENABLE);

	return i;
}

//////////////////////Interrupt//////////////////////////
void tim1EncInterrupt(){
	if(TIM1->CNT < 32768)(*tim1Cnt)++;
	else (*tim1Cnt)--;
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}

void tim2EncInterrupt(){
	if(TIM2->CNT < 32768)(*tim2Cnt)++;
	else (*tim2Cnt)--;
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

void tim3EncInterrupt(){
	if(TIM3->CNT < 32768)(*tim3Cnt)++;
	else (*tim3Cnt)--;
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void tim4EncInterrupt(){
	if(TIM4->CNT < 32768)(*tim4Cnt)++;
	else (*tim4Cnt)--;
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

