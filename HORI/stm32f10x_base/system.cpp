#include "system.hpp"

//interrupt--------------------------
static u32 systictime = 0;

//frequency--------------------------
static u32 SYSCLKFreq = 0;
static u32 HCLKFreq = 0;
static u32 PCLK1Freq = 0;
static u32 PCLK2Freq = 0;
static u32 ADCCLKFreq = 0;

///////////////////////Å´SysTim///////////////////////////

//systime -------------------------
int setupSysTime(){
	bool i=0;

	//SysTickTime
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency/MICROS);
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

	//é¸îgêîÇÃéÊìæ
	SYSCLKFreq = RCC_Clocks.SYSCLK_Frequency;
	HCLKFreq = RCC_Clocks.HCLK_Frequency;
	PCLK1Freq = RCC_Clocks.PCLK1_Frequency;
	PCLK2Freq = RCC_Clocks.PCLK2_Frequency;
	ADCCLKFreq = RCC_Clocks.ADCCLK_Frequency;

	return i;
}

u32 millis(){
	return systictime/MILLIS;
}

u32 micros(){
	return systictime;
}

//Interrupt//////////////////////////
void sysTickTimInterrupt(){
	systictime++;
}

//////////////////////Å™SysTim/////////////////////////////

//read Frequency--------------------------
u32 readSYSCLKFrequency(){
	return SYSCLKFreq;
}
u32 readPCLK1Frequency(){
	return PCLK1Freq;
}
u32 readPCLK2Frequency(){
	return PCLK2Freq;
}

//system function--------------------------

void remapJtag(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}


