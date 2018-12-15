#ifndef CANENC_HPP
#define CANENC_HPP

#include "can.hpp"
#include "enc.hpp"

class CanEncoder:public CanHandler,public Encoder{
private:
	int number;
	int encValue;
	unsigned short frequency;
	long lastGetTimeData;
	int id;
public:
	CanEncoder(Can &can,int number);
	CanEncoder(Can &can,int number,unsigned short frequencyMillisArg);
	int setup();
	int count();
	long lastReadTime();
	int canRead(int id,int number,unsigned char data[8]);
	int canId(int id);
};




#endif//CANENC_HPP
