/****************************************************************************************
 *   FileName    : SettingDBusManager.h
 *   Description : 
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited 
to re-distribution in source or binary form is strictly prohibited.
This source code is provided ¡°AS IS¡± and nothing contained in this source code 
shall constitute any express or implied warranty of any kind, including without limitation, 
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent, 
copyright or other third party intellectual property right. 
No warranty is made, express or implied, regarding the information¡¯s accuracy, 
completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, 
out of or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************************/

#ifndef SETTING_DBUS_MANAGER_H
#define SETTING_DBUS_MANAGER_H

void InitilaizeSettingDBusManager(void);
void ReleaseSettingDBusManager(void);

void SendDBusActiveApplication(int app, int active);
void SendDBusSetVolume(unsigned short sourceID, short volume);
void SendDBusGetVolumeStep(unsigned short sourceID);

int SendDBusChangeMode(const char* mode, int app);
void SendDBusReleaseResourceDone(int resource, int app);

#ifdef ENABLE_BT
/* Bluetooth Method */
void SendDBusGetBluetoothStatus(void);
void SendDBusBluetoothOnOff(bool operation);
void SendDBusGetBluetoothLocalInfo(void);

void SendDBusBluetoothDiscoverable(bool operation);
void SendDBusBluetoothSearch(bool operation);

void SendDBusBluetoothConnect(const char *deviceAddr);
void SendDBusBluetoothDisconnect(const char *deviceAddr);
void SendDBusBluetoothDelete(const char *deviceAddr);
void SendDBusBluetoothGetPairedListCount(void);
void SendDBusBluetoothGetPairedList(uint32_t startIndex, uint32_t endIndex);
void SendDBusBluetoothGetPairedDeviceInfo(uint32_t index);
#endif

#endif // EQ_DBUS_MANAGER_H
