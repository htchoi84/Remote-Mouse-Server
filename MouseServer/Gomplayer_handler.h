#pragma once

#include "RXCMD.h"

class Gomplayer_handler
{
private:
	RXCMD Gomplayer_RX;  //RXdata 저장할 구조체변수
public:
	Gomplayer_handler(void);
	~Gomplayer_handler(void);
	void GomplayerCMD_Parsing(RXCMD RX_Data); //remote-cmd= 헤더로 시작되는 커맨드들 parsing하는 함수
	CString RegReadString(HKEY hKey,LPCTSTR lpsubKey,LPCTSTR keyName);  //레지스트리 값 read
	void POWERCMD_handler();  //remote-cmd=POWER 커맨드 핸들러
	void PLAYCMD_handler();  //remote-cmd=PLAY 커맨드 핸들러
	void OKCMD_handler();  //remote-cmd=OK 커맨드 핸들러
	void UPCMD_handler();  //remote-cmd=UP 커맨드 핸들러
	void DOWNCMD_handler();  //remote-cmd=DOWN 커맨드 핸들러
	void RIGHTCMD_handler();  //remote-cmd=RIGHT 커맨드 핸들러
	void LEFTCMD_handler();  //remote-cmd=LEFT 커맨드 핸들러
	void STOPCMD_handler();  //remote-cmd=STOP 커맨드 핸들러
	void FFCMD_handler();  //remote-cmd=FF 커맨드 핸들러
	void RWCMD_handler();  //remote-cmd=RW 커맨드 핸들러
	void VOLUMEUP_CMD_handler();  //remote-cmd=VOLUME-UP 커맨드 핸들러
	void VOLUMEDOWN_CMD_handler();  //remote-cmd=VOLUME-DOWN 커맨드 핸들러
	void OPENCMD_handler();
	void MUTECMD_handler();
	void CANCELCMD_handler();
	void TABCMD_handler();
};

