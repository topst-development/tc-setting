/****************************************************************************************
 *   FileName    : FWUpdateThread.cpp
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
#include <QThread>
#include "FWUpdateManager.h"
#include "FWUpdateThread.h"
#include "update-engine-def.h"
#include "update-engine.h"

extern int g_debug;

#define FWUPDATE_THREAD_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[FWUPDATE_THREAD] %s: " format "", __FUNCTION__, ##arg); \
	}

#define FWUPDATE_ERROR_THREAD_PRINTF(format, arg...) \
			fprintf(stderr, "[FWUPDATE_THREAD][ERROR] %s: " format "", __FUNCTION__, ##arg);

FWUpdateThread::FWUpdateThread(QObject *parent) :
		QThread(parent),
		_update_cmd(-1)
{
}

FWUpdateThread::~FWUpdateThread()
{
	Stop();
}

void FWUpdateThread::Run(int32_t update_cmd)
{
	FWUPDATE_THREAD_PRINTF("targetCore:(%d)\n", update_cmd);
	_update_cmd = update_cmd;
	this->start();
}

void FWUpdateThread::run()
{
	FWUPDATE_THREAD_PRINTF("targetCore:(%d)\n", _update_cmd);

	int32_t ret = -1;
	int32_t updateCore = -1;

	if(_update_cmd > -1)
	{
		if(_update_cmd == CMD_UPDATE_MAIN_CORE)
		{
			ret = updateFWImage(MAIN_CORE,update_main_dir_path);
			updateCore = CMD_UPDATE_MAIN_CORE;
			emit updateResult(updateCore, ret);
		}
		else if(_update_cmd == CMD_UPDATE_SUB_CORE)
		{
			ret = updateFWImage(SUB_CORE,update_sub_dir_path);
			updateCore = CMD_UPDATE_SUB_CORE;
			emit updateResult(updateCore, ret);			
		}
		else if(_update_cmd == CMD_UPDATE_SNOR)
		{
			ret = updateFWImage(MICOM_CORE,update_main_dir_path);
			updateCore = CMD_UPDATE_SNOR;
			emit updateResult(updateCore, ret);

		}
		else if(_update_cmd == CMD_UPDATE_ALL)
		{
			ret = updateFWImage(MAIN_CORE,update_main_dir_path);
			updateCore = CMD_UPDATE_MAIN_CORE;
			emit updateResult(updateCore, ret);
			if(ret == 0)
			{
				ret = updateFWImage(SUB_CORE,update_sub_dir_path);
				updateCore = CMD_UPDATE_SUB_CORE;
				emit updateResult(updateCore, ret);				
			}

			if(ret == 0)
			{
				ret = updateFWImage(MICOM_CORE,update_main_dir_path);
				updateCore = CMD_UPDATE_SNOR;
				emit updateResult(updateCore, ret);				
			}
		}
	}
	emit updateDone();
	FWUPDATE_THREAD_PRINTF("result : %d\n", ret);
	this->exec();
}

void FWUpdateThread::Stop()
{
	ExitFWUpdateThread();
}

void FWUpdateThread::ExitFWUpdateThread()
{
	this->exit();
	this->wait();
}

static FWUpdateThread _fwupdateThread;
FWUpdateThread *GetfwUpdateThread()
{
	return &_fwupdateThread;
}

