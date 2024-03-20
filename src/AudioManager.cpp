/****************************************************************************************
 *   FileName    : AudioManager.cpp
 *   Description : 
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited 
to re-distribution in source or binary form is strictly prohibited.
This source code is provided “AS IS” and nothing contained in this source code 
shall constitute any express or implied warranty of any kind, including without limitation, 
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent, 
copyright or other third party intellectual property right. 
No warranty is made, express or implied, regarding the information’s accuracy, 
completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, 
out of or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************************/

#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <QMouseEvent>
#include "TCInput.h"
#include "Setting.h"
#include "SettingIFManager.h"
#include "SettingDBusManager.h"
#include "AudioManager.h"

extern int g_debug;
extern int g_appID;

#define AUDIO_MANAGER_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[AUDIOMANAGER_MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

#define MEDIAVOL_SRC_ID				100
#define NAVIVOL_SRC_ID				101
#define TEXTVOL_SRC_ID				102
#define CALLVOL_SRC_ID				103
#define ALERTVOL_SRC_ID				104

AudioManager::AudioManager(QObject *parent) :
	QObject(parent)
{
}

AudioManager::~AudioManager()
{
}

void AudioManager::Initialize(void)
{
	InitializeSignalSlots();

	if( mkdir("/home/root/.telechips", S_IRWXU|S_IRGRP|S_IXGRP) == -1)
	{
		if(errno != 17)
		{
			fprintf(stderr, "%s : mkdir error , error(%d) - %s\n",__FUNCTION__, errno, strerror (errno));
		}
	}

	for(unsigned short i = 100; i < 105; i++)
	{
		_sourceIDtoVolume[i] = 11;
		emit changedVolume(i, _sourceIDtoVolume[i] - 1);
	}
}

void AudioManager::increaseVolume(int type)
{
	if (type == MEDIAVOL_SRC_ID)
	{
		OnMediaVolumeUpClicked();
	}
	else if (type == NAVIVOL_SRC_ID)
	{
		OnNaviVolumeUpClicked();
	}
	else if (type == TEXTVOL_SRC_ID)
	{
		OnTextVolumeUpClicked();
	}
	else if (type == CALLVOL_SRC_ID)
	{
		OnCallVolumeUpClicked();
	}
	else if (type == ALERTVOL_SRC_ID)
	{
		OnAlertVolumeUpClicked();
	}
	else
	{
		AUDIO_MANAGER_PROCESS_PRINTF("Invalid Volume Type\n");
	}
}

void AudioManager::decreaseVolume(int type)
{
	if (type == MEDIAVOL_SRC_ID)
	{
		OnMediaVolumeDownClicked();
	}
	else if (type == NAVIVOL_SRC_ID)
	{
		OnNaviVolumeDownClicked();
	}
	else if (type == TEXTVOL_SRC_ID)
	{
		OnTextVolumeDownClicked();
	}
	else if (type == CALLVOL_SRC_ID)
	{
		OnCallVolumeDownClicked();
	}
	else if (type == ALERTVOL_SRC_ID)
	{
		OnAlertVolumeDownClicked();
	}
	else
	{
		AUDIO_MANAGER_PROCESS_PRINTF("Invalid Volume Type\n");
	}
}

void AudioManager::InitializeSignalSlots(void)
{
	connect(SETTING_IF_MANAGER, SIGNAL(VolumeChanged(unsigned short, short)), SLOT(OnVolumeChanged(unsigned short , short)));

	connect(this, SIGNAL(SetVolume(unsigned short, short)), SETTING_IF_MANAGER, SLOT(OnSetVolume(unsigned short, short)));
}

void AudioManager::OnMediaVolumeDownClicked(void)
{
	unsigned short SourceID = MEDIAVOL_SRC_ID;
	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	if(_sourceIDtoVolume[SourceID] > 0)
	{
		short setvolume = 0;
		setvolume = _sourceIDtoVolume[SourceID];
		setvolume = (setvolume-1)*10;
		emit SetVolume(SourceID, setvolume);
	}
}

