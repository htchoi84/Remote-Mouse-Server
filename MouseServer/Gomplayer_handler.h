#pragma once

#include "RXCMD.h"

class Gomplayer_handler
{
private:
	RXCMD Gomplayer_RX;  //RXdata ������ ����ü����
public:
	Gomplayer_handler(void);
	~Gomplayer_handler(void);
	void GomplayerCMD_Parsing(RXCMD RX_Data); //remote-cmd= ����� ���۵Ǵ� Ŀ�ǵ�� parsing�ϴ� �Լ�
	CString RegReadString(HKEY hKey,LPCTSTR lpsubKey,LPCTSTR keyName);  //������Ʈ�� �� read
	void POWERCMD_handler();  //remote-cmd=POWER Ŀ�ǵ� �ڵ鷯
	void PLAYCMD_handler();  //remote-cmd=PLAY Ŀ�ǵ� �ڵ鷯
	void OKCMD_handler();  //remote-cmd=OK Ŀ�ǵ� �ڵ鷯
	void UPCMD_handler();  //remote-cmd=UP Ŀ�ǵ� �ڵ鷯
	void DOWNCMD_handler();  //remote-cmd=DOWN Ŀ�ǵ� �ڵ鷯
	void RIGHTCMD_handler();  //remote-cmd=RIGHT Ŀ�ǵ� �ڵ鷯
	void LEFTCMD_handler();  //remote-cmd=LEFT Ŀ�ǵ� �ڵ鷯
	void STOPCMD_handler();  //remote-cmd=STOP Ŀ�ǵ� �ڵ鷯
	void FFCMD_handler();  //remote-cmd=FF Ŀ�ǵ� �ڵ鷯
	void RWCMD_handler();  //remote-cmd=RW Ŀ�ǵ� �ڵ鷯
	void VOLUMEUP_CMD_handler();  //remote-cmd=VOLUME-UP Ŀ�ǵ� �ڵ鷯
	void VOLUMEDOWN_CMD_handler();  //remote-cmd=VOLUME-DOWN Ŀ�ǵ� �ڵ鷯
	void OPENCMD_handler();
	void MUTECMD_handler();
	void CANCELCMD_handler();
	void TABCMD_handler();
};

