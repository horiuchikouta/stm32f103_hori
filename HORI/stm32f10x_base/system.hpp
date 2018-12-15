#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include "stm32f10x.h"
/*
 * システムを管理するクラス
 * ピンのリマップやシステムタイムなどを使える
 */

#define MILLIS 1000
#define MICROS 1000000

//systime -------------------------
int setupSysTime();
u32 millis();
u32 micros();
//Interrupt//////////////////////////
void sysTickTimInterrupt();

//read Frequency--------------------------
u32 readSYSCLKFrequency();
u32 readPCLK1Frequency();
u32 readPCLK2Frequency();


//system function-------------------------
void remapJtag();



#endif
