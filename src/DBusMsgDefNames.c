/****************************************************************************************
 *   FileName    : DBusMsgDefNames.c
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


#include "DBusMsgDef.h"

const char *g_signalLauncherEventNames[TotalSignalLauncherEvents] = {
    SIGNAL_LAUNCHER_KEY_PRESSED,
    SIGNAL_LAUNCHER_KEY_LONG_PRESSED,
    SIGNAL_LAUNCHER_KEY_LONG_LONG_PRESSED,
    SIGNAL_LAUNCHER_KEY_RELEASED,
    SIGNAL_LAUNCHER_KEY_CLICKED,
    SIGNAL_LAUNCHER_ENABLE_KEY_EVENT,
    SIGNAL_LAUNCHER_DISABLE_KEY_EVENT
};

const char *g_methodLauncherEventNames[TotalMethodLauncherEvents] = {
    METHOD_LAUNCHER_ACTIVE_APPLICATION,
    METHOD_LAUNCHER_START_APPLICATIONS,
    METHOD_LAUNCHER_PREEMPT_KEY_EVENT,
    METHOD_LAUNCHER_RELEASE_PREEMPT_KEY_EVENT
};


const char *g_signalAudioManagerEventNames[TotalSignalAudioManagerEvents] = {
    SIGNAL_AUDIOMANAGER_VOLUME_CHANGED
};

const char *g_methodAudioManagerEventNames[TotalMethodAudioManagerEvents] = {
    METHOD_AUDIOMANAGER_GET_VOLUME,
    METHOD_AUDIOMANAGER_SET_VOLUME
};

const char *g_signalBluetoothApplicationEventNames[TotalSignalBluetoothApplicationEvents] = {
	SIGNAL_BLUETOOTH_BT_ON_OFF,
	SIGNAL_BLUETOOTH_LOCAL_INFO,
	SIGNAL_BLUETOOTH_SEARCH_RESULT,
	SIGNAL_BLUETOOTH_SEARCH_COMPLETE,
	SIGNAL_BLUETOOTH_CONNECTED,
	SIGNAL_BLUETOOTH_DISCONNECTED,
	SIGNAL_BLUETOOTH_PROFILE_STATUS,
	SIGNAL_BLUETOOTH_PAIRED_DEVICE_LIST,
	SIGNAL_BLUETOOTH_PAIRED_DEVICE_INFO
};

const char *g_methodBluetoothApplicationEventNames[TotalMethodBluetoothApplicationEvents] = {
	METHOD_BLUETOOTH_BT_ON_OFF,
	METHOD_BLUETOOTH_GET_LOCAL_INFO,
	METHOD_BLUETOOTH_DISCOVERABLE,
	METHOD_BLUETOOTH_SEARCH,
	METHOD_BLUETOOTH_CONNECT,
	METHOD_BLUETOOTH_DISCONNECT,
	METHOD_BLUETOOTH_DELETE,
	METHOD_BLUETOOTH_GET_PAIRED_LIST
};

const char *g_methodModeManagerEventNames[TotalMethodModeManagerEvent] = {
    CHANGE_MODE,
    RELEASE_RESOURCE_DONE,
    END_MODE,
    MODE_ERROR_OCCURED,
	SUSPEND,
	RESUME
};

const char *g_signalModeManagerEventNames[TotalSignalModeManagerEvent] = {
    CHANGED_MODE,
    RELEASE_RESOURCE,
    ENDED_MODE,
	SUSPEND_MODE,
	RESUME_MODE
};

/* End of file */
