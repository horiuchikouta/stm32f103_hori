#ifndef CAN_ENCODER_SEND_HPP
#define CAN_ENCODER_SEND_HPP

#include "can.hpp"
#include "enc.hpp"
#include "system.hpp"
#include "util.hpp"


class CanEncoderSend :public CanClassBase,public Enc{
private:
	Enc *enc;
	int id;
	int Mode;
	int CountMemory;
	bool SetFlag;
	bool OrderFlag;
	uint32_t time;

public:
	u16 frequency;

	int canRead(int id,int length,unsigned char data[8]);
	int canId(int id);
	void cycle();
	enum{
		CAN_ENC_SEN_NULL=0,
		CAN_ENC_SEN_TRANS,
		CAN_ENC_SEN_ORDER,
	};
	int returnFrequency(){
		return frequency;
	}
	int returnMode(){
		return Mode;
	}

	int returnId(){
		return id;
	}

	bool returnOrderFlag(){
		return OrderFlag;
	}

};

#endif
