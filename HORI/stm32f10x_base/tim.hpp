#ifndef TIM_HPP_
#define TIM_HPP_

#include "stm32f10x.h"
#include "handler.hpp"

class Tim {
public:
	//setup--------------------------
	Tim();
	Tim(TIM_TypeDef* tim,u16 period,u16 prescaler);
	void set(TIM_TypeDef* tim,u16 period,u16 prescaler);
	int setup();

private:
	//mode--------------------------
	u8 mode;	//クラス全体の状態
	enum{
		TIM_STOP=0,	//停止
		TIM_RUN,		//動作中
		TIM_SET,		//設定値がある状態
	};

	//config--------------------------
	TIM_TypeDef* timTim;
	u16 timPeriod;//カウントの最大値
	u16	timPrescaler;//カウントスケール
	int configTim();
};

void tim1TimInterrupt();

void tim2TimInterrupt();

void tim3TimInterrupt();

void tim4TimInterrupt();

#endif

