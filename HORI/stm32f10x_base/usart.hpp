#ifndef USART_HPP_
#define USART_HPP_

#include <stdarg.h>
#include <stdio.h>

#include "stm32f10x.h"
#include "util.hpp"

#define USART_DEFAULT_BAUD 115200
#define USART_RX_BUFFER_SIZE 256
#define USART_TX_BUFFER_SIZE 256

class Usart{
public:
	//setup--------------------------
	void init();
	Usart();
	Usart(GPIO_TypeDef* portTx,u16 pinTx,GPIO_TypeDef* portRx,u16 pinRx,USART_TypeDef* usart,u32 baudRate);
	void set(GPIO_TypeDef* portTx,u16 pinTx,GPIO_TypeDef* portRx,u16 pinRx,USART_TypeDef* usart,u32 baudRate=9600);
	int setup();

	//function--------------------------
	void printf(const char *format, ...);

	//read--------------------------
	char readChar();
	char readPeek();

	USART_TypeDef* readUsart(){return usartUsart;};

	char* readStr();


//private:
	//mode--------------------------
	u8 mode;	//クラス全体の状態
	enum{
		USART_STOP=0,	//停止
		USART_RUN,		//動作中
		USART_SET,		//設定値がある状態
	};

	//config//////////////////////////
	GPIO_TypeDef* usartPortTx;
	u16 usartPinTx;
	GPIO_TypeDef* usartPortRx;
	u16 usartPinRx;
	USART_TypeDef* usartUsart;
	u32 usartBaudRate;

	RingBuffer<> rxBuf;
	RingBuffer<> txBuf;

	int configUsart();
};
//extra function--------------------------
void pprintf(const char* format, ...) ;

//Interrupt//////////////////////////
void usart1TxInterrupt();
void usart2TxInterrupt();
void usart3TxInterrupt();
void usartRxInterrupt(USART_TypeDef *usart);
void usartTransmit(USART_TypeDef *usart,const char str);




#endif
