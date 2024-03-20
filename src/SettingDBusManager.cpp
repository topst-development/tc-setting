/****************************************************************************************
 *   FileName    : EQDBusManager.cpp
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <dbus/dbus.h>
#include "DBusMsgDef.h"
#include "TCDBusRawAPI.h"
#include "SettingIFManager.h"

//#include "BluetoothSetting.h"

extern int g_debug;
extern int g_appID;
//extern BluetoothSetting *g_btSetting;

#define SETTING_DBUS_MANAGER_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[SETTING DBUS MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

static DBusMsgErrorCode OnReceivedDBusSignal(DBusMessage *message, const char *interface);
static void LauncherSignalDBusProcess(unsigned int id, DBusMessage *message);
static void AudioManagerSignalDBusProcess(unsigned int id, DBusMessage *message);
#ifdef ENABLE_BT
static void BluetoothSettingSignalDBusProcess(unsigned int id, DBusMessage *message);
#endif
static void ModeManagerSignalDBusProcess(unsigned int id, DBusMessage *message);

void InitilaizeSettingDBusManager(void)
{
	SetCallBackFunctions(OnReceivedDBusSignal, NULL);
	AddSignalInterface(LAUNCHER_EVENT_INTERFACE);
	AddSignalInterface(AUDIOMANAGER_EVENT_INTERFACE);
	AddSignalInterface(BLUETOOTH_APPLICATION_EVENT_INTERFACE);
	AddSignalInterface(MODEMANAGER_EVENT_INTERFACE);
	InitializeRawDBusConnection("SETTING DBUS");
}

void ReleaseSettingDBusManager(void)
{
	ReleaseRawDBusConnection();
}

void SendDBusSetVolume(unsigned short sourceID, short volume)
{
    SETTING_DBUS_MANAGER_PRINTF("sourceID : %hu setVolume : %hd \n", sourceID, volume);
    DBusMessage* message;
    short error;
    message = CreateDBusMsgMethodCall(AUDIOMANAGER_PROCESS_DBUS_NAME,AUDIOMANAGER_PROCESS_OBJECT_PATH,
                                    AUDIOMANAGER_EVENT_INTERFACE,
                                    METHOD_AUDIOMANAGER_SET_VOLUME,
                                    DBUS_TYPE_UINT16, &sourceID,
                                    DBUS_TYPE_INT16, &volume,
                                    DBUS_TYPE_INVALID);
    if(message != NULL)
    {
        DBusPendingCall* pending = NULL;
        if(SendDBusMessage(message, &pending))
        {
            if(pending != NULL)
            {
                if(GetArgumentFromDBusPendingCall(pending,
                                                    DBUS_TYPE_INT16, &error,
                                                    DBUS_TYPE_INVALID))
                {
                    if(!error)
                    {
                        SETTING_DBUS_MANAGER_PRINTF("GetArgmentFromDBusPendingCall, volume set Complete \n");
                    }
                }
                else
                {
                    fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
                }
                dbus_pending_call_unref(pending);
            }
        }
    }
    else
    {
        fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
    }

}

void SendDBusGetVolumeStep(unsigned short sourceID)
{
    SETTING_DBUS_MANAGER_PRINTF("\n");
    DBusMessage* message;
    short volume;
    short error;

    message = CreateDBusMsgMethodCall(AUDIOMANAGER_PROCESS_DBUS_NAME,AUDIOMANAGER_PROCESS_OBJECT_PATH,
                                    AUDIOMANAGER_EVENT_INTERFACE,
                                    METHOD_AUDIOMANAGER_GET_VOLUME,
                                    DBUS_TYPE_UINT16, &sourceID,
                                    DBUS_TYPE_INVALID);

    if(message != NULL)
    {
        DBusPendingCall* pending = NULL;
        if(SendDBusMessage(message, &pending))
        {
            if(pending != NULL)
            {
                if(GetArgumentFromDBusPendingCall(pending,
                                                    DBUS_TYPE_INT16, &error,
                                                    DBUS_TYPE_INT16, &volume,
                                                    DBUS_TYPE_INVALID))
                {
                    if(!error)
                    {
                        SETTING_IF_MANAGER->EmitAudioVolumeChagned(sourceID, volume);
                    }
                }
                else
                {
                    fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
                }
                dbus_pending_call_unref(pending);
            }
        }
        else
        {
            fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
        }
        dbus_message_unref(message);
    }
    else
    {
        fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
    }
}

int SendDBusChangeMode(const char* mode, int app)
{
	DBusMessage *message;
	int ack = NACK;

	SETTING_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(MODEMANAGER_PROCESS_DBUS_NAME,
										MODEMANAGER_PROCESS_OBJECT_PATH,
										MODEMANAGER_EVENT_INTERFACE,
										CHANGE_MODE,
										DBUS_TYPE_STRING, &mode,
										DBUS_TYPE_INT32, &app,
										DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		DBusPendingCall *pending = NULL;
		if (SendDBusMessage(message, &pending))
		{
			if(pending != NULL)
			{
				if(GetArgumentFromDBusPendingCall(pending,
										DBUS_TYPE_INT32, &ack,
										DBUS_TYPE_INVALID))
				{
					SETTING_DBUS_MANAGER_PRINTF("reply : %s\n", ack ? "ACK" : "NACK");
				}
				else
				{
					fprintf(stderr, "%s: GetArgumentFromDBusPendingCall failed\n", __FUNCTION__);
				}
				dbus_pending_call_unref(pending);
			}
		}
		else
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
	return ack;
}

void SendDBusReleaseResourceDone(int resource, int app)
{
	DBusMessage *message;
	SETTING_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(MODEMANAGER_PROCESS_DBUS_NAME,
										MODEMANAGER_PROCESS_OBJECT_PATH,
										MODEMANAGER_EVENT_INTERFACE,
										RELEASE_RESOURCE_DONE,
										DBUS_TYPE_INT32, &resource,
										DBUS_TYPE_INT32, &app,
										DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

#ifdef ENABLE_BT
void SendDBusGetBluetoothStatus(void)
{
    SETTING_DBUS_MANAGER_PRINTF("\n");
    DBusMessage* message;

    message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
										BLUETOOTH_APPLICATION_OBJECT_PATH,
										BLUETOOTH_APPLICATION_EVENT_INTERFACE,
										METHOD_BLUETOOTH_GET_STATUS,
										DBUS_TYPE_INVALID);

	if (message != NULL)
	{
		DBusPendingCall* pending = NULL;
		if (SendDBusMessage(message, &pending))
		{
			if (pending != NULL)
			{
				dbus_bool_t status;
				if (GetArgumentFromDBusPendingCall(pending,
							DBUS_TYPE_BOOLEAN, &status,
							DBUS_TYPE_INVALID))
				{
					if(status == TRUE)
					{
						SETTING_IF_MANAGER->EmitBluetoothOnOff(true);
					}
					else
					{
						SETTING_IF_MANAGER->EmitBluetoothOnOff(false);
					}
				}
				else
				{
					fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
				}
				dbus_pending_call_unref(pending);
			}
		}
		else
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}
		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}


void SendDBusBluetoothOnOff(bool operation)
{
	SETTING_DBUS_MANAGER_PRINTF("operation(%d)\n",operation);
    DBusMessage* message;
	dbus_bool_t btOn;

	if(operation == true)
	{
		btOn = TRUE;
	}
	else
	{
		btOn = FALSE;
	}

	message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
										BLUETOOTH_APPLICATION_OBJECT_PATH,
										BLUETOOTH_APPLICATION_EVENT_INTERFACE,
										METHOD_BLUETOOTH_BT_ON_OFF,
										DBUS_TYPE_BOOLEAN, &btOn,
										DBUS_TYPE_INVALID);

    if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}

}

void SendDBusGetBluetoothLocalInfo(void)
{
    SETTING_DBUS_MANAGER_PRINTF("\n");
    DBusMessage* message;

        message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
											BLUETOOTH_APPLICATION_OBJECT_PATH,
											BLUETOOTH_APPLICATION_EVENT_INTERFACE,
											METHOD_BLUETOOTH_GET_LOCAL_INFO,
											DBUS_TYPE_INVALID);

    if (message != NULL)
    {
        DBusPendingCall* pending = NULL;
        if (SendDBusMessage(message, &pending))
        {
            if (pending != NULL)
            {
                char *address = NULL;
                char *name = NULL;
                if (GetArgumentFromDBusPendingCall(pending,
                                                    DBUS_TYPE_STRING, &address,
                                                    DBUS_TYPE_STRING, &name,
                                                    DBUS_TYPE_INVALID))
                {
                    if (address != NULL && name != NULL)
                    {
                        SETTING_IF_MANAGER->EmitBluetoothLocalInfo(address, name);
                    }
                }
                else
                {
                    fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
                }
                dbus_pending_call_unref(pending);
            }
        }
        else
        {
            fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
        }
        dbus_message_unref(message);
    }
    else
    {
        fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
    }
}

void SendDBusBluetoothDiscoverable(bool operation)
{
	SETTING_DBUS_MANAGER_PRINTF("\n");
    DBusMessage* message;
	dbus_bool_t disCoverable;

	if(operation == true)
	{
		disCoverable = TRUE;
	}
	else
	{
		disCoverable = FALSE;
	}

	message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
										BLUETOOTH_APPLICATION_OBJECT_PATH,
										BLUETOOTH_APPLICATION_EVENT_INTERFACE,
										METHOD_BLUETOOTH_DISCOVERABLE,
										DBUS_TYPE_BOOLEAN, &disCoverable,
										DBUS_TYPE_INVALID);

    if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusBluetoothSearch(bool operation)
{
	SETTING_DBUS_MANAGER_PRINTF("\n");
    DBusMessage* message;
	dbus_bool_t searchOn;

	if(operation == true)
	{
		searchOn = TRUE;
	}
	else
	{
		searchOn = FALSE;
	}

	message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
										BLUETOOTH_APPLICATION_OBJECT_PATH,
										BLUETOOTH_APPLICATION_EVENT_INTERFACE,
										METHOD_BLUETOOTH_SEARCH,
										DBUS_TYPE_BOOLEAN, &searchOn,
										DBUS_TYPE_INVALID);

    if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusBluetoothConnect(const char *deviceAddr)
{
	SETTING_DBUS_MANAGER_PRINTF("\n");
    DBusMessage* message;

	message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
										BLUETOOTH_APPLICATION_OBJECT_PATH,
										BLUETOOTH_APPLICATION_EVENT_INTERFACE,
										METHOD_BLUETOOTH_CONNECT,
										DBUS_TYPE_STRING, &deviceAddr,
										DBUS_TYPE_INVALID);

    if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusBluetoothDisconnect(const char *deviceAddr)
{
	DBusMessage *message;
	SETTING_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
									  BLUETOOTH_APPLICATION_OBJECT_PATH,
									  BLUETOOTH_APPLICATION_EVENT_INTERFACE,
									  METHOD_BLUETOOTH_DISCONNECT,
									  DBUS_TYPE_STRING, &deviceAddr,
									  DBUS_TYPE_INVALID);

	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusBluetoothDelete(const char *deviceAddr)
{
	DBusMessage *message;
	SETTING_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
									  BLUETOOTH_APPLICATION_OBJECT_PATH,
									  BLUETOOTH_APPLICATION_EVENT_INTERFACE,
									  METHOD_BLUETOOTH_DELETE,
									  DBUS_TYPE_STRING, &deviceAddr,
									  DBUS_TYPE_INVALID);

	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusBluetoothGetPairedListCount(void)
{
	SETTING_DBUS_MANAGER_PRINTF("\n");
	DBusMessage* message;

	message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
										BLUETOOTH_APPLICATION_OBJECT_PATH,
										BLUETOOTH_APPLICATION_EVENT_INTERFACE,
										METHOD_BLUETOOTH_GET_PAIRED_LIST_COUNT,
										DBUS_TYPE_INVALID);

	if (message != NULL)
	{
		DBusPendingCall* pending = NULL;
		if (SendDBusMessage(message, &pending))
		{
			if (pending != NULL)
			{
				int32_t totalNum;
				if (GetArgumentFromDBusPendingCall(pending,
													DBUS_TYPE_INT32, &totalNum,
													DBUS_TYPE_INVALID))
				{
					SETTING_IF_MANAGER->EmitBluetoothPairedListCount((uint32_t)totalNum);
				}
				else
				{
					fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
				}
				dbus_pending_call_unref(pending);
			}
		}
		else
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}
		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusBluetoothGetPairedList(uint32_t startIndex, uint32_t endIndex)
{
	DBusMessage *message;
	SETTING_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
									  BLUETOOTH_APPLICATION_OBJECT_PATH,
									  BLUETOOTH_APPLICATION_EVENT_INTERFACE,
									  METHOD_BLUETOOTH_GET_PAIRED_LIST,
									  DBUS_TYPE_UINT32, &startIndex,
									  DBUS_TYPE_UINT32, &endIndex,
									  DBUS_TYPE_INVALID);

	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
		}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusBluetoothGetPairedDeviceInfo(uint32_t index)
{
	SETTING_DBUS_MANAGER_PRINTF("\n");
	DBusMessage* message;

	message = CreateDBusMsgMethodCall(BLUETOOTH_APPLICATION_DBUS_NAME,
										BLUETOOTH_APPLICATION_OBJECT_PATH,
										BLUETOOTH_APPLICATION_EVENT_INTERFACE,
										METHOD_BLUETOOTH_GET_PAIRED_DEVICE_INFO,
										DBUS_TYPE_UINT32, &index,
										DBUS_TYPE_INVALID);

	if (message != NULL)
	{
		if (message != NULL)
		{
			if (!SendDBusMessage(message, NULL))
			{
				fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
			}
		
			dbus_message_unref(message);
			}
		else
		{
			fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
		}
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}
#endif

static DBusMsgErrorCode OnReceivedDBusSignal(DBusMessage *message, const char *interface)
{
	DBusMsgErrorCode error = ErrorCodeNoError;
	//SETTING_DBUS_MANAGER_PRINTF("\n");

	if (message != NULL &&
		interface != NULL)
	{
		unsigned int index;
		unsigned int stop = 0;

		if (strcmp(interface, LAUNCHER_EVENT_INTERFACE) == 0)
		{
			for (index = SignalLauncherKeyPressed; index < TotalSignalLauncherEvents && !stop; index++)
			{
				if (dbus_message_is_signal(message,
										   LAUNCHER_EVENT_INTERFACE,
										   g_signalLauncherEventNames[index]))
				{
					LauncherSignalDBusProcess(index, message);
					stop = 1;
				}
			}
		}
		else if(strcmp(interface,  AUDIOMANAGER_EVENT_INTERFACE) == 0)
		{
			for (index = SignalAudioManagerVolumeChanged; index < TotalSignalAudioManagerEvents && !stop; index++)
			{
				if (dbus_message_is_signal(message,
										   AUDIOMANAGER_EVENT_INTERFACE,
										   g_signalAudioManagerEventNames[index]))
				{
					AudioManagerSignalDBusProcess(index, message);
					stop = 1;
				}
			}
		}
#ifdef ENABLE_BT
		else if(strcmp(interface, BLUETOOTH_APPLICATION_EVENT_INTERFACE) == 0)
		{
			for (index = SignalBluetoothBtOnOff; index < TotalSignalBluetoothApplicationEvents && !stop; index++)
			{
				if (dbus_message_is_signal(message,
										   BLUETOOTH_APPLICATION_EVENT_INTERFACE,
										   g_signalBluetoothApplicationEventNames[index]))
				{
					BluetoothSettingSignalDBusProcess(index, message);
					stop = 1;
				}
			}
		}
#endif
		else if (strcmp(interface, MODEMANAGER_EVENT_INTERFACE) == 0)
		{
			for (index = Changed_Mode; index < TotalSignalModeManagerEvent && !stop; index++)
			{
				if (dbus_message_is_signal(message, MODEMANAGER_EVENT_INTERFACE,
											g_signalModeManagerEventNames[index]))
				{
					ModeManagerSignalDBusProcess(index, message);
					stop = 1;
				}
			}
		}
		
		if (!stop)
		{
			error = ErrorCodeUnknown;
		}
	}

	return error;
}

static void LauncherSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalLauncherEvents)
	{
		SETTING_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
									  g_signalLauncherEventNames[id], id);

		if (message != NULL)
		{
			int value = -1;
			if (GetArgumentFromDBusMessage(message,
										   DBUS_TYPE_INT32, &value,
										   DBUS_TYPE_INVALID))
			{
				switch (id)
				{
					case SignalLauncherKeyPressed:
						SETTING_IF_MANAGER->EmitKeyboardPressedEvent(value);
						break;
					case SignalLauncherKeyLongPressed:
						SETTING_IF_MANAGER->EmitKeyboardLongPressedEvent(value);
						break;
					case SignalLauncherKeyLongLongPressed:
						SETTING_IF_MANAGER->EmitKeyboardLongLongPressedEvent(value);
						break;
					case SignalLauncherKeyReleased:
						SETTING_IF_MANAGER->EmitKeyboardReleasedEvent(value);
						break;
					case SignalLauncherKeyClicked:
						SETTING_IF_MANAGER->EmitKeyboardClickedEvent(value);
						break;
					case SignalLauncherEnableKeyEvent:
						SETTING_IF_MANAGER->EmitLauncherEnableKeyEvent(value);
						break;
					case SignalLauncherDisableKeyEvent:
						SETTING_IF_MANAGER->EmitLauncherDisableKeyEvent(value);
						break;
					default:
						fprintf(stderr, "%s: unknown signal id(%d)\n", __FUNCTION__, id);
						break;
				}
			}
			else
			{
				fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __FUNCTION__);
			}
		}
	}
	else
	{
		SETTING_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%d]\n",
									  id);
	}
}

static void AudioManagerSignalDBusProcess(unsigned int id, DBusMessage * message)
{
    if(id < TotalSignalAudioManagerEvents)
    {
        SETTING_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
                                      g_signalAudioManagerEventNames[id], id);
        if (message != NULL)
        {
			if (id == SignalAudioManagerVolumeChanged)
			{
				unsigned short sourceID = 0;
				short volume = -1;
				if (GetArgumentFromDBusMessage(message,
											   DBUS_TYPE_UINT16, &sourceID,
											   DBUS_TYPE_INT16, &volume,
											   DBUS_TYPE_INVALID))
				{
					SETTING_IF_MANAGER->EmitAudioVolumeChagned(sourceID, volume);
				}
				else
				{
					fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __FUNCTION__);
				}
			}
        }
    }
}

#ifdef ENABLE_BT
static void BluetoothSettingSignalDBusProcess(unsigned int id, DBusMessage * message)
{
    if(id < TotalSignalBluetoothApplicationEvents)
    {
        SETTING_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
                                      g_signalBluetoothApplicationEventNames[id], id);
        if (message != NULL)
        {
			dbus_bool_t operation;
			char *addr;
			char *name;
			char **names;
			uint32_t u32_arg1, u32_arg2, u32_arg3, u32_arg4, u32_arg5, u32_arg6, u32_arg7, u32_arg8;
			int8_t i8_arg;

			switch(id)
			{
				case SignalBluetoothBtOnOff:
				{
					if (GetArgumentFromDBusMessage(message,
												DBUS_TYPE_BOOLEAN, &operation,
												DBUS_TYPE_INVALID))
					{
						SETTING_DBUS_MANAGER_PRINTF("SIGNAL(%s) : %s\n", SIGNAL_BLUETOOTH_BT_ON_OFF,
													(operation == TRUE) ? "On" : "Off");
						if(operation == TRUE)
						{
							SETTING_IF_MANAGER->EmitBluetoothOnOff(true);
						}
						else
						{
							SETTING_IF_MANAGER->EmitBluetoothOnOff(false);
						}
					}
					else
					{
						fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", SIGNAL_BLUETOOTH_BT_ON_OFF);
					}
				}
					break;
				case SignalBluetoothLocalInfo:
					if (GetArgumentFromDBusMessage(message,
												DBUS_TYPE_STRING, &addr,
												DBUS_TYPE_STRING, &name,
												DBUS_TYPE_INVALID))
					{
						SETTING_DBUS_MANAGER_PRINTF("SIGNAL(%s) : %s, %s\n", SIGNAL_BLUETOOTH_LOCAL_INFO, addr, name);
						SETTING_IF_MANAGER->EmitBluetoothLocalInfo(addr, name);
					}
					else
					{
						fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", SIGNAL_BLUETOOTH_LOCAL_INFO);
					}
					break;
				case SignalBluetoothSearchResult:
					if (GetArgumentFromDBusMessage(message,
												DBUS_TYPE_UINT32, &u32_arg1,
												DBUS_TYPE_STRING, &addr,
												DBUS_TYPE_STRING, &name,
												DBUS_TYPE_BYTE, &i8_arg,
												DBUS_TYPE_INVALID))
					{
						SETTING_DBUS_MANAGER_PRINTF("SIGNAL(%s) : Class(%u), %s, %s, RSSI(%d)\n",
												SIGNAL_BLUETOOTH_SEARCH_RESULT,
												u32_arg1, addr, name, i8_arg);
						SETTING_IF_MANAGER->EmitBluetoothSearchResult(u32_arg1, addr, name, i8_arg);
					}
					else
					{
						fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", SIGNAL_BLUETOOTH_SEARCH_RESULT);
					}
					break;
				case SignalBluetoothSearchComplete:
					SETTING_DBUS_MANAGER_PRINTF("SIGNAL(%s)\n", SIGNAL_BLUETOOTH_SEARCH_COMPLETE);
					SETTING_IF_MANAGER->EmitBluetoothSearchComplete();
					break;
				case SignalBluetoothConnected:
					if (GetArgumentFromDBusMessage(message,
												DBUS_TYPE_STRING, &addr,
												DBUS_TYPE_STRING, &name,
												DBUS_TYPE_INVALID))
					{
						SETTING_DBUS_MANAGER_PRINTF("SIGNAL(%s) : %s, %s\n", SIGNAL_BLUETOOTH_CONNECTED, addr, name);
						SETTING_IF_MANAGER->EmitBluetoothConnected(addr, name);
					}
					else
					{
						fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", SIGNAL_BLUETOOTH_CONNECTED);
					}
					break;
				case SignalBluetoothDisconnected:
					if (GetArgumentFromDBusMessage(message,
												DBUS_TYPE_STRING, &addr,
												DBUS_TYPE_INVALID))
					{
						SETTING_DBUS_MANAGER_PRINTF("SIGNAL(%s) : %s\n", SIGNAL_BLUETOOTH_DISCONNECTED, addr);
						SETTING_IF_MANAGER->EmitBluetoothDisConnected(addr);
					}
					else
					{
						fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", SIGNAL_BLUETOOTH_DISCONNECTED);
					}
					break;
				case SignalBluetoothProfileStatus:
					if (GetArgumentFromDBusMessage(message,
												DBUS_TYPE_STRING, &addr,
												DBUS_TYPE_UINT32, &u32_arg1, /* HF*/
												DBUS_TYPE_UINT32, &u32_arg2, /* A2DP */
												DBUS_TYPE_UINT32, &u32_arg3, /* AVRCP */
												DBUS_TYPE_UINT32, &u32_arg4, /* PBAP */
												DBUS_TYPE_UINT32, &u32_arg5, /* MAP */
												DBUS_TYPE_UINT32, &u32_arg6, /* HIDD */
												DBUS_TYPE_UINT32, &u32_arg7, /* PAN */
												DBUS_TYPE_INVALID))
					{
						SETTING_DBUS_MANAGER_PRINTF("SIGNAL(%s) : ADDR(%s)\n", SIGNAL_BLUETOOTH_PROFILE_STATUS, addr);
						SETTING_DBUS_MANAGER_PRINTF("\t HF(%u), A2DP(%u), AVRCP(%u), PBAP(%u)\n", u32_arg1, u32_arg2, u32_arg3, u32_arg4);
						SETTING_DBUS_MANAGER_PRINTF("\t MAP(%u), HIDD(%u), PAN(%u)\n", u32_arg5, u32_arg6, u32_arg7);
						SETTING_IF_MANAGER->EmitBluetoothProfileStatus(addr, u32_arg1, u32_arg2, u32_arg3, u32_arg4, u32_arg5, u32_arg6, u32_arg7);
					}
					else
					{
						fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", SIGNAL_BLUETOOTH_PROFILE_STATUS);
					}
					break;
				case SignalBluetoothPairedDeviceList:
					if (GetArgumentFromDBusMessage(message,
												DBUS_TYPE_UINT32, &u32_arg1,
												DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &names, &u32_arg2,
												DBUS_TYPE_INVALID))
					{
						for(u32_arg3 = 0; u32_arg3 < u32_arg2; u32_arg3++)
						{
							SETTING_IF_MANAGER->EmitBluetoothPairedDeviceList(u32_arg3, names[u32_arg3]);
						}
					}
					break;
				case SignalBluetoothPairedDeviceInfo:
					if (GetArgumentFromDBusMessage(message,
												DBUS_TYPE_UINT32, &u32_arg1, /* Class */
												DBUS_TYPE_STRING, &addr,
												DBUS_TYPE_STRING, &name,
												DBUS_TYPE_UINT32, &u32_arg2, /* HF */
												DBUS_TYPE_UINT32, &u32_arg3, /* A2DP */
												DBUS_TYPE_UINT32, &u32_arg4, /* AVRCP */
												DBUS_TYPE_UINT32, &u32_arg5, /* PBAP */
												DBUS_TYPE_UINT32, &u32_arg6, /* MAP */
												DBUS_TYPE_UINT32, &u32_arg7, /* HIDD */
												DBUS_TYPE_UINT32, &u32_arg8, /* PAN */
												DBUS_TYPE_INVALID))
					{
						SETTING_DBUS_MANAGER_PRINTF("SIGNAL(%s) : class(%u) ADDR(%s) Name(%s)\n", SIGNAL_BLUETOOTH_PAIRED_DEVICE_LIST, u32_arg1, addr, name);
						SETTING_DBUS_MANAGER_PRINTF("\t HF(%u), A2DP(%u), AVRCP(%u), PBAP(%u)\n", u32_arg2, u32_arg3, u32_arg4, u32_arg5);
						SETTING_DBUS_MANAGER_PRINTF("\t MAP(%u), HIDD(%u), PAN(%u)\n", u32_arg6, u32_arg7, u32_arg8);

						SETTING_IF_MANAGER->EmitBluetoothPairedDeviceInfo(u32_arg1, addr, name, u32_arg2, u32_arg3, 
																			u32_arg4, u32_arg5, u32_arg6, u32_arg7, u32_arg8);

					}
					else
					{
						fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", SIGNAL_BLUETOOTH_PAIRED_DEVICE_LIST);
					}

					break;
				default:
					SETTING_DBUS_MANAGER_PRINTF("BT RECEIVED UNKNOWN SIGNAL[%u]\n", id);
					break;
			}
        }
    }
}
#endif

