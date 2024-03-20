/****************************************************************************************
 *   FileName    : SettingIFManager.cpp
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

#include "SettingIFManager.h"
#include "SettingDBusManager.h"
#include "TCInput.h"

extern int g_debug;

#define SETTING_IF_MANAGER_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[SETTING_IF_MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

#define MODE_SETTING_IDLE			"idle"
#define MODE_SETTING_VIEW			"view"
#define MODE_SETTING_HOME			"home"

const char *g_SettingMode[TotalSettingModes] = {
	MODE_SETTING_IDLE,
	MODE_SETTING_VIEW,
	MODE_SETTING_HOME
};

SettingIFManager::SettingIFManager(QObject *parent) :
	QObject(parent)
{
#ifdef ENABLE_TSOUND
	m_EQControl = new EQController;
#endif
}

SettingIFManager::~SettingIFManager()
{
#ifdef ENABLE_TSOUND
	delete m_EQControl;
#endif
}

int SettingIFManager::Initialize(void)
{
	int ret = 0;
#ifdef ENABLE_TSOUND
	ret = m_EQControl->Initialize();
#endif
	InitilaizeSettingDBusManager();

	return ret;
}

void SettingIFManager::Release(void)
{
	ReleaseSettingDBusManager();
#ifdef ENABLE_TSOUND
	m_EQControl->Deinitialize();
#endif
}

#ifdef ENABLE_TSOUND
void SettingIFManager::OnSetBandEQLowBass(int val)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("val:%d\n", val);
	m_EQControl->SetBandEQLowBass(val);
}

void SettingIFManager::OnSetBandEQBass(int val)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("val:%d\n", val);
	m_EQControl->SetBandEQBass(val);
}

void SettingIFManager::OnSetBandEQMid(int val)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("val:%d\n", val);
	m_EQControl->SetBandEQMid(val);
}

void SettingIFManager::OnSetBandEQLowTreble(int val)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("val:%d\n", val);
	m_EQControl->SetBandEQLowTreble(val);
}

void SettingIFManager::OnSetBandEQTreble(int val)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("val:%d\n", val);
	m_EQControl->SetBandEQTreble(val);
}

void SettingIFManager::OnSetPowerBass(int val)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("val:%d\n", val);
	m_EQControl->SetPowerBass(val);
}

void SettingIFManager::OnSetTrebleEnhancement(int val)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("val:%d\n", val);
	m_EQControl->SetTrebleEnhancement(val);
}

void SettingIFManager::OnSetBalance(int val)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("val:%d\n", val);
	m_EQControl->SetBalance(val);
}

void SettingIFManager::OnSetFade(int val)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("val:%d\n", val);
	m_EQControl->SetFade(val);
}
#endif

void SettingIFManager::OnChangeMode(int mode, int app)
{
    SETTING_IF_MANAGER_PROCESS_PRINTF("mode : %d, app : %d\n", mode, app);
    if( (mode >= ModeSettingIdle) && (mode < TotalSettingModes) )
    {
        if( SendDBusChangeMode(g_SettingMode[mode], app) == NACK )
        {
        	SETTING_IF_MANAGER_PROCESS_PRINTF("Change Mode reject! mode : %d, app : %d\n", mode, app);
            //emit ChangeModeReject(mode, app);
        }
    }
}


void SettingIFManager::OnReleaseResourceDone(int resource, int app)
{
    SETTING_IF_MANAGER_PROCESS_PRINTF("resource : %d, app : %d\n", resource, app);
    SendDBusReleaseResourceDone(resource, app);
}

#ifdef ENABLE_BT
void SettingIFManager::OnRequestBTGetStatus(void)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusGetBluetoothStatus();
}

void SettingIFManager::OnRequestBTOnOff(bool operation)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusBluetoothOnOff(operation);
}

void SettingIFManager::OnRequestBTLocalInfo(void)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusGetBluetoothLocalInfo();
}

void SettingIFManager::OnRequestBTDiscoverable(bool operation)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusBluetoothDiscoverable(operation);
}

void SettingIFManager::OnRequestBTSearch(bool operation)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusBluetoothSearch(operation);
}

void SettingIFManager::OnRequestBTConnect(QString addr)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusBluetoothConnect(addr.toLatin1().data());
}

void SettingIFManager::OnRequestBTDisconnect(QString addr)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusBluetoothDisconnect(addr.toLatin1().data());
}

void SettingIFManager::OnRequestBTDelete(QString addr)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusBluetoothDelete(addr.toLatin1().data());
}

void SettingIFManager::OnRequestBTPairedListCount(void)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusBluetoothGetPairedListCount();
}

void SettingIFManager::OnRequestBTPairedList(uint32_t startIndex, uint32_t endIndex)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusBluetoothGetPairedList(startIndex, endIndex);
}

void SettingIFManager::OnRequestBTPairedInfo(uint32_t index)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	SendDBusBluetoothGetPairedDeviceInfo(index);
}
#endif

void SettingIFManager::EmitKeyboardPressedEvent(int key)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("key:%d\n", key);
	emit SigKeyboardPressed(key);
}

void SettingIFManager::EmitKeyboardLongPressedEvent(int key)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("key:%d\n", key);
	emit SigKeyboardLongPressed(key);
}

void SettingIFManager::EmitKeyboardLongLongPressedEvent(int key)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("key:%d\n", key);
	emit SigKeyboardLongLongPressed(key);
}

void SettingIFManager::EmitKeyboardReleasedEvent(int key)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("key:%d\n", key);
	emit SigKeyboardReleased(key);
}

void SettingIFManager::EmitKeyboardClickedEvent(int key)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("key:%d\n", key);
	emit SigKeyboardClicked(key);
}

void SettingIFManager::EmitLauncherEnableKeyEvent(int app)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("APPLICATION(%d)\n", app);
	emit SigLauncherEnableKeyEvent(app);
}

void SettingIFManager::EmitLauncherDisableKeyEvent(int app)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("APPLICATION(%d)\n", app);
	emit SigLauncherDisableKeyEvent(app);
}

void SettingIFManager::EmitModeManagerChangedMode(const char *mode, int app)
{
    int i, len;

    SETTING_IF_MANAGER_PROCESS_PRINTF("Changed Mode(%s, %d)\n", mode, app);

    len = strlen(mode);
    for (i = ModeSettingIdle; i < TotalSettingModes; i++)
    {
        if(!strncmp(mode, g_SettingMode[i], len))
        {
            SETTING_IF_MANAGER_PROCESS_PRINTF("Call ChangedMode(%s, %d)\n", g_SettingMode[i], app);
            emit SigModeManagerChangedMode(i, app);
            break;
        }
    }

    if (i == TotalSettingModes)
    {
        SETTING_IF_MANAGER_PROCESS_PRINTF("Invalid mode (%s, %d)\n", mode, app);
    }
}

void SettingIFManager::EmitModeManagerSuspendMode()
{
    SETTING_IF_MANAGER_PROCESS_PRINTF("Suspend Mode\n");
	emit SigModeManagerSuspendMode();
}

void SettingIFManager::EmitModeManagerResumeMode()
{
    SETTING_IF_MANAGER_PROCESS_PRINTF("Resume Mode\n");
	emit SigModeManagerResumeMode();
}

void SettingIFManager::EmitModeManagerReleaseResource(int resource, int app)
{
    SETTING_IF_MANAGER_PROCESS_PRINTF("Release Resource : %d, AppID : %d\n", resource, app);
    emit SigModeManagerReleaseResource(resource, app);
}

void SettingIFManager::EmitAudioVolumeChagned(unsigned short sourceID, short volume)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("SOURCE ID (%hu) VOLUME (%u)\n",
								  sourceID, volume);
	emit VolumeChanged(sourceID, volume/10);
}

#ifdef ENABLE_BT
void SettingIFManager::EmitBluetoothOnOff(bool isOn)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("\n");
	emit BluetoothOnOff(isOn);
}

void SettingIFManager::EmitBluetoothConnected(char *remoteAddr, char *remoteName)
{
	QString address = remoteAddr;
	QString name = remoteName;

	emit BluetoothConnected(address, name);
}

void SettingIFManager::EmitBluetoothDisConnected(char *addr)
{
	QString address = addr;

	emit BluetoothDisConnected(address);
}


void SettingIFManager::EmitBluetoothProfileStatus(char *addr, uint32_t hf,
																	uint32_t a2dp,
																	uint32_t avrcp,
																	uint32_t pbap,
																	uint32_t map,
																	uint32_t hidd,
																	uint32_t pan)
{
	QString address = addr;
	emit BluetoothProfileStatus(address, hf, a2dp, avrcp, pbap, map, hidd, pan);
}


void SettingIFManager::EmitBluetoothLocalInfo(char *localAddr, char *localName)
{
	QString address = localAddr;
	QString name = localName;

	emit BluetoothLocalInfo(address, name);
}

void SettingIFManager::EmitBluetoothPairedListCount(uint32_t totalNum)
{
	emit BluetoothPairedListCount(totalNum);
}

void SettingIFManager::EmitBluetoothPairedDeviceList(uint32_t index, char *deviceName)
{
	QString name = deviceName;
	emit BluetoothPairedDeviceList(index, name);
}

void SettingIFManager::EmitBluetoothPairedDeviceInfo(uint32_t deviceClass, char * addr, char *name, uint32_t hf, uint32_t a2dp, 
																uint32_t avrcp, uint32_t pbap, uint32_t map, uint32_t hidd , uint32_t PAN)
{
	QString deviceAddress = addr;
	QString deviceName = name;

	emit BluetoothPairedDeviceInfo(deviceClass, deviceAddress, deviceName, hf, a2dp, avrcp, pbap,map, hidd, PAN);

}

void SettingIFManager::EmitBluetoothSearchResult(uint32_t deviceClass, char *address, char *name, int8_t rssi)
{
	QString deviceAddress = address;
	QString deviceName = name;

	emit BluetoothSearchResult(deviceClass, deviceAddress, deviceName, (int)rssi);
}

void SettingIFManager::EmitBluetoothSearchComplete(void)
{
	emit BluetoothSearchComplete();
}
#endif

void SettingIFManager::OnSetVolume(unsigned short sourceID, short volume)
{
	SETTING_IF_MANAGER_PROCESS_PRINTF("SOURCE ID (%hu) VOLUME (%u)\n",
								  sourceID, volume);

	SendDBusSetVolume(sourceID, volume);
}

static SettingIFManager _settingIFManager;
SettingIFManager *GetsettingIFManager()
{
	return &_settingIFManager;
}
