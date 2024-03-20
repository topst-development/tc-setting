/****************************************************************************************
 *   FileName    : FWUpdateManager.cpp
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
#include <linux/limits.h>
#include "Setting.h"
#include "SettingIFManager.h"
#include "SettingDBusManager.h"
#include "FWUpdateManager.h"
#include "FWUpdateThread.h"
#include "update-engine-def.h"
#include "update-engine.h"

extern int g_debug;

#define FWUPDATE_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[FWUPDATE_MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

#define FWUPDATE_ERROR_PROCESS_PRINTF(format, arg...) \
			fprintf(stderr, "[FWUPDATE_MANAGER][ERROR] %s: " format "", __FUNCTION__, ##arg);

//#define ENABLE_CORE_SPECIFIC_UPDATE

FWUpdate::FWUpdate(QObject *parent) :
	QObject(parent),
	m_coreSpecUpdate(false),
	m_allUpdate(false),
	m_mainUpdate(false),
	m_subUpdate(false),
	m_snorUpdate(false),
	m_initFWUpdate(false),
	m_nowUpdating(false)

{
#ifdef ENABLE_CORE_SPECIFIC_UPDATE
	m_coreSpecUpdate = true;
#endif
	m_FWUpdateThread = new FWUpdateThread(this);
}

FWUpdate::~FWUpdate()
{
}

void FWUpdate::Initialize(void)
{
	int32_t ret;
	FWUPDATE_PROCESS_PRINTF("Init\n");

	InitializeSignalSlots();

	if(g_debug > 0)
	{
		setDebugLogLevel(3);
	}

	ret = initUpdateEngine(MAIN_CORE);
	if(ret != 0 )
	{
		m_initFWUpdate = false;
		FWUPDATE_ERROR_PROCESS_PRINTF("FWUpdate Init Error\n");
	}
}

void FWUpdate::showUpdateMenu(void)
{
	int32_t mainImageResult, subImageResult, snorImageResult;
	int32_t ret;

	FWUPDATE_PROCESS_PRINTF("In\n");

	m_resultLog = "If menu is not activated, insert the USB with the images";
	m_notifyLog = "Firmware update";

	emit resultLogChanged();
	emit notifyLogChanged();

	if(m_nowUpdating == false)
	{
		ret = find_update_dir();

		if (ret == 0)
		{
			mainImageResult = updateFWImageFileCheck(MAIN_CORE, (const char *)m_FWUpdateThread->update_main_dir_path);
			subImageResult =  updateFWImageFileCheck(SUB_CORE, (const char *)m_FWUpdateThread->update_sub_dir_path);
			snorImageResult = updateFWImageFileCheck(MICOM_CORE, (const char *)m_FWUpdateThread->update_main_dir_path);

#ifdef ENABLE_CORE_SPECIFIC_UPDATE
			m_mainUpdate = (mainImageResult == 0);
			m_subUpdate = (subImageResult == 0);
			m_snorUpdate = (snorImageResult == 0);

			emit mainUpdateChanged();
			emit subUpdateChanged();
			emit snorUpdateChanged();
#endif

			m_allUpdate = ((mainImageResult == 0) && (subImageResult == 0) && (snorImageResult == 0));
			emit allUpdateChanged();
		}
	}
}

void FWUpdate::updateAllFW(void)
{
	FWUPDATE_PROCESS_PRINTF("\n");
#ifdef ENABLE_CORE_SPECIFIC_UPDATE
	m_mainUpdate = false;
	m_subUpdate = false;
	m_snorUpdate = false;

	emit mainUpdateChanged();
	emit subUpdateChanged();
	emit snorUpdateChanged();
#endif
	m_allUpdate = false;
	m_notifyLog = "Select ALL core Update. Main->Sub->Snor";
	m_resultLog = "Main Update Start. This can take a long time.";
	emit allUpdateChanged();
	emit resultLogChanged();
	emit notifyLogChanged();

	m_nowUpdating = true;
	m_FWUpdateThread->Run(CMD_UPDATE_ALL);

	FWUPDATE_PROCESS_PRINTF("Done \n");
}

void FWUpdate::updateMainFW(void)
{
	FWUPDATE_PROCESS_PRINTF("\n");

	m_allUpdate = false;
	m_mainUpdate = false;
	m_subUpdate = false;
	m_snorUpdate = false;

	emit allUpdateChanged();
	emit mainUpdateChanged();
	emit subUpdateChanged();
	emit snorUpdateChanged();

	m_resultLog = "Main Update Start. This can take a long time.";
	m_notifyLog = "Select Main core Update.";

	emit resultLogChanged();
	emit notifyLogChanged();

	m_nowUpdating = true;
	m_FWUpdateThread->Run(CMD_UPDATE_MAIN_CORE);

	FWUPDATE_PROCESS_PRINTF("Done \n");
}

void FWUpdate::updateSubFW(void)
{
	FWUPDATE_PROCESS_PRINTF("\n");

	m_allUpdate = false;
	m_mainUpdate = false;
	m_subUpdate = false;
	m_snorUpdate = false;

	emit allUpdateChanged();
	emit mainUpdateChanged();
	emit subUpdateChanged();
	emit snorUpdateChanged();

	m_resultLog = "Sub Update Start. This can take a long time.";
	m_notifyLog = "Select Sub core Update.";

	emit resultLogChanged();
	emit notifyLogChanged();

	m_nowUpdating = true;
	m_FWUpdateThread->Run(CMD_UPDATE_SUB_CORE);

	FWUPDATE_PROCESS_PRINTF("Done \n");
}

void FWUpdate::updateSnorFW(void)
{
	FWUPDATE_PROCESS_PRINTF("\n");

	m_allUpdate = false;
	m_mainUpdate = false;
	m_subUpdate = false;
	m_snorUpdate = false;

	emit allUpdateChanged();
	emit mainUpdateChanged();
	emit subUpdateChanged();
	emit snorUpdateChanged();

	m_resultLog = "Snor Update Start. This can take a long time.";
	m_notifyLog = "Select Snor core Update.";

	emit resultLogChanged();
	emit notifyLogChanged();

	m_nowUpdating = true;
	m_FWUpdateThread->Run(CMD_UPDATE_SNOR);

	FWUPDATE_PROCESS_PRINTF("Done \n");
}

void FWUpdate::InitializeSignalSlots(void)
{
	connect(m_FWUpdateThread, SIGNAL(updateResult(int ,int)), this, SLOT(OnFWUpdateResult(int, int)));
	connect(m_FWUpdateThread, SIGNAL(updateDone()), this, SLOT(OnFWUpdateDone()));
}

void FWUpdate::OnFWUpdateResult(int updateCore,int result)
{
	FWUPDATE_PROCESS_PRINTF("Core_ID(%d), result(%d)\n", updateCore, result);

	QString resultLog;

	switch(updateCore)
	{
		case CMD_UPDATE_MAIN_CORE:
			resultLog.append("Main Core: ");
			break;
		case CMD_UPDATE_SUB_CORE:
			resultLog.append("Sub Core: ");
			break;
		case CMD_UPDATE_SNOR:
			resultLog.append("Snor: ");
			break;
		default:
			resultLog.append("Error!!");
			break;
	}

	if(result == 0)
	{
		resultLog.append("Update Success");

		if(updateCore == CMD_UPDATE_SNOR)
		{
			resultLog.append(", Please Reset!");
		}
	}
	else
	{
		resultLog.append("Update Fail");
	}

	m_resultLog = resultLog;

	emit resultLogChanged();
}

void FWUpdate::OnFWUpdateDone()
{
	FWUPDATE_PROCESS_PRINTF("In\n");
	m_FWUpdateThread->Stop();
	m_nowUpdating = false;
}

int FWUpdate::find_update_dir()
{
	FILE * dirStream;
	int ret = -1;
	size_t path_size;

	(void)memset(m_FWUpdateThread->update_main_dir_path, 0x00, PATH_MAX);
	(void)memset(m_FWUpdateThread->update_sub_dir_path, 0x00, PATH_MAX);

	FWUPDATE_PROCESS_PRINTF("In\n");

	/* find main update dir */
	dirStream = popen("find /run/media/ -maxdepth 2 -type d -name update_main", "r");
	if (dirStream != NULL)
	{
		if(fgets(m_FWUpdateThread->update_main_dir_path, PATH_MAX, dirStream) != NULL)
		{
			path_size = strlen(m_FWUpdateThread->update_main_dir_path);
			m_FWUpdateThread->update_main_dir_path[path_size - 1] = '\0';
			FWUPDATE_PROCESS_PRINTF("find update dir : %s\n",m_FWUpdateThread->update_main_dir_path);
			ret = 0;
		}
		else
		{
			FWUPDATE_PROCESS_PRINTF("Not find main update dir\n");
		}

		pclose(dirStream);
		dirStream = NULL;
	}
	else
	{
		FWUPDATE_PROCESS_PRINTF("Not find main update dir\n");
	}

	if(ret == 0)
	{
		ret = -1;
		/* find sub update dir */
		dirStream = popen("find /run/media/ -maxdepth 2 -type d -name update_sub", "r");
		if (dirStream != NULL)
		{
			if(fgets(m_FWUpdateThread->update_sub_dir_path, PATH_MAX, dirStream) != NULL)
			{
				path_size = strlen(m_FWUpdateThread->update_sub_dir_path);
				m_FWUpdateThread->update_sub_dir_path[path_size - 1] = '\0';
				FWUPDATE_PROCESS_PRINTF("find update dir : %s\n",m_FWUpdateThread->update_sub_dir_path);
				ret = 0;
			}
			else
			{
				FWUPDATE_PROCESS_PRINTF("Not find sub update dir\n");
			}

			pclose(dirStream);
			dirStream = NULL;
		}
		else
		{
			FWUPDATE_PROCESS_PRINTF("Not find sub update dir\n");
		}

	}
	return ret;
}

