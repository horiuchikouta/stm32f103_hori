#include "canEnc.hpp"
#include <string.h>
#include <stdio.h>
#include "util.hpp"
#include "system.hpp"


CanEnc::CanEnc(Can &can,int idNum){
	canClass = &can;
	id=CANID_ENC+idNum;
	lastGetTimeData=millis();
	frequency=0;
	canClass.setCanHandler(this);
}

CanEnc::CanEncoder(Can &can,int number,unsigned short frequencyArg){
	canClass = &can;

	id=CANID_ENC+number;
	lastGetTimeData=millis();
	frequency=frequencyArg;
	canClass.setCanHandler(this);
}

int CanEnc::setup(){
	if(canClass->setup()) return 1;
	if(setup()) return 1;
	unsigned char data[8];
	canSetId(id);
	data[0]=0;
	canWrite(id-0x040,1,data);
	data[0]=1;
	ushort_to_uchar2(data+1,frequency);
	canWrite(id-0x040,3,data);
	//encValue=0;
	lastGetTimeData = millis();
	return 0;
}

int CanEnc::count(){
	/*if(frequency == 0){
		unsigned char data[8];
		long lastTime = lastGetTimeData;
		canWrite(id,0,data);
		while(lastTime==lastGetTimeData);
	}*/
	if(dir)return -encValue;
	else return encValue;
}

long CanEnc::lastReadTime(){
	return lastGetTimeData;
}

//--------------------------------------------
/*void CanEnc::setCan(Can can,u16 idNum){
	canCan=can;
}*/

////
void CanEnc::canRead(int id,int number,unsigned char data[8]){
	encValue=uchar4_to_int(data);
	lastGetTimeData=millis();
	return 0;
}

int CanEnc::canId(int id){
	return id == this->id;
}

