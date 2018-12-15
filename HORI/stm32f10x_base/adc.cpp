#include "adc.hpp"


//config--------------------------
u16 Adc::gsValue[16] = {0};
Adc* Adc::gsAdc[16] = {0};
static u8 oRank=0;

//setup--------------------------
Adc::Adc(){
	mode=ADC_STOP;
}

Adc::Adc(GPIO_TypeDef* port,u16 pin,ADC_TypeDef* adc,u8 channel){
	mode=ADC_SET;
	adcPort = port;
	adcPin = pin;
	adcAdc = adc;
	adcChannel = channel;
	adcRank = ++oRank;
}

void Adc::set(GPIO_TypeDef* port,u16 pin,ADC_TypeDef* adc,u8 channel){
	mode=ADC_SET;
	adcPort = port;
	adcPin = pin;
	adcAdc = adc;
	adcChannel = channel;
	adcRank = ++oRank;
}

int Adc::setup(){
	bool i=0;
	if(mode!=ADC_SET)return 1;	//設定抜けがあったらセットアップを実行しない
	i+=configAdc();

	if(!i)mode=ADC_RUN;
	return i;
}

//read--------------------------
u16 Adc::readValue(){
	return gsValue[adcRank-1];//
}
float Adc::readVol(){
	return (float)((float)gsValue[adcRank-1]*3.3/ADC_MAX_VALUE);
}
float Adc::readRatio(){
	return (float)(gsValue[adcRank-1]/ADC_MAX_VALUE);
}


//config//////////////////////////
int Adc::configAdc(){
	bool i=0;
	gsAdc[adcRank-1]=this;

	//gpio
	if(adcPort==GPIOA)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(adcPort == GPIOB)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	else if(adcPort == GPIOC)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	else if(adcPort == GPIOD)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = adcPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(adcPort,&GPIO_InitStructure);

	//adc
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
	if(adcAdc == ADC1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	else if(adcAdc == ADC2)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
	////adc_config
	ADC_DeInit(adcAdc);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_NbrOfChannel=adcRank;
	ADC_Init(adcAdc,&ADC_InitStructure);

	////adc許可
	ADC_Cmd(adcAdc,ENABLE);

	////adcキャリブレーション
	ADC_ResetCalibration(adcAdc);
	while(ADC_GetResetCalibrationStatus(adcAdc));
	ADC_StartCalibration(adcAdc);
	while(ADC_GetCalibrationStatus(adcAdc));

	for(int j=0;j<adcRank;j++){ADC_RegularChannelConfig(gsAdc[j]->adcAdc,gsAdc[j]->adcChannel,gsAdc[j]->adcRank,ADC_Cycle);}

	//dma
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//
	DMA_DeInit(DMA1_Channel1);
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&adcAdc->DR;//&ADC1->DR;				//DMAy Channelx用周辺回路のベースアドレスを定義する//ここではADC１のDRレジスタを入れている
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)&gsValue;					//DMAy Channelxのメモリ側ベースアドレスを定義する
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC;			//データ転送方向
	DMA_InitStructure.DMA_BufferSize 			= adcRank;	////							//バッファサイズ
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;		//ペリフェラル側のアドレスを転送毎にインクリメントするかどうか
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;				//メモリ側のアドレスを転送毎にインクリメントするかどうか
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord;	//ペリフェラル側のデータサイズ
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_HalfWord;		//メモリ側のデータサイズ
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;				//DMAの動作モードを指定
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High;				//DMA転送のソフトウェア優先度
	DMA_InitStructure.DMA_M2M					= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//DMAの設定

	//dma許可
	DMA_Cmd(DMA1_Channel1,ENABLE);//

	//動作許可
	ADC_DMACmd(adcAdc,ENABLE);//
	ADC_SoftwareStartConvCmd(adcAdc,ENABLE);	//ADC変換開始


	return i;
	//}

}
