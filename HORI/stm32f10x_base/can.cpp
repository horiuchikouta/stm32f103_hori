#include "can.hpp"
#include "usart.hpp"
//filter--------------------------
u16 Can::filter[FILTER_UPPER_LIMIT] = {0};
u8 Can::filterNum = 0;

//handler--------------------------
static CanClassBase* canHandler[FILTER_UPPER_LIMIT] = {0};
static u8 handlerNum = 0;

//interrupt--------------------------
static CanRxMsg rxMessage;
static RingBuffer<CanTxMsg,256> can1TxMsger;
static bool isTransmitting = 0 ;

////////////////////////Can////////////////////////////
//setup--------------------------
Can::Can(){
	mode=CAN_STOP;
}

Can::Can(GPIO_TypeDef* portTx,u16 pinTx,GPIO_TypeDef* portRx,u16 pinRx,CAN_TypeDef* can,u8 mode){
	bool i = 0;
	this->mode = CAN_SET;
	canPortTx = portTx;
	canPinTx = pinTx;
	canPortRx = portRx;
	canPinRx = pinRx;
	canCan = can;
	canMode = mode;

	i += configCan();
	if(!i)this->mode = CAN_RUN;
}

void Can::set(GPIO_TypeDef* portTx,u16 pinTx,GPIO_TypeDef* portRx,u16 pinRx,CAN_TypeDef* can,u8 mode){
	this->mode = CAN_SET;
	canPortTx = portTx;
	canPinTx = pinTx;
	canPortRx = portRx;
	canPinRx = pinRx;
	canCan = can;
	canMode = mode;
}

int Can::setup(){
	bool i = 0;
	if(mode == CAN_STOP)return 1;	//設定抜けがあったらセットアップを実行しない
	i += configCan();

	if(!i)mode = CAN_RUN;
	return i;
}

//function--------------------------
int Can::setId(u16 id){
	filter[filterNum] = (u16)id;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterIdHigh	 = (filter[4*(filterNum/4)+0]<<5);
	CAN_FilterInitStructure.CAN_FilterIdLow		 = (filter[4*(filterNum/4)+1]<<5);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (filter[4*(filterNum/4)+2]<<5);
	CAN_FilterInitStructure.CAN_FilterMaskIdLow	 = (filter[4*(filterNum/4)+3]<<5);
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;//(can==CAN1)?CAN_FIFO0:CAN_FIFO1;
	CAN_FilterInitStructure.CAN_FilterNumber = (filterNum/4)+1;
	CAN_FilterInitStructure.CAN_FilterMode	= CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale		 = CAN_FilterScale_16bit;//16bit
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	filterNum++;
	return 0;
}

int Can::setIdAll(){
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterIdHigh	 = 0;
	CAN_FilterInitStructure.CAN_FilterIdLow		 = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow	 = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;//(can==CAN1)?CAN_FIFO0:CAN_FIFO1;
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode	= CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale		 = CAN_FilterScale_16bit;//16bit
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	return 0;
}

int Can::setCanHandler(CanClassBase* HandlerArg){
	if (handlerNum >= FILTER_UPPER_LIMIT) return 1;
	canHandler[handlerNum] = HandlerArg;
	handlerNum++;
	return 0;
}

int Can::write(int id, int length, unsigned char data[8]) {
	CanTxMsg canTxFrame;
	canTxFrame.StdId = id;
	canTxFrame.ExtId = 0;
	canTxFrame.IDE = CAN_ID_STD;
	if (length == 0)canTxFrame.RTR = CAN_RTR_REMOTE;
	else canTxFrame.RTR = CAN_RTR_DATA;
	canTxFrame.DLC = (u8)length;
	canTxFrame.Data[0] = data[0];
	canTxFrame.Data[1] = data[1];
	canTxFrame.Data[2] = data[2];
	canTxFrame.Data[3] = data[3];
	canTxFrame.Data[4] = data[4];
	canTxFrame.Data[5] = data[5];
	canTxFrame.Data[6] = data[6];
	canTxFrame.Data[7] = data[7];
	canTransmit(canCan,canTxFrame);
	return 0;
}

//config--------------------------
int Can::configCan(){
	bool i=0;

	//gpio
	if(canPortTx == GPIOA||canPortRx == GPIOA)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(canPortTx == GPIOB||canPortRx == GPIOB)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	else if(canPortTx == GPIOC||canPortRx == GPIOC)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	else if(canPortTx == GPIOD||canPortRx == GPIOD)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	////tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = canPinTx;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(canPortTx,&GPIO_InitStructure);
	////rx
	GPIO_InitStructure.GPIO_Pin = canPinRx;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(canPortRx,&GPIO_InitStructure);

	//can
	CAN_DeInit(canCan);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	CAN_InitTypeDef CAN_InitStructure;
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = ENABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = canMode;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
	CAN_InitStructure.CAN_Prescaler = 3;
	CAN_Init(canCan,&CAN_InitStructure);

	//nvic
	NVIC_InitTypeDef NVIC_InitStructure;
	if(canCan == CAN1)NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if(canCan == CAN1)NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	CAN_ITConfig(canCan,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(canCan,CAN_IT_TME,ENABLE);

	return i;
}

////////////////////////↑Can↓CanHandler////////////////////////////


/*int CanClassBase::setUpCan(){
	can->setup();
	can->setId(id);
}*/

////////////////////////Interrupt////////////////////////////

extern "C"{
void USB_HP_CAN1_TX_IRQHandler(){
	if(CAN_GetITStatus(CAN1,CAN_IT_TME))canTxInterrupt(CAN1);
}
void USB_LP_CAN1_RX0_IRQHandler(void){
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0))canRxInterrupt(CAN1);
}

}

void canTxInterrupt(CAN_TypeDef* can){
	if(!can1TxMsger.isEmpty())CAN_Transmit(CAN1,&can1TxMsger.read());
	else isTransmitting=0;
	CAN_ClearITPendingBit(can,CAN_IT_TME);
}

void canRxInterrupt(CAN_TypeDef* can){
	if(can==CAN1)CAN_Receive(can, CAN_FIFO1, &rxMessage);
	for(int j=0;j<=handlerNum;j++ ){
		if(canHandler[j]->canId((int)rxMessage.StdId)){
			canHandler[j]->canRead((int)rxMessage.StdId, (int)rxMessage.DLC, rxMessage.Data);
		}
	}
}

void canTransmit(CAN_TypeDef* can,CanTxMsg data){
	if(!isTransmitting){
		CAN_Transmit(can, &data);
		isTransmitting = 1;
	}
	else {
		if(can == CAN1)can1TxMsger.write(data);
	}
}

