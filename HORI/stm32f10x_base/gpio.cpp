#include "gpio.hpp"
//setup--------------------------
void Gpio::init(){
	mode	=GPIO_STOP;
	state	=0;
}

Gpio::Gpio(){
	init();
}

Gpio::Gpio(GPIO_TypeDef* port,u16 pin,GPIOMode_TypeDef mode){
	init();
	this->mode	=GPIO_SET;
	gpioPort	=port;
	gpioPin		=pin;
	gpioMode	=mode;
}

void Gpio::set(GPIO_TypeDef* port,u16 pin,GPIOMode_TypeDef mode){
	this->mode	=GPIO_SET;
	gpioPort	=port;
	gpioPin		=pin;
	gpioMode	=mode;
}

int Gpio::setup(){
	bool i=0;
	if(mode==GPIO_STOP)return 1;	//設定抜けがあったらセットアップを実行しない
	i+=configGpio();

	if(!i)mode=GPIO_RUN;
	return i;
}

//function--------------------------

void Gpio::write(bool condition){
	if(mode==GPIO_RUN){
		condition?gpioHigh():gpioLow();
	}
}

void Gpio::toggle(){
	if(mode==GPIO_RUN){
		readState()?gpioLow():gpioHigh();
	}
}

void Gpio::gpioHigh(){
	if(mode==GPIO_RUN){
		GPIO_WriteBit(gpioPort,gpioPin,Bit_SET);
	}
}

void Gpio::gpioLow(){
	if(mode==GPIO_RUN){
		GPIO_WriteBit(gpioPort,gpioPin,Bit_RESET);
	}

}
//read--------------------------

u8 Gpio::readState(){
	if(mode!=GPIO_RUN)return 0;
	state=GPIO_ReadInputDataBit(gpioPort,gpioPin);
	return state;
}

//config//////////////////////////

int Gpio::configGpio(){
	bool i=0;

	if(gpioPort==GPIOA)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(gpioPort == GPIOB)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	else if(gpioPort == GPIOC)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	else if(gpioPort == GPIOD)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = gpioPin;
	GPIO_InitStructure.GPIO_Mode = gpioMode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpioPort,&GPIO_InitStructure);

	return i;
}


