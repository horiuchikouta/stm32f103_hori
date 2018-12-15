#ifndef ENC_HPP_
#define ENC_HPP_

#include "stm32f10x.h"
#include "handler.hpp"

class Enc {
public:
	//setup--------------------------
	Enc();
	Enc(GPIO_TypeDef* port1,u16 pin1,GPIO_TypeDef* port2,u16 pin2,TIM_TypeDef *tim);
	void set(GPIO_TypeDef* port1,u16 pin1,GPIO_TypeDef* port2,u16 pin2,TIM_TypeDef *tim);
	int setup();

	//read--------------------------
	s32 readCount();

	s32 count;


private:
	//mode--------------------------
	u8 mode;	//クラス全体の状態
	enum{
		ENC_STOP=0,	//停止
		ENC_RUN,	//動作中
		ENC_SET,	//設定値がある状態
	};

	//config--------------------------
	GPIO_TypeDef* encPort1;
	u16 encPin1;
	GPIO_TypeDef* encPort2;
	u16 encPin2;
	TIM_TypeDef* encTim;
	bool dir;//回転向き変更用
	//s32 count;
	int configEnc();
};

////////////////////////Interrupt////////////////////////////

void tim1EncInterrupt();
void tim2EncInterrupt();
void tim3EncInterrupt();
void tim4EncInterrupt();


#endif