void AudioManager::OnTextVolumeDownClicked(void)
{
	unsigned short SourceID = TEXTVOL_SRC_ID;
	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	if(_sourceIDtoVolume[SourceID] > 0)
	{
		short setvolume = 0;
		setvolume = _sourceIDtoVolume[SourceID];
		setvolume = (setvolume-1)*10;
		emit SetVolume(SourceID, setvolume);
	}
}

void AudioManager::OnNaviVolumeDownClicked(void)
{
	unsigned short SourceID = NAVIVOL_SRC_ID;
	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	if(_sourceIDtoVolume[SourceID] > 0)
	{
		short setvolume = 0;
		setvolume = _sourceIDtoVolume[SourceID];
		setvolume = (setvolume-1)*10;
		emit SetVolume(SourceID, setvolume);
	}
}

void AudioManager::OnCallVolumeDownClicked(void)
{
	unsigned short SourceID = CALLVOL_SRC_ID;
	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	if(_sourceIDtoVolume[SourceID] > 0)
	{
		short setvolume = 0;
		setvolume = _sourceIDtoVolume[SourceID];
		setvolume = (setvolume-1)*10;
		emit SetVolume(SourceID, setvolume);
	}
}

void AudioManager::OnAlertVolumeDownClicked(void)
{
	unsigned short SourceID = ALERTVOL_SRC_ID;
	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	if(_sourceIDtoVolume[SourceID] > 0)
	{
		short setvolume = 0;
		setvolume = _sourceIDtoVolume[SourceID];
		setvolume = (setvolume-1)*10;
		emit SetVolume(SourceID, setvolume);
	}
}

void AudioManager::OnMediaVolumeUpClicked(void)
{
	unsigned short SourceID = MEDIAVOL_SRC_ID;
	if(_sourceIDtoVolume[SourceID] < MAX_VOLUME_COUNT - 1)
	{
       	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	       short setvolume = 0;
       	setvolume = _sourceIDtoVolume[SourceID];
	       setvolume = (setvolume+1)*10;
	 	emit SetVolume(SourceID,setvolume);
	}
}

void AudioManager::OnTextVolumeUpClicked(void)
{
	unsigned short SourceID = TEXTVOL_SRC_ID;
	if(_sourceIDtoVolume[SourceID] < MAX_VOLUME_COUNT - 1)
	{
       	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	       short setvolume = 0;
       	setvolume = _sourceIDtoVolume[SourceID];
	       setvolume = (setvolume+1)*10;
	 	emit SetVolume(SourceID,setvolume);
	}
}

void AudioManager::OnNaviVolumeUpClicked(void)
{
	unsigned short SourceID = NAVIVOL_SRC_ID;
	if(_sourceIDtoVolume[SourceID] < MAX_VOLUME_COUNT - 1)
	{
       	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	       short setvolume = 0;
       	setvolume = _sourceIDtoVolume[SourceID];
	       setvolume = (setvolume+1)*10;
	 	emit SetVolume(SourceID,setvolume);
	}
}

void AudioManager::OnCallVolumeUpClicked(void)
{
	unsigned short SourceID = CALLVOL_SRC_ID;
	if(_sourceIDtoVolume[SourceID] < MAX_VOLUME_COUNT - 1)
	{
       	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	       short setvolume = 0;
       	setvolume = _sourceIDtoVolume[SourceID];
	       setvolume = (setvolume+1)*10;
	 	emit SetVolume(SourceID,setvolume);
	}
}

void AudioManager::OnAlertVolumeUpClicked(void)
{
	unsigned short SourceID = ALERTVOL_SRC_ID;
	if(_sourceIDtoVolume[SourceID] < MAX_VOLUME_COUNT - 1)
	{
       	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[SourceID]);
	       short setvolume = 0;
       	setvolume = _sourceIDtoVolume[SourceID];
	       setvolume = (setvolume+1)*10;
	 	emit SetVolume(SourceID,setvolume);
	}
}

void AudioManager::OnVolumeChanged(unsigned short sourceID, short volume)
{
	AUDIO_MANAGER_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", volume);

	_sourceIDtoVolume[sourceID] = volume;
	emit changedVolume(sourceID, volume);
}
