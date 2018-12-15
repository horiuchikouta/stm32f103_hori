#ifndef CAN_HPP_
#define CAN_HPP_

#include "stm32f10x.h"
#include "util.hpp"

#define FILTER_UPPER_LIMIT 13*4

class CanClassBase;

////////////////////////Can////////////////////////////
class Can{
public:
	//setup--------------------------
	Can();
	Can(GPIO_TypeDef* portTx,u16 pinTx,GPIO_TypeDef* portRx,u16 pinRx,CAN_TypeDef* can,u8 mode=CAN_Mode_Normal);
	void set(GPIO_TypeDef* portTx,u16 pinTx,GPIO_TypeDef* portRx,u16 pinRx,CAN_TypeDef* can,u8 canMode=CAN_Mode_Normal);
	int setup();

	//function--------------------------

	int setId(u16 id);
	int setIdAll();

	int setCanHandler(CanClassBase* HandlerArg);
	int write(int id, int length, unsigned char data[8]);

	//read--------------------------

private:
	//mode--------------------------
	u8 mode;	//クラス全体の状態
	enum{
		CAN_STOP=0,	//停止
		CAN_RUN,	//動作中
		CAN_SET,	//設定値がある状態
	};

	//config--------------------------
	GPIO_TypeDef* canPortTx;
	u16 canPinTx;
	GPIO_TypeDef* canPortRx;
	u16 canPinRx;
	CAN_TypeDef* canCan;
	u8 canMode;
	static u16 filter[FILTER_UPPER_LIMIT];
	static u8 filterNum;
	int configCan();
};

////////////////////////↑Can↓CanClassBase////////////////////////////

class CanClassBase {
private:
public:
	//setup--------------------------
	//virtual void setCan(Can can)=0;
	//virtual int setUpCan()=0;

	//can function--------------------------
	virtual int canId(u16 id)=0;
	virtual void canRead(u16 id,u8 length,u8 data[8])=0;

protected:
	Can* canClass;

	int id; //全体のid
	int idNum; //クラスナンバー
	long lastGetTimeData; //最後に受け取った時間
};
////////////////////////Interrupt////////////////////////////

void canTxInterrupt(CAN_TypeDef* can);
void canRxInterrupt(CAN_TypeDef* can);
void canTransmit(CAN_TypeDef* can,CanTxMsg data);
#endif
