#include "stdafx.h"
#include "KeyboardCMD_handler.h"


KeyboardCMD_handler::KeyboardCMD_handler(void)
{
}


KeyboardCMD_handler::~KeyboardCMD_handler(void)
{
}

void KeyboardCMD_handler::KeyboardCMD_Parsing(RXCMD RX_Data) 
{
	KeyboardRX = RX_Data;  //RX커맨드 저장 

	if(KeyboardRX.data1 == VK_HANGUL)  //한영 key
	{
		if ((GetKeyState(VK_CAPITAL) & 0x01) == 0x01)  //캡스락 key가 눌려져있는 상태
		{
			return;
		}
		else
		{
			if(KeyboardRX.action == DOWN)
			{
				keybd_event(VK_HANGUL,0,0,0); 
			}
			else if(KeyboardRX.action == UP)
			{
				keybd_event(VK_HANGUL,0,KEYEVENTF_KEYUP,0);
			}
			else{}
		}
	}
	else if(KeyboardRX.data1 == VK_CAPITAL) //CapsLock key
	{
		if(KeyboardRX.action == UP)
		{
			if(KeyboardRX.data2 == CAPSLOCK)  //대문자 status
			{
				TRACE("KeyboardCMD_Parsing capslock status: %d \n",(GetKeyState(0x14) & 0x01));
				if ((GetKeyState(VK_CAPITAL) & 0x01) == 0x01)  //캡스락 key가 눌려져있는 상태
				{
				}
				else
				{
					keybd_event( KeyboardRX.data1,0,0,0); 
					keybd_event( KeyboardRX.data1,0,KEYEVENTF_KEYUP,0);
				}
			}
			else if(KeyboardRX.data2 == ENGLISH)  //소문자 status
			{
				TRACE("KeyboardCMD_Parsing capslock status: %d \n",(GetKeyState(0x14) & 0x01));
				if ((GetKeyState(VK_CAPITAL) & 0x01) == 0x01)  //캡스락 key가 눌려져있는 상태
				{
					keybd_event( KeyboardRX.data1,0,0,0); 
					keybd_event( KeyboardRX.data1,0,KEYEVENTF_KEYUP,0);
				}
				else
				{
				}
			}
		}
	}
	else
	{
		if(KeyboardRX.action == DOWN)
		{
			TRACE("키보드 다운\n");  
			keybd_event( KeyboardRX.data1,0,0,0); 
		}
		else if(KeyboardRX.action == UP)
		{
			TRACE("키보드 업\n"); 
			keybd_event( KeyboardRX.data1,0,KEYEVENTF_KEYUP,0);
		}
		else{}
	}

	TRACE("KeyboardCMD_Parsing keycode: %d \n",KeyboardRX.data1);  //data1 = key값
	TRACE("KeyboardCMD_Parsing status: %d \n",KeyboardRX.data2);  //data2 = status 값 한영/캡스락
}
