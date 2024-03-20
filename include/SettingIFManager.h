/****************************************************************************************
 *   FileName    : SettingIFManager.h
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
#ifndef SETTING_IF_MANAGER_H
#define SETTING_IF_MANAGER_H

#include <QObject>
#include <stdio.h>

#ifdef ENABLE_TSOUND
#include "EQController.h"
#endif

typedef enum
{
    ModeSettingIdle = 0x00,
    ModeSettingView,
    ModeSettingHome,
    TotalSettingModes
}SettingMode;

#define NACK        0
#define ACK         1

extern const char *g_SettingMode[TotalSettingModes];

class SettingIFManager : public QObject
{
	Q_OBJECT

	public:
		SettingIFManager(QObject *parent = 0);
		~SettingIFManager();
		int Initialize(void);
		void Release(void);

		void EmitLauncherShowApplication(int app);
		void EmitLauncherHideApplications(int app);
		void EmitLauncherExitApplications();
		void EmitKeyboardPressedEvent(int key);
		void EmitKeyboardLongPressedEvent(int key);
		void EmitKeyboardLongLongPressedEvent(int key);
		void EmitKeyboardReleasedEvent(int key);
		void EmitKeyboardClickedEvent(int key);
		void EmitLauncherEnableKeyEvent(int app);
		void EmitLauncherDisableKeyEvent(int app);
		void EmitModeManagerChangedMode(const char *mode, int app);
		void EmitModeManagerSuspendMode();
		void EmitModeManagerResumeMode();
		void EmitModeManagerReleaseResource(int resource, int app);

		void EmitAudioVolumeChagned(unsigned short sourceID, short volume);

#ifdef ENABLE_BT
		void EmitBluetoothOnOff(bool isOn);
		void EmitBluetoothConnected(char *remoteAddr, char *remoteName);
		void EmitBluetoothDisConnected(char *address);
		void EmitBluetoothProfileStatus(char *addr, uint32_t hf, uint32_t a2dp, uint32_t avrcp,
												uint32_t pbap, uint32_t map, uint32_t hidd, uint32_t pan);
		void EmitBluetoothPairedDeviceInfo(uint32_t deviceClass, char * addr, char *name, uint32_t hf, uint32_t a2dp,
											uint32_t avrcp, uint32_t pbap, uint32_t map, uint32_t hidd , uint32_t PAN);
		void EmitBluetoothLocalInfo(char *localAddr, char *localName);
		void EmitBluetoothPairedListCount(uint32_t totalNum);
		void EmitBluetoothPairedDeviceList(uint32_t index, char *deviceName);
		void EmitBluetoothSearchResult(uint32_t deviceClass, char *address, char *name, int8_t rssi);
		void EmitBluetoothSearchComplete(void);
#endif

   	private :
#ifdef ENABLE_TSOUND
		EQController *m_EQControl;
#endif

	private slots :
		//AudioManager Interface
		void OnSetVolume(unsigned short sourceID, short volume);

#ifdef ENABLE_TSOUND
		//EQManager Interface
		void OnSetBandEQLowBass(int val);
		void OnSetBandEQBass(int val);
		void OnSetBandEQMid(int val);
		void OnSetBandEQLowTreble(int val);
		void OnSetBandEQTreble(int val);
		void OnSetPowerBass(int val);
		void OnSetTrebleEnhancement(int val);
		void OnSetBalance(int val);
		void OnSetFade(int val);
#endif

		//ModeManager Interface
		void OnChangeMode(int mode, int app);
		void OnReleaseResourceDone(int resource, int app);

#ifdef ENABLE_BT
		//Bluetooth
		void OnRequestBTGetStatus(void);
		void OnRequestBTOnOff(bool operation);
		void OnRequestBTLocalInfo(void);
		void OnRequestBTDiscoverable(bool operation);
		void OnRequestBTSearch(bool operation);
		void OnRequestBTConnect(QString addr);
		void OnRequestBTDisconnect(QString addr);
		void OnRequestBTDelete(QString addr);
		void OnRequestBTPairedListCount(void);
		void OnRequestBTPairedList(uint32_t startIndex, uint32_t endIndex);
		void OnRequestBTPairedInfo(uint32_t index);
#endif

	signals :
		void SigKeyboardPressed(int key);
		void SigKeyboardLongPressed(int key);
		void SigKeyboardLongLongPressed(int key);
		void SigKeyboardReleased(int key);
		void SigKeyboardClicked(int key);
		void SigLauncherEnableKeyEvent(int app);
		void SigLauncherDisableKeyEvent(int app);
		void SigModeManagerChangedMode(int mode, int app);
		void SigModeManagerReleaseResource(int resource, int app);
		void SigModeManagerSuspendMode(void);
		void SigModeManagerResumeMode(void);

		void VolumeChanged(unsigned short sourceID, short volume);

#ifdef ENABLE_BT
		void BluetoothOnOff(bool isBtOn);
		void BluetoothConnected(QString remoteAddr, QString remoteName);
		void BluetoothDisConnected(QString address);
		void BluetoothLocalInfo(QString localAddr, QString localName);
		void BluetoothPairedListCount(uint32_t totalNum);
		void BluetoothPairedDeviceList(uint32_t index, QString name);
		void BluetoothSearchResult(uint32_t deviceClass, QString address, QString name, int rssi);
		void BluetoothSearchComplete(void);
		void BluetoothProfileStatus(QString addr, uint32_t hf, uint32_t a2dp, uint32_t avrcp,
											uint32_t pbap, uint32_t map, uint32_t hidd, uint32_t pan);
		void BluetoothPairedDeviceInfo(uint32_t deviceClass, QString addr, QString name, uint32_t hf, uint32_t a2dp,
											uint32_t avrcp, uint32_t pbap, uint32_t map, uint32_t hidd , uint32_t PAN);
#endif
};

extern SettingIFManager *GetsettingIFManager();
#define SETTING_IF_MANAGER (GetsettingIFManager())

#endif
