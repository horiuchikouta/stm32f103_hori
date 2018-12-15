#include "adc.hpp"
#include "can.hpp"
#include "enc.hpp"
#include "gpio.hpp"
#include "handler.hpp"
#include "pwm.hpp"
#include "system.hpp"
#include "tim.hpp"
#include "usart.hpp"

Gpio led0;
Pwm pwm0;
Usart s;

void setup(){
	setupSysTime();

	s.set(PB10,PB11,USART3);
	s.setup();



	led0.set(PB12,OUTPUT);
	led0.setup();
	pwm0.set(PB13,TIM3,2,2000);
	pwm0.setup();

}
