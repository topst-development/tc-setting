/****************************************************************************************
 *   FileName    : DBusMsgDef.h
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

#ifndef DBUS_MSG_DEF_H
#define DBUS_MSG_DEF_H

/*================================================ LAUNCHER PROCESS DEFINE START ================================================*/

#define LAUNCHER_PROCESS_DBUS_NAME					"telechips.launcher.process"
#define LAUNCHER_PROCESS_OBJECT_PATH				"/telechips/launcher/process"

#define LAUNCHER_EVENT_INTERFACE					"launcher.event"
// LAUNCHER SIGNAL EVENT DEFINES
#define SIGNAL_LAUNCHER_KEY_PRESSED                 	"signal_launcher_key_pressed"
#define SIGNAL_LAUNCHER_KEY_LONG_PRESSED            	"signal_launcher_key_long_pressed"
#define SIGNAL_LAUNCHER_KEY_LONG_LONG_PRESSED   "signal_launcher_key_long_long_pressed"
#define SIGNAL_LAUNCHER_KEY_RELEASED                	"signal_launcher_key_released"
#define SIGNAL_LAUNCHER_KEY_CLICKED                 		"signal_launcher_key_clicked"
#define SIGNAL_LAUNCHER_ENABLE_KEY_EVENT           	"signal_launcher_enable_key_event"
#define SIGNAL_LAUNCHER_DISABLE_KEY_EVENT           	"signal_launcher_disable_key_event"

typedef enum {
    SignalLauncherKeyPressed,
    SignalLauncherKeyLongPressed,
    SignalLauncherKeyLongLongPressed,
    SignalLauncherKeyReleased,
    SignalLauncherKeyClicked,
    SignalLauncherEnableKeyEvent,
    SignalLauncherDisableKeyEvent,
    TotalSignalLauncherEvents
} SignalLauncherEvent;
extern const char *g_signalLauncherEventNames[TotalSignalLauncherEvents];


// LAUNCHER METHOD EVENT DEFINES
#define METHOD_LAUNCHER_ACTIVE_APPLICATION          "method_launcher_active_application"
#define METHOD_LAUNCHER_START_APPLICATIONS          "method_launcher_start_applications"
#define METHOD_LAUNCHER_PREEMPT_KEY_EVENT           "method_launcher_preempt_key_event"
#define METHOD_LAUNCHER_RELEASE_PREEMPT_KEY_EVENT   "method_launcher_release_preempt_key_event"

typedef enum {
    MethodLauncherActiveApplication,
    MethodLauncherStartApplications,
    MethodLauncherPreemptKeyEvent,
    MethodLauncherReleasePreemptKeyEvent,
    TotalMethodLauncherEvents
} MethodLauncherEvent;
extern const char *g_methodLauncherEventNames[TotalMethodLauncherEvents];


/*================================================ LAUNCHER PROCESS DEFINE END ================================================*/

/*================================================ AUDIO MANAGER PROCESS DEFINE START ================================================*/

#define AUDIOMANAGER_PROCESS_DBUS_NAME                  "org.genivi.audiomanager"
#define AUDIOMANAGER_PROCESS_OBJECT_PATH                "/org/genivi/audiomanager/commandinterface"
#define AUDIOMANAGER_EVENT_INTERFACE                    "org.genivi.audiomanager.commandinterface"

#define SIGNAL_AUDIOMANAGER_VOLUME_CHANGED              "SourceVolumeChanged"

typedef enum {
    SignalAudioManagerVolumeChanged,
    TotalSignalAudioManagerEvents
} SignalAudioManagerEvent;
extern const char *g_signalAudioManagerEventNames[TotalSignalAudioManagerEvents];


#define METHOD_AUDIOMANAGER_GET_VOLUME          "GetSourceVolume"
#define METHOD_AUDIOMANAGER_SET_VOLUME               "SetSourceVolume"

typedef enum {
    MethodAudioManagerGetVolume,
    MethodAudioManagerSetVolume,
    TotalMethodAudioManagerEvents
} MethodAudioManagerEvent;
extern const char *g_methodAudioManagerEventNames[TotalMethodAudioManagerEvents];

/*================================================ AUDIO MANAGER DEFINE END ================================================*/


/*============================ BLUETOOTH APPLICATION PROCESS DEFINE START ============================*/

#define BLUETOOTH_APPLICATION_DBUS_NAME                 "telechips.bluetooth.application"
#define BLUETOOTH_APPLICATION_OBJECT_PATH				"/telechips/bluetooth/application"
#define BLUETOOTH_APPLICATION_EVENT_INTERFACE			"bluetoothapplication.event"

