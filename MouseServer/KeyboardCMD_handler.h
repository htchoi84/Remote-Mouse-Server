#pragma once

#include "RXCMD.h"

class KeyboardCMD_handler
{
private:
	RXCMD KeyboardRX;  //RXdata 저장할 구조체변수
public:
	KeyboardCMD_handler(void);
	~KeyboardCMD_handler(void);
	void KeyboardCMD_Parsing(RXCMD RX_Data);
};

