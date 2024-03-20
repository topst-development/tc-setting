/****************************************************************************************
 *   FileName    : Setting.cpp
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
#include <string>
#include <cstdint>
#include <unistd.h>
#include "TCInput.h"
#include "SettingIFManager.h"
#include "Setting.h"
#include "EQMacro.h"

extern int g_debug;
extern int g_appID;
extern bool g_rear;

#define SETTING_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[SETTING] %s: " format "", __FUNCTION__, ##arg); \
	}

#define LAUNCHER_APPID      0

#define RESOURCE_NONE       0x00
#define RESOURCE_DISPLAY    0x01
#define RESOURCE_AUDIO      0x02
#define RESOURCE_MASK       0xFF

Setting::Setting(QObject *parent) :
	QObject(parent),
	m_enableKey(true)
{
}

Setting::~Setting()
{
}

void Setting::Initialize(int tsound)
{
	if(tsound != -1)
	{
#ifdef ENABLE_TSOUND
		m_BandEQManager->Initialize(WIDGET_X_OFFSET, 0, 1024-WIDGET_X_OFFSET, 600-WIDGET_Y_OFFSET);

		m_PowerBassTrebleEnhancementManger->Initialize(WIDGET_X_OFFSET, 0, 1024-WIDGET_X_OFFSET, 600-WIDGET_Y_OFFSET);

		m_BalanceFaderManager->Initialize(WIDGET_X_OFFSET, 0, 1024-WIDGET_X_OFFSET, 600-WIDGET_Y_OFFSET);
#endif
	}

#ifdef CA_ENABLE
	m_btnSetting.AddButton(m_btnColorAdjustment);
	m_ColorAdjustmentManager->Initialize(WIDGET_X_OFFSET, 0, 1024-WIDGET_X_OFFSET, 600-WIDGET_Y_OFFSET);
	m_ColorAdjustmentManager->hide();
#endif
	InitializeSignalSlots();
}

void Setting::AddMenu(QString menu)
{
	SETTING_PROCESS_PRINTF("%s added\n", menu.toLatin1().data());
	m_menu << menu;
}

void Setting::CompleteMenu(void)
{
	SETTING_PROCESS_PRINTF("\n");
	emit menuChanged();
}

void Setting::goHome(void)
{
	SETTING_PROCESS_PRINTF("Go to the HOME. SHOW LAUNCHER\n");
	emit ChangeMode(ModeSettingHome, LAUNCHER_APPID);
}

void Setting::goBack(void)
{
	SETTING_PROCESS_PRINTF("BACK, SHOW LAUNCHER\n");
	emit ChangeMode(ModeSettingView, LAUNCHER_APPID);
}

void Setting::goSetting(void)
{
	SETTING_PROCESS_PRINTF("SETTING PRESSED\n");
	emit ChangeMode(ModeSettingView, g_appID);
}

void Setting::ShowSetting()
{
	SETTING_PROCESS_PRINTF("SHOW Setting Application\n");
	emit showHideChanged(true);
}

void Setting::HideSetting()
{
	SETTING_PROCESS_PRINTF("HIDE Setting Application\n");
	emit showHideChanged(false);
}


void Setting::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	connect(SETTING_IF_MANAGER, SIGNAL(SigLauncherEnableKeyEvent(int)),
			this, SLOT(OnLauncherEnableKeyEvent(int)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(SigLauncherDisableKeyEvent(int)),
			this, SLOT(OnLauncherDisableKeyEvent(int)), dbusConnectionType);

	connect(SETTING_IF_MANAGER, SIGNAL(SigKeyboardClicked(int)), this, SLOT(OnKeyboardClicked(int)), dbusConnectionType);

	connect(SETTING_IF_MANAGER, SIGNAL(SigModeManagerChangedMode(int, int)),
			this, SLOT(OnChangedMode(int, int)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(SigModeManagerSuspendMode()),
			this, SLOT(OnSuspendMode()), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(SigModeManagerResumeMode()),
			this, SLOT(OnResumeMode()), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(SigModeManagerReleaseResource(int, int)),
			this, SLOT(OnReleaseResource(int, int)), dbusConnectionType);
	connect(this, SIGNAL(ChangeMode(int, int)),
			SETTING_IF_MANAGER, SLOT(OnChangeMode(int, int)), dbusConnectionType);
	connect(this, SIGNAL(ReleaseResourceDone(int, int)),
			SETTING_IF_MANAGER, SLOT(OnReleaseResourceDone(int, int)), dbusConnectionType);
}

void Setting::OnKeyboardClicked(int key)
{
	SETTING_PROCESS_PRINTF("key: %d\n", key);

	if (m_enableKey) {
		if (key == g_knobKeys[TCKeyHome])
		{
			emit homePressed();
		}
		else if(key == g_knobKeys[TCKeyBack])
		{
			emit backPressed();
		}
		else if (key == g_knobKeys[TCKeySetting]) {
			emit settingPressed();
		}
	}
	else {
		if(key == g_knobKeys[TCKeySetting]) {
			emit ChangeMode(ModeSettingView, g_appID);
		}
	}
}

void Setting::OnLauncherEnableKeyEvent(int app)
{
	SETTING_PROCESS_PRINTF("APPLICATION(%d)\n", app);

	if (app != g_appID) {
		m_enableKey = true;
	}
}

void Setting::OnLauncherDisableKeyEvent(int app)
{
	SETTING_PROCESS_PRINTF("APPLICATION(%d)\n", app);

	if (app != g_appID) {
		m_enableKey = false;
	}
}

void Setting::OnChangedMode(int mode, int app)
{
	SETTING_PROCESS_PRINTF("Mode(%s), application(%d)\n", g_SettingMode[mode], app);

	switch(mode)
	{
		case ModeSettingIdle:
			HideSetting();
			break;
		case ModeSettingView:
			ShowSetting();
			break;
		default:
			SETTING_PROCESS_PRINTF("Unknown mode : %d\n", mode);
			break;
	}
}

void Setting::OnSuspendMode()
{
	SETTING_PROCESS_PRINTF("Suspend Mode : Go to the HOME.\n");
	emit ChangeMode(ModeSettingHome, LAUNCHER_APPID);
}

void Setting::OnResumeMode()
{
	SETTING_PROCESS_PRINTF("Resume Mode : Nothing to do\n");
}

void Setting::OnReleaseResource(int resource, int app)
{
	SETTING_PROCESS_PRINTF("Resource(Display : %s, Audio : %s)\n",
			(resource & RESOURCE_DISPLAY) ? "release" : "none",
			(resource & RESOURCE_AUDIO) ? "release" : "none" );

	if((resource & RESOURCE_MASK) == (RESOURCE_DISPLAY | RESOURCE_AUDIO))
	{
		HideSetting();
		emit ReleaseResourceDone(resource, app);
	}
	else if((resource & RESOURCE_MASK) == RESOURCE_AUDIO)
	{
		emit ReleaseResourceDone(resource, app);
	}
	else if((resource & RESOURCE_MASK) == RESOURCE_DISPLAY)
	{
		HideSetting();
		emit ReleaseResourceDone(resource, app);
	}
	else
	{
		SETTING_PROCESS_PRINTF("Unknown Resource : 0x%x\n", resource);
	}
}
