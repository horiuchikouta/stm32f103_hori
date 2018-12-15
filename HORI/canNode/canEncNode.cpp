#include "canEncNode.hpp"

CanEncoderSend::CanEncoderSend(){
	Mode=CAN_ENC_SEN_NULL;
	CountMemory=0;
	SetFlag=false;
	OrderFlag=false;
	frequency=65535;
	time=0;
}


CanEncoderSend::CanEncoderSend(Can &can,Enc &enc,int number ){
	//canHandlerSetup(&can);
	//can.setHandler(this);
	this->can=&can;
	this->enc=&enc;
	id=0x440+number;
	Mode=CAN_ENC_SEN_NULL;
	CountMemory=0;
	SetFlag=false;
	OrderFlag=false;
	frequency=65535;
	time=0;
}


int CanEncoderSend::setup(){
	can->setup();
	enc->setup();
	can->setId(id-0x040);
	return 0;
}

int CanEncoderSend::setup(Can &can,Enc &enc,int number){
	this->can=&can;
	this->enc=&enc;
	id=0x440+number;
	Mode=CAN_ENC_SEN_NULL;
	CountMemory=0;
	SetFlag=false;
	OrderFlag=false;
	frequency=65535;

	return setup();
}

int CanEncoderSend::canRead(int id,int length,unsigned char data[8]){
	if(length==1 && data[0]==0){
		SetFlag=true;
		CountMemory=enc->readCount();
	}
	if(length==3 && data[0]==1 && SetFlag==true){
		frequency=(unsigned short)uchar2_to_ushort(data+1);
		if(frequency == 65535){
			Mode=CAN_ENC_SEN_ORDER;
		}
		else Mode=CAN_ENC_SEN_TRANS;
	}
	if(length==0 ){
		OrderFlag=true;
	}
	return 0;
}

int CanEncoderSend::canId(int id){
	//s1.printf("// Id=%x //",id);
	if(Mode != CAN_ENC_SEN_ORDER)return(this->id-0x040 == id);
	else return(this->id == id);
}

void CanEncoderSend::cycle(){
	if(SetFlag == true){
		unsigned char data[4];
		if(millis()-time >= frequency && Mode == CAN_ENC_SEN_TRANS){
			time=millis();
			int_to_uchar4(data,enc->readCount()-CountMemory);
			//int_to_uchar4(data,123);
			can->write(id,4,data);
		}
		else if(OrderFlag == true){
			int_to_uchar4(data,enc->readCount()-CountMemory);
			//int_to_uchar4(data,111);
			can->write(id,4,data);
			OrderFlag = false;
		}
	}
}
