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
	u8 mode;	//�N���X�S�̂̏��
	enum{
		TIM_STOP=0,	//��~
		TIM_RUN,		//���쒆
		TIM_SET,		//�ݒ�l��������
	};

	//config--------------------------
	TIM_TypeDef* timTim;
	u16 timPeriod;//�J�E���g�̍ő�l
	u16	timPrescaler;//�J�E���g�X�P�[��
	int configTim();
};

void tim1TimInterrupt();

void tim2TimInterrupt();

void tim3TimInterrupt();

void tim4TimInterrupt();

#endif