/* BT Setting */
#define SIGNAL_BLUETOOTH_BT_ON_OFF						"signal_bluetooth_bt_on_off"
#define SIGNAL_BLUETOOTH_LOCAL_INFO						"signal_bluetooth_local_info"
/* BT Search */
#define SIGNAL_BLUETOOTH_SEARCH_RESULT					"signal_bluetooth_search_result"
#define SIGNAL_BLUETOOTH_SEARCH_COMPLETE				"signal_bluetooth_search_complete"
/* BT Connect */
#define SIGNAL_BLUETOOTH_CONNECTED						"signal_bluetooth_connected"
#define SIGNAL_BLUETOOTH_DISCONNECTED					"signal_bluetooth_disconnected"
#define SIGNAL_BLUETOOTH_PROFILE_STATUS					"signal_bluetooth_profile_status"
#define SIGNAL_BLUETOOTH_PAIRED_DEVICE_LIST				"signal_bluetooth_paired_device_list" 
#define SIGNAL_BLUETOOTH_PAIRED_DEVICE_INFO				"signal_bluetooth_paired_device_info" 


typedef enum {
	SignalBluetoothBtOnOff,
	SignalBluetoothLocalInfo,
	SignalBluetoothSearchResult,
	SignalBluetoothSearchComplete,
	SignalBluetoothConnected,
	SignalBluetoothDisconnected,
	SignalBluetoothProfileStatus,
	SignalBluetoothPairedDeviceList,
	SignalBluetoothPairedDeviceInfo,
	TotalSignalBluetoothApplicationEvents
} SignalBluetoothApplicationEvent;
extern const char *g_signalBluetoothApplicationEventNames[TotalSignalBluetoothApplicationEvents];

/* BT Setting */
#define METHOD_BLUETOOTH_GET_STATUS						"method_bluetooth_bt_get_status"
#define METHOD_BLUETOOTH_BT_ON_OFF						"method_bluetooth_bt_on_off"
#define METHOD_BLUETOOTH_GET_LOCAL_INFO					"method_bluetooth_get_local_info"

/* BT Search */
#define METHOD_BLUETOOTH_DISCOVERABLE					"method_bluetooth_discoverable"
#define METHOD_BLUETOOTH_SEARCH							"method_bluetooth_search"
/* BT Connect */
#define METHOD_BLUETOOTH_CONNECT						"method_bluetooth_connect"
#define METHOD_BLUETOOTH_DISCONNECT						"method_bluetooth_disconnect"
#define METHOD_BLUETOOTH_DELETE							"method_bluetooth_delete"

/* BT Paired Device */
#define METHOD_BLUETOOTH_GET_PAIRED_LIST_COUNT			"method_bluetooth_get_paired_list_count"
#define METHOD_BLUETOOTH_GET_PAIRED_LIST				"method_bluetooth_get_paired_list"
#define METHOD_BLUETOOTH_GET_PAIRED_DEVICE_INFO			"method_bluetooth_get_paired_device_info"


typedef enum {
	MethodBluetoothGetStatus,
	MethodBluetoothBtOnOff,
	MethodBluetoothGetLocalInfo,
	MethodBluetoothDiscoverable,
	MethodBluetoothSearch,
	MethodBluetoothConnect,
	MethodBluetoothDisconnect,
	MethodBluetoothDelete,
	MethodBluetoothGetPairedListCount,
	MethodBluetoothGetPairedList,
	MethodBluetoothGetPairedDeviceInfo,	
    TotalMethodBluetoothApplicationEvents
} MethodBluetoothApplicationEvent;
extern const char *g_methodBluetoothApplicationEventNames[TotalMethodBluetoothApplicationEvents];

/*============================ BLUETOOTH APPLICATION PROCESS DEFINE END ============================*/

/*============================ Mode Manager D-Bus Define End ============================*/

#define MODEMANAGER_PROCESS_DBUS_NAME                   "telechips.mode.manager"
#define MODEMANAGER_PROCESS_OBJECT_PATH                 "/telechips/mode/manager"
#define MODEMANAGER_EVENT_INTERFACE                     "mode.manager"
 
 
/********************************METHOD*************************************************/
#define CHANGE_MODE                                     "change_mode"
#define RELEASE_RESOURCE_DONE                           "release_resource_done"
#define END_MODE                                        "end_mode"
#define MODE_ERROR_OCCURED                              "mode_error_occured"
#define SUSPEND                                         "suspend"
#define RESUME                                          "resume"

typedef enum{
    Change_Mode,
    Release_Resource_Done,
    End_Mode,
    Mode_Error_Occured,
	Suspend,
	Resume,
    TotalMethodModeManagerEvent
}MethodModeManagerEvent;
extern const char* g_methodModeManagerEventNames[TotalMethodModeManagerEvent];
 
/********************************SIGNAL*************************************************/
#define CHANGED_MODE                                    "changed_mode"
#define RELEASE_RESOURCE                                "release_resource"
#define ENDED_MODE                                      "ended_mode"
#define SUSPEND_MODE                                    "suspend_mode"
#define RESUME_MODE                                     "resume_mode"

typedef enum{
    Changed_Mode,
    Release_Resource,
    Ended_Mode,
	SuspendMode,
	ResumeMode,
    TotalSignalModeManagerEvent
}SignalModeManagerEvent;
extern const char* g_signalModeManagerEventNames[TotalSignalModeManagerEvent];

/*============================ Mode Manager D-Bus Define End ============================*/

#endif // DBUS_MSG_DEF_H

