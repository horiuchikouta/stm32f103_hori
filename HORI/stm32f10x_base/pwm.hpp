#ifndef PWM_HPP_
#define PWM_HPP_

#include "stm32f10x.h"
#include "handler.hpp"

#define PWM_PERIOD_MAX 0xFFFF
#define PWM_PERIOD_MIN 0x0000

class Pwm {
public:
	//setup--------------------------
	void init();
	Pwm();
	Pwm(GPIO_TypeDef* port,u16 pin,TIM_TypeDef* tim,u16 channel,u16 frequency = 36000,u16 ocMode=TIM_OCMode_PWM1);
	void set(GPIO_TypeDef* port,u16 pin,TIM_TypeDef* tim,u16 channel,u16 frequency = 36000,u16 ocMode=TIM_OCMode_PWM1);
	int setup();
	int reset(u16 frequency = 36000,u16 ocMode=TIM_OCMode_PWM1);

	//function--------------------------
	void duty(float duty);//0.0~1.0�܂ł�duty��ݒ�
	void pulse(u16 pulse);//0~pwmPeriod�܂ł�duty��ݒ�


	//read--------------------------
	float readDuty(){return pwmDuty;}
	u16 readPulse(){return pwmPulse;}
	u16 readPeriod(){return pwmPeriod;}
	u16 readFrequency(){return pwmFrequency;}
	u16 readPrescaler(){return pwmPrescaler;}
	u16 readOcMode(){return pwmOcMode;}

private:
	//mode--------------------------
	u8 mode;	//�N���X�S�̂̏��
	enum{
		PWM_STOP=0,	//��~
		PWM_RUN,	//���쒆
		PWM_SET,	//�ݒ�l��������
	};

	//config--------------------------
	GPIO_TypeDef* pwmPort;
	u16 pwmPin;
	TIM_TypeDef* pwmTim;
	u16 pwmChannel;
	u16 pwmPeriod;//�J�E���g�̍ő�l
	u16 pwmFrequency;//���g��
	u16 pwmPrescaler;//�J�E���g�X�P�[��
	u16 pwmOcMode;
	u16 pwmPulse;//�J�E���g�̋�؂�
	float pwmDuty;
	int configPwm();
};

////////////////////////Interrupt////////////////////////////

void tim1PwmInterrupt();
void tim2PwmInterrupt();
void tim3PwmInterrupt();
void tim4PwmInterrupt();

#endif