static void ModeManagerSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalModeManagerEvent)
	{
		SETTING_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n", g_signalModeManagerEventNames[id], id);
		if(message != NULL)
		{
			if(id == Changed_Mode)
			{
				char *mode;
				int app;
				if(GetArgumentFromDBusMessage(message,
											DBUS_TYPE_STRING, &mode,
											DBUS_TYPE_INT32, &app,
											DBUS_TYPE_INVALID))
				{
					if(app == g_appID)
					{
						SETTING_DBUS_MANAGER_PRINTF("Mode : %s, appID : %d\n", mode, app);
						SETTING_IF_MANAGER->EmitModeManagerChangedMode(mode, app);
					}
				}
			}
			else if(id == SuspendMode)
			{
				if(GetArgumentFromDBusMessage(message,
											DBUS_TYPE_INVALID))
				{
						SETTING_DBUS_MANAGER_PRINTF("SuspendMode\n");
						SETTING_IF_MANAGER->EmitModeManagerSuspendMode();
				}
			}
			else if(id == ResumeMode)
			{
				if(GetArgumentFromDBusMessage(message,
											DBUS_TYPE_INVALID))
				{
						SETTING_DBUS_MANAGER_PRINTF("ResumeMode\n");
						SETTING_IF_MANAGER->EmitModeManagerResumeMode();
				}
			}
			else if(id == Release_Resource)
			{
				int resource;
				int app;
				if(GetArgumentFromDBusMessage(message,
											DBUS_TYPE_INT32, &resource,
											DBUS_TYPE_INT32, &app,
											DBUS_TYPE_INVALID))
				{
					if(app == g_appID)
					{
						SETTING_DBUS_MANAGER_PRINTF("Release Resource : 0x%x, appID : %d\n", resource, app);
						SETTING_IF_MANAGER->EmitModeManagerReleaseResource(resource, app);
					}
				}
			}
			else if(id == Ended_Mode)
			{
				SETTING_DBUS_MANAGER_PRINTF("Ended Mode Signal ID : %d\n", id);
			}
			else
			{
				SETTING_DBUS_MANAGER_PRINTF("Unknown Signal ID : %d\n", id);
			}
		}
		else
		{
			fprintf(stderr, "%s: Error - message is NULL\n", __FUNCTION__);
		}
	}
	else
	{
		fprintf(stderr, "%s: Unknown received signal[%d]\n", __FUNCTION__, id);
	}
}

