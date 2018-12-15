#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include "stm32f10x.h"
//#include "main.hpp"

#include "adc.hpp"
#include "can.hpp"
#include "enc.hpp"
#include "gpio.hpp"
#include "pwm.hpp"
#include "system.hpp"
#include "tim.hpp"
#include "usart.hpp"


//can--------------------------------------------------------


//tim--------------------------------------------------------
enum TIM_MODE{
	TIM_MODE_TIM=0,
	TIM_MODE_ENC,
	TIM_MODE_PWM,
};

void setTimInterruptMode(TIM_TypeDef* tim,enum TIM_MODE mode);

//usart--------------------------------------------------------



#endif
