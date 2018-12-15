#ifndef ADC_HPP_
#define ADC_HPP_

#include "stm32f10x.h"

#define ADC_MAX_VALUE 4095

#define ADC_Cycle13 ADC_SampleTime_13Cycles5	//0.96us,11.4k��
#define ADC_Cycle28 ADC_SampleTime_28Cycles5	//2.04us,25.2k��
#define ADC_Cycle41 ADC_SampleTime_41Cycles5	//2.96us,37.2k��
#define ADC_Cycle ADC_Cycle41

class Adc{
public:
	//setup--------------------------
	Adc();
	Adc(GPIO_TypeDef* port,u16 pin,ADC_TypeDef* adc,u8 channel);
	void set(GPIO_TypeDef* port,u16 pin,ADC_TypeDef* adc,u8 channel);
	int setup();

	//read--------------------------
	u16 readValue();
	float readVol();
	float readRatio();

private:
	//mode--------------------------
	u8 mode;	//�N���X�S�̂̏��
	enum{
		ADC_STOP=0,	//��~
		ADC_RUN,	//���쒆
		ADC_SET,	//�ݒ�l��������
	};

	//config//////////////////////////
	GPIO_TypeDef* adcPort;
	u16 adcPin;
	ADC_TypeDef* adcAdc;
	u8 adcChannel;
	u8 adcRank;
	static Adc* gsAdc[16];
	static u16 gsValue[16];
	int configAdc();

};

#endif
