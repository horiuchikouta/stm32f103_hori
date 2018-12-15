#include "usart.hpp"

//int test=0;
//interrupt--------------------------
static bool isUsart1Sending=0;//送信中は1
static bool isUsart2Sending=0;//送信中は1
static bool isUsart3Sending=0;//送信中は1
static RingBuffer<>* usart1TxBuf;
static RingBuffer<>* usart2TxBuf;
static RingBuffer<>* usart3TxBuf;
static RingBuffer<>* usart1RxBuf;
static RingBuffer<>* usart2RxBuf;
static RingBuffer<>* usart3RxBuf;

//setup--------------------------
void Usart::init(){
	mode	=USART_STOP;
	usartBaudRate = 0;
}

Usart::Usart(){
	init();
}

Usart::Usart(GPIO_TypeDef* portTx,u16 pinTx,GPIO_TypeDef* portRx,u16 pinRx,USART_TypeDef* usart,u32 baudRate){
	init();
	mode 		= USART_SET;
	usartPortTx = portTx;
	usartPinTx 	= pinTx;
	usartPortRx = portRx;
	usartPinRx 	= pinRx;
	usartUsart 	= usart;
	usartBaudRate = baudRate;
}

void Usart::set(GPIO_TypeDef* portTx,u16 pinTx,GPIO_TypeDef* portRx,u16 pinRx,USART_TypeDef* usart,u32 baudRate){
	mode = USART_SET;
	usartPortTx = portTx;
	usartPinTx 	= pinTx;
	usartPortRx = portRx;
	usartPinRx 	= pinRx;
	usartUsart 	= usart;
	usartBaudRate = baudRate;
}

int Usart::setup(){
	bool i=0;
	if(mode==USART_STOP)return 1;	//設定抜けがあったらセットアップを実行しない
	i+=configUsart();

	if(!i)mode=USART_RUN;
	return i;
}

//function--------------------------
void Usart::printf(const char* format, ...) {
	if(mode!=USART_RUN)return;

	char buffer[USART_TX_BUFFER_SIZE];
	va_list arg;
	va_start(arg, format);
	vsprintf(buffer,format,arg);
	for(int i=0;buffer[i]!='\0';i++){
		usartTransmit(usartUsart,buffer[i]);
	}
	va_end(arg);
}

//extra function--------------------------
void pprintf(const char* format, ...) {
	char buffer[USART_TX_BUFFER_SIZE];
	va_list arg;
	va_start(arg, format);
	vsprintf(buffer,format,arg);
	for(int i=0;buffer[i]!='\0';i++){
		usartTransmit(USART2,buffer[i]);
	}
	va_end(arg);
	return;
}//*/

//read--------------------------
char Usart::readChar(){
	if(!rxBuf.isEmpty())return rxBuf.read();
	return 0;
}

char Usart::readPeek(){
	if(!rxBuf.isEmpty())return rxBuf.peek();
	return 0;
}

char* Usart::readStr(){
	int i=0;
	static char string[100];
	while(!rxBuf.isEmpty()&&rxBuf.peek()!='\0'){
		string[i]=rxBuf.read();
		i++;
	}
	return string;
}

//config//////////////////////////
int Usart::configUsart(){
	bool i=0;
	//test++;
	//gpio
	if(usartPortTx == GPIOA||usartPortRx == GPIOA)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(usartPortTx == GPIOB||usartPortRx == GPIOB)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	else if(usartPortTx == GPIOC||usartPortRx == GPIOC)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	else if(usartPortTx == GPIOD||usartPortRx == GPIOD)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	////tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = usartPinTx;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(usartPortTx,&GPIO_InitStructure);
	////rx
	GPIO_InitStructure.GPIO_Pin = usartPinRx;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(usartPortRx,&GPIO_InitStructure);

	//usart
	USART_DeInit(usartUsart);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	if(usartUsart == USART1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	else if(usartUsart == USART2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	else if(usartUsart == USART3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART_InitTypeDef USARTInitStructure;
	USART_StructInit(&USARTInitStructure);
	USARTInitStructure.USART_BaudRate = usartBaudRate;
	USARTInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInitStructure.USART_Parity = USART_Parity_No;
	USARTInitStructure.USART_StopBits = USART_StopBits_1;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(usartUsart,&USARTInitStructure);

	//nvic
	NVIC_InitTypeDef NVIC_InitStructure;
	if(usartUsart == USART1)NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	else if(usartUsart == USART2)NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	else if(usartUsart == USART3)NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//関連付け
	if(usartUsart == USART1){
		usart1TxBuf = &txBuf;
		usart1RxBuf = &rxBuf;
	}else if(usartUsart == USART2){
		usart2TxBuf = &txBuf;
		usart2RxBuf = &rxBuf;
	}else if(usartUsart == USART3){
		usart3TxBuf = &txBuf;
		usart3RxBuf = &rxBuf;
	}
	//許可
	USART_Cmd(usartUsart,ENABLE);
	USART_ITConfig(usartUsart,USART_IT_TC,ENABLE);
	USART_ITConfig(usartUsart,USART_IT_RXNE,ENABLE);

	return i;
}
//Interrupt//////////////////////////
void usart1TxInterrupt(){
	if(!usart1TxBuf->isEmpty()) USART_SendData(USART1,usart1TxBuf->read());
	else isUsart1Sending=0;
	USART_ClearITPendingBit(USART1,USART_IT_TC);
}

void usart2TxInterrupt(){
	if(!usart2TxBuf->isEmpty()) USART_SendData(USART2,usart2TxBuf->read());
	else isUsart2Sending=0;
	USART_ClearITPendingBit(USART2,USART_IT_TC);
}

void usart3TxInterrupt(){
	if(!usart3TxBuf->isEmpty()) USART_SendData(USART3,usart3TxBuf->read());
	else isUsart3Sending=0;
	USART_ClearITPendingBit(USART3,USART_IT_TC);
}

void usartRxInterrupt(USART_TypeDef* usart){
	if(usart==USART1)usart1RxBuf->write(USART_ReceiveData(USART1));
	else if(usart==USART2)usart2RxBuf->write(USART_ReceiveData(USART2));
	else if(usart==USART3)usart3RxBuf->write(USART_ReceiveData(USART3));
}

void usartTransmit(USART_TypeDef* usart,const char str){
	if(usart==USART1){
		if(!isUsart1Sending){
			USART_SendData(USART1, str);
			isUsart1Sending = 1;
		}else usart1TxBuf->write(str);
	}
	else if(usart==USART2){
		if(!isUsart2Sending){
			USART_SendData(USART2, str);
			isUsart2Sending = 1;
		}else usart2TxBuf->write(str);
	}
	else if(usart==USART3){
		if(!isUsart3Sending){
			USART_SendData(USART3, str);
			isUsart3Sending = 1;
		}else usart3TxBuf->write(str);
	}
}
