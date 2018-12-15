#ifndef CANENC_HPP
#define CANENC_HPP

#include "can.hpp"
#include "enc.hpp"

#define CANID_ENC 0x440 //

class CanEnc:public CanClassBase,public Enc{
private:
	int encValue;
	unsigned short frequency;//0 or 1~65535


public:
	//--------------------------------
	CanEnc(Can &can,int idNum);
	CanEnc(Can &can,int idNum,unsigned short frequencyMillisArg);
	int setup();
	int count();
	long lastReadTime();

	//canbase-----------------------------
	//void setCan(Can can,u16 idNum);
	//int setUpCan();
	////can function--------------------------
	int canId(u16 id);
	void canRead(u16 id,u8 length,u8 data[8]);


};




#endif//CANENC_HPP
