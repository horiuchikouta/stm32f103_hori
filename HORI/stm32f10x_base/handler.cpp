#include "handler.hpp"

///////////////////////Å´ModeFunction/////////////////////////


//can--------------------------------------------------------



//tim--------------------------------------------------------
enum TIM_MODE tim1Mode;
enum TIM_MODE tim2Mode;
enum TIM_MODE tim3Mode;
enum TIM_MODE tim4Mode;

void setTimInterruptMode(TIM_TypeDef* tim,enum TIM_MODE  mode){
	if(tim == TIM1)tim1Mode = mode;
	else if(tim == TIM2)tim2Mode = mode;
	else if(tim == TIM3)tim3Mode = mode;
	else if(tim == TIM4)tim4Mode = mode;
}

//usart--------------------------------------------------------





//////////////////Å™ModeFunctionÅ´Handler/////////////////////////
extern "C" {
/*//canHandler--------------------------------------------------------
void USB_HP_CAN1_TX_IRQHandler(){
	if(CAN_GetITStatus(CAN1,CAN_IT_TME))canTxInterrupt(CAN1);
}
//*/

/*void USB_LP_CAN1_RX0_IRQHandler(){
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0))canRxInterrupt(CAN1);
}*/

//timHandler--------------------------------------------------------
void SysTick_Handler(void){
	sysTickTimInterrupt();
}
//*/

/*//timHandler--------------------------------------------------------
void TIM1_UP_IRQHandler(void){
	if(tim1Mode == MODE_TIM)tim1TimInterrupt();
	else if(tim1Mode == MODE_ENC)tim1EncInterrupt();
	else if(tim1Mode == MODE_PWM)tim1PwmInterrupt();
}

void TIM2_IRQHandler(void){
	if(tim2Mode == MODE_TIM)tim2TimInterrupt();
	else if(tim2Mode == MODE_ENC)tim2EncInterrupt();
	else if(tim2Mode == MODE_PWM)tim2PwmInterrupt();
}

void TIM3_IRQHandler(void){
	if(tim3Mode == MODE_TIM)tim3TimInterrupt();
	else if(tim3Mode == MODE_ENC)tim3EncInterrupt();
	else if(tim3Mode == MODE_PWM)tim3PwmInterrupt();
}

void TIM4_IRQHandler(void){
	if(tim4Mode == MODE_TIM)tim4TimInterrupt();
	else if(tim4Mode == MODE_ENC)tim4EncInterrupt();
	else if(tim4Mode == MODE_PWM)tim4PwmInterrupt();
}
//*/

//usartHandler--------------------------------------------------------
void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_TC) == SET)usart1TxInterrupt();
	else if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)usartRxInterrupt(USART1);
}
void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_TC) == SET)usart2TxInterrupt();
	else if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)usartRxInterrupt(USART2);
}
void USART3_IRQHandler(void) {
	if (USART_GetITStatus(USART3, USART_IT_TC) == SET)usart3TxInterrupt();
	else if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)usartRxInterrupt(USART3);
}
//*/

}

