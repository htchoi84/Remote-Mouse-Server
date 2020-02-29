#include "stdafx.h"
#include "RemoteCMD_handler.h"

RemoteCMD_handler::RemoteCMD_handler(void)
{
}

RemoteCMD_handler::~RemoteCMD_handler(void)
{
}


void RemoteCMD_handler::RemoteCMD_Parsing(RXCMD RX_Data) 
{
	TRACE("RemoteCMD_Parsing remoteRX.subCMD : %d \n",RX_Data.subCMD);

	if(RX_Data.subCMD == GOMPLAYER)
	{
		mGomplayer_handler.GomplayerCMD_Parsing(RX_Data);
	}
	else if(RX_Data.subCMD == ALSHOW)
	{
		mALshow_handler.ALShowCMD_Parsing(RX_Data);
	}
	else if(RX_Data.subCMD == WINMEDIAPLAYER)
	{
		mMediaPlayer_handler.MediaplayerCMD_Parsing(RX_Data);
	}
	else if(RX_Data.subCMD == DAUMPOTPLAYER)
	{
		mDaumPotPlayer_handler.DaumPotPlayerCMD_Parsing(RX_Data);
	}
	/*else if(RX_Data.subCMD == KMPP)
	{
	}
	else if(RX_Data.subCMD == ALSONG)
	{
	}
	else if(RX_Data.subCMD == WINDOW)
	{
	}*/
	else
	{
		TRACE("remote 해당 sub커맨드 없음 \n");
		return;
	}
}
