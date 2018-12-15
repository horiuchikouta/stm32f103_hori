#ifndef GPIO_HPP_
#define GPIO_HPP_

#include "stm32f10x.h"

//Pin-------------------------
#define PA0 GPIOA,GPIO_Pin_0
#define PA1 GPIOA,GPIO_Pin_1
#define PA2 GPIOA,GPIO_Pin_2
#define PA3 GPIOA,GPIO_Pin_3
#define PA4 GPIOA,GPIO_Pin_4
#define PA5 GPIOA,GPIO_Pin_5
#define PA6 GPIOA,GPIO_Pin_6
#define PA7 GPIOA,GPIO_Pin_7

#define PA8 GPIOA,GPIO_Pin_8
#define PA9 GPIOA,GPIO_Pin_9
#define PA10 GPIOA,GPIO_Pin_10
#define PA11 GPIOA,GPIO_Pin_11
#define PA12 GPIOA,GPIO_Pin_12
#define PA13 GPIOA,GPIO_Pin_13
#define PA14 GPIOA,GPIO_Pin_14
#define PA15 GPIOA,GPIO_Pin_15

#define PB0 GPIOB,GPIO_Pin_0
#define PB1 GPIOB,GPIO_Pin_1
#define PB2 GPIOB,GPIO_Pin_2
#define PB3 GPIOB,GPIO_Pin_3
#define PB4 GPIOB,GPIO_Pin_4
#define PB5 GPIOB,GPIO_Pin_5
#define PB6 GPIOB,GPIO_Pin_6
#define PB7 GPIOB,GPIO_Pin_7

#define PB8 GPIOB,GPIO_Pin_8
#define PB9 GPIOB,GPIO_Pin_9
#define PB10 GPIOB,GPIO_Pin_10
#define PB11 GPIOB,GPIO_Pin_11
#define PB12 GPIOB,GPIO_Pin_12
#define PB13 GPIOB,GPIO_Pin_13
#define PB14 GPIOB,GPIO_Pin_14
#define PB15 GPIOB,GPIO_Pin_15

#define PC13 GPIOC,GPIO_Pin_13
#define PC14 GPIOC,GPIO_Pin_14
#define PC15 GPIOC,GPIO_Pin_15

#define PD0 GPIOD,GPIO_Pin_0
#define PD1 GPIOD,GPIO_Pin_1
#define PD2 GPIOD,GPIO_Pin_2
#define PD3 GPIOD,GPIO_Pin_3

//Mode-----------------------------

#define OUTPUT GPIO_Mode_Out_PP
#define OUTPUT_OD GPIO_Mode_Out_OD
#define OUTPUT_AF GPIO_Mode_AF_PP
#define OUTPUT_AF_OD GPIO_Mode_AF_OD

#define INPUT GPIO_Mode_IN_FLOATING
#define INPUT_AIN GPIO_Mode_AIN
#define INPUT_PU GPIO_Mode_IPU
#define INPUT_PD GPIO_Mode_IPD

///////////////////////↓Gpio////////////////////////
class Gpio {
public:
	//setup--------------------------
	void init();
	Gpio();
	Gpio(GPIO_TypeDef* port,u16 pin,GPIOMode_TypeDef mode);
	void set(GPIO_TypeDef* port,u16 pin,GPIOMode_TypeDef mode);
	int setup();

	//function--------------------------
	void write(bool condition);
	void toggle();
	void gpioHigh();
	void gpioLow();

	//read--------------------------
	u8 readState();
	u8 readMode(){return mode;};

private:
	//mode--------------------------
	u8 mode;	//クラス全体の状態
	enum{
		GPIO_STOP=0,	//停止
		GPIO_RUN,		//動作中
		GPIO_SET,		//設定値がある状態
	};

	//function--------------------------
	bool state;	//ピンの状態

	//config//////////////////////////
	GPIO_TypeDef* gpioPort;
	u16 gpioPin;
	GPIOMode_TypeDef gpioMode;
	int configGpio();

};

/////////////////////↑Gpio↓GpioNull///////////////////
class GpioNull:public Gpio {
public:

private:

};
///////////////////////↑Gpio/////////////////////////


#endif
