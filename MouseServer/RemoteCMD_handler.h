#if !defined(AFX_RemoteCMD_handler_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_)
#define AFX_RemoteCMD_handler_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_

#pragma once

#include "RXCMD.h"
#include "Gomplayer_handler.h"
#include "ALshow_handler.h"
#include "MediaPlayer_handler.h"
#include "DaumPotPlayer_handler.h"

class RemoteCMD_handler
{
private:
	Gomplayer_handler mGomplayer_handler;
	ALshow_handler mALshow_handler;
	MediaPlayer_handler mMediaPlayer_handler;
	DaumPotPlayer_handler mDaumPotPlayer_handler;
public: 
	RemoteCMD_handler(void);
	~RemoteCMD_handler(void);
	void RemoteCMD_Parsing(RXCMD RX_Data); //remote-cmd= 헤더로 시작되는 커맨드들 parsing하는 함수
};

#endif