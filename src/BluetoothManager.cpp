/****************************************************************************************
 *   FileName    : BluetoothManager.cpp
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
#include <QObject>
#include "Setting.h"
#include "SettingDBusManager.h"
#include "SettingIFManager.h"
#include "BluetoothManager.h"

extern int g_debug;

#define BLUETOOTH_MANAGER_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[BLUETOOTH_SETTING] %s: " format "", __FUNCTION__, ##arg); \
	}

#define COD_COMPUTER_MAX		9
#define COD_PHONE_MAX		7
#define COD_AV_MAX		20
#define COD_PERI_MAX		10
#define COD_PERI_UP_MAX		5
#define COD_IMAGING_MAX		17
#define COD_WEARABLE_MAX		7
#define COD_TOY_MAX			7
#define COD_HEALTH_MAX			8
#define COLOR_GREEN		QColor(0x0, 0xB4, 0x0)
#define COLOR_BLUE		QColor(0x0, 0x0, 0xB4)
#define COLOR_GRAY		QColor(0x80, 0x80, 0x80)
#define COLOR_LIGHTGRAY	QColor(0xd4, 0xd4, 0xd4)
#define COLOR_WHITE		QColor(0xff, 0xff, 0xff)

typedef enum
{
	Computer = 1,
	Phone = 2,
	LanNetworkAccessPoint = 3,
	AudioVideo = 4,
	Peripheral = 5,
	Imaging = 6,
	Wearable = 7,
	Toy = 8,
	Health = 9,
	Uncategorized = 31
}CodMajorDeviceClass;

static const char *_codComputer[COD_COMPUTER_MAX] = {
	"Uncategorized Computer",
	"Desktop Workstation",
	"Server-class Computer",
	"Laptop",
	"Handheld PC/PDA",
	"Palm sized PC/PDA",
	"Wearable Computer",
	"Tablet",
	"Computer"
};

static const char *_codPhone[COD_PHONE_MAX] = {
	"Uncategorized Phone",
	"Cellular Phone",
	"Cordless Phone",
	"Smart Phone",
	"Wired Modem or Voice Gateway",
	"Common ISDN Gateway",
	"Phone"
};

static const char *_codLAN = "LAN/Network Access Point";

static const char *_codAV[COD_AV_MAX] = {
	"Uncategorized Audio/Video Device",
	"Wearable Headset",
	"Handsfree Device",
	"Reserved",
	"Microphone",
	"Loudspeaker",
	"Headphones",
	"Portable Audio Device",
	"Car Audio Device",
	"Set-top Box",
	"HiFi Audio Device",
	"Video Cassette Recorder",
	"Video Camera",
	"Camcorder",
	"Video Monitor",
	"Video Display and Loudspeaker",
	"Video Conferencing",
	"Reserved",
	"Gaming/Toy",
	"Audio/Video Device"
};

static const char *_codPeripheral[COD_PERI_MAX] = {
	"Peripheral",
	"Joystick",
	"Gamepad",
	"Remote Control",
	"Sensing Device",
	"Digitizer Tablet",
	"Card Reader",
	"Digital Pen",
	"Handheld Scanner",
	"Handheld gestural input Device"
};

static const char *_codPeripheralUp[COD_PERI_UP_MAX] = {
	"Peripheral",
	"Keyboard",
	"Pointing Device",
	"Keyboard and Pointing Device",
	"Peripheral"
};

static const char *_codImaging[COD_IMAGING_MAX] = {
	"Uncategorized Imaging Device",
	"Display",
	"Camera",
	"Display and Camera",
	"Scanner",
	"Display and Scanner",
	"Camera and Scanner",
	"Display, Camera and Scanner",
	"Printer",
	"Display and Printer",
	"Camera and Printer",
	"Display, Camera and Printer",
	"Scanner and Printer",
	"Display, Scanner and Printer",
	"Camera, Scanner and Printer",
	"Display, Camera, Scanner and Printer",
	"Imaging Device"
};

static const char *_codWearable[COD_WEARABLE_MAX] = {
	"Wearable Device",
	"Wrist Watch",
	"Pager",
	"Jacket",
	"Helmet",
	"Glasses",
	"Wearable Device"
};

static const char *_codToy[COD_TOY_MAX] = {
	"Toy",
	"Robot",
	"Vehicle",
	"Action Figure",
	"Controller",
	"Game",
	"Toy"
};

static const char *_codHealth[COD_HEALTH_MAX] = {
	"Undefined",
	"Blood Pressure Monitor",
	"Thermometer",
	"Glucose Meter",
	"Pulse Oximeter",
	"Heart/Rate Monitor",
	"Health Data Display",
	"Health Minor Class",
};

static const char *_codUncategory = "Uncategorized Device";
static const char *_codDefault = "Unspecified Device Class";

typedef enum
{
    BT_PROFILE_STATUS_NOT_SUPPORTED = 0,
    BT_PROFILE_STATUS_SUPPORTED,
    BT_PROFILE_STATUS_CONNECTED,
    BT_PROFILE_STATUS_MAX
} BT_PROFILE_STATUS;

#define INVALID_INDEX	(255)

BluetoothManager::BluetoothManager(QObject *parent) :
	QObject(parent),
	_haveLocalinfo(false),
	_isBTInit(false),
	_isBTOn(false),
	_discoverable(true),
	_searchStart(false),
	_currentSearchIndex(0)
{
	_searchList = new SearchDeviceList();
	_pairedList = new PairedDeviceList();
}

BluetoothManager::~BluetoothManager()
{
	delete _searchList;
	delete _pairedList;
}

void BluetoothManager::Initialize(void)
{
	BLUETOOTH_MANAGER_PRINTF("\n");
	InitializeSignalSlots();
}

void BluetoothManager::prepareBluetooth(void)
{
	BLUETOOTH_MANAGER_PRINTF("\n");

	if (!_haveLocalinfo)
	{
		BLUETOOTH_MANAGER_PRINTF("REQUREST BLUETOOTH LOCAL INFORMATION\n");
		emit RequestBTLocalInfo();
	}

	if(!_isBTInit)
	{
		BLUETOOTH_MANAGER_PRINTF("REQUREST BLUETOOTH Status(On/Off)\n");
		emit RequestBTGetStatus();
	}
}

void BluetoothManager::enableBluetooth(void)
{
	BLUETOOTH_MANAGER_PRINTF("\n");
	if(_isBTOn)
	{
		_isBTOn = false;
	}
	else
	{
		_isBTOn = true;
	}
	emit RequestBTOnOff(_isBTOn);
}

void BluetoothManager::connectDevice(QString addr)
{
	if(addr != nullptr)
	{
		BLUETOOTH_MANAGER_PRINTF("Connect(%s)\n", addr.toLatin1().data());
		emit RequestBTConnect(addr);
	}
}

void BluetoothManager::disconnectPairedDevice(QString addr)
{
	if(addr != nullptr)
	{
		BLUETOOTH_MANAGER_PRINTF("Disconnect(%s)\n", addr.toLatin1().data());
		emit RequestBTDisconnect(addr);
	}
}

void BluetoothManager::deletePairedDevice(QString addr)
{
	int index;
	BLUETOOTH_MANAGER_PRINTF("\n");

	/* Get Current Device Address Infomation */
	index = _pairedList->FindPairedInfoByAddress(addr);
	BLUETOOTH_MANAGER_PRINTF("index(%d)\n", index);
	if(index != INVALID_INDEX)
	{
		_pairedList->RemovePairedDevice(index);
		BLUETOOTH_MANAGER_PRINTF("Delete(%s)\n", addr.toLatin1().data());
		emit pairedListChanged();
		emit RequestBTDelete(addr);
		emit RequestBTGetPairedListCount();
	}
}

bool BluetoothManager::bluetoothInit(void) const
{
	return _isBTInit;
}

bool BluetoothManager::bluetoothOn(void) const
{
	return _isBTOn;
}

QAbstractListModel *BluetoothManager::pairedList(void) const
{
	return (QAbstractListModel *)_pairedList;
}

QAbstractListModel *BluetoothManager::searchList(void) const
{
	return (QAbstractListModel *)_searchList;
}

void BluetoothManager::setDiscoverable(bool discoverable)
{
	BLUETOOTH_MANAGER_PRINTF("\n");

	_discoverable = discoverable;

	emit RequestBTDiscoverable(_discoverable);
}

void BluetoothManager::searchDevice(bool search)
{
	BLUETOOTH_MANAGER_PRINTF("\n");

	_searchStart = search;
	if(_searchStart == true)
	{
		DeleteAllSearchInfo();
	}
	emit RequestBTSearch(_searchStart);
}

void BluetoothManager::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;
	BLUETOOTH_MANAGER_PRINTF("\n");

	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothOnOff(bool)),
			this, SLOT(OnBTOnOff(bool)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothLocalInfo(QString, QString)),
			this, SLOT(OnBTLocalInfo(QString, QString)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothConnected(QString, QString)),
			this, SLOT(OnBTConnected(QString, QString)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothDisConnected(QString)),
			this, SLOT(OnBTDisconnected(QString)), dbusConnectionType);

	connect(this, SIGNAL(RequestBTGetStatus(void)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTGetStatus(void)), dbusConnectionType);
	connect(this, SIGNAL(RequestBTOnOff(bool)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTOnOff(bool)), dbusConnectionType);
	connect(this, SIGNAL(RequestBTLocalInfo(void)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTLocalInfo(void)), dbusConnectionType);

	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothProfileStatus(QString, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)),
			this, SLOT(OnBluetoothProfileStatus(QString, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothPairedListCount(uint32_t)),
			this, SLOT(OnBluetoothPairedListCount(uint32_t)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothPairedDeviceList(uint32_t, QString)),
			this, SLOT(OnBluetoothPairedDeviceList(uint32_t, QString)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothPairedDeviceInfo(uint32_t, QString, QString, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)),
			this, SLOT(OnBluetoothPairedDeviceInfo(uint32_t, QString, QString, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)), dbusConnectionType);

	connect(this, SIGNAL(RequestBTConnect(QString)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTConnect(QString)), dbusConnectionType);
	connect(this, SIGNAL(RequestBTDisconnect(QString)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTDisconnect(QString)), dbusConnectionType);
	connect(this, SIGNAL(RequestBTDelete(QString)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTDelete(QString)), dbusConnectionType);
	connect(this, SIGNAL(RequestBTGetPairedListCount()),
			SETTING_IF_MANAGER, SLOT(OnRequestBTPairedListCount()), dbusConnectionType);
	connect(this, SIGNAL(RequestBTGetPairedList(uint32_t, uint32_t)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTPairedList(uint32_t, uint32_t)), dbusConnectionType);
	connect(this, SIGNAL(RequestBTGetPairedInfo(uint32_t)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTPairedInfo(uint32_t)), dbusConnectionType);


	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothSearchResult(uint32_t, QString, QString, int)),
			this, SLOT(OnBluetoothSearchResult(uint32_t, QString, QString, int)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(BluetoothSearchComplete()),
			this, SLOT(OnBluetoothSearchComplete()), dbusConnectionType);

	connect(this, SIGNAL(RequestBTDiscoverable(bool)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTDiscoverable(bool)), dbusConnectionType);
	connect(this, SIGNAL(RequestBTSearch(bool)),
			SETTING_IF_MANAGER, SLOT(OnRequestBTSearch(bool)), dbusConnectionType);
}

QString BluetoothManager::GetBTCodName(uint32_t deviceClass)
{
	QString codName;
	uint32_t majorDeviceClass;
	uint32_t minorDeviceClass;

	majorDeviceClass = (deviceClass & 0x00001F00) >> 8;
	minorDeviceClass = (deviceClass & 0x000000FC) >> 2;

	switch(majorDeviceClass)
	{
		case Computer:
			if(minorDeviceClass < COD_COMPUTER_MAX)
			{
				codName = _codComputer[minorDeviceClass];
			}
			else
			{
				codName = _codComputer[COD_COMPUTER_MAX-1];
			}
			break;
		case Phone:
			if(minorDeviceClass < COD_PHONE_MAX)
			{
				codName = _codPhone[minorDeviceClass];
			}
			else
			{
				codName = _codPhone[COD_PHONE_MAX-1];
			}
			break;
		case LanNetworkAccessPoint:
			codName = _codLAN;
			break;
		case AudioVideo:
			if(minorDeviceClass < COD_AV_MAX)
			{
				codName = _codAV[minorDeviceClass];
			}
			else
			{
				codName = _codAV[COD_AV_MAX-1];
			}
			break;
		case Peripheral:
			if(((minorDeviceClass & 0xF) > 0) &&
				((minorDeviceClass & 0xF) < COD_PERI_MAX))
			{
				codName = _codPeripheral[(minorDeviceClass & 0xF)];
			}
			else if((minorDeviceClass >> 4) < COD_PERI_UP_MAX)
			{
				codName = _codPeripheralUp[(minorDeviceClass >> 4)];
			}
			else
			{
				codName = _codPeripheral[0];
			}
			break;
		case Imaging:
			minorDeviceClass = (minorDeviceClass >> 2);
			if(minorDeviceClass < COD_IMAGING_MAX)
			{
				codName = _codImaging[minorDeviceClass];
			}
			else
			{
				codName = _codImaging[COD_IMAGING_MAX-1];
			}
			break;
		case Wearable:
			if(minorDeviceClass < COD_WEARABLE_MAX)
			{
				codName = _codWearable[minorDeviceClass];
			}
			else
			{
				codName = _codWearable[COD_WEARABLE_MAX-1];
			}
			break;
		case Toy:
			if(minorDeviceClass < COD_TOY_MAX)
			{
				codName = _codToy[minorDeviceClass];
			}
			else
			{
				codName = _codToy[COD_TOY_MAX-1];
			}
			break;
		case Health:
			if(minorDeviceClass < COD_HEALTH_MAX)
			{
				codName = _codHealth[minorDeviceClass];
			}
			else
			{
				codName = _codHealth[COD_HEALTH_MAX-1];
			}
			break;
		case Uncategorized:
			codName = _codUncategory;
			break;
		default:
			codName = _codDefault;
			break;
	}
	return codName;
}

void BluetoothManager::DeleteAllPairedInfo(void)
{
	_pairedList->ClearList();
	emit pairedListChanged();
}

void BluetoothManager::DeleteAllSearchInfo(void)
{
	_searchList->ClearList();
}

void BluetoothManager::OnBTConnected(QString addr, QString name)
{
	int index;

	BLUETOOTH_MANAGER_PRINTF("ADDRESS(%s), NAME(%s)\n",
									 addr.toLatin1().data(), name.toLatin1().data());

	emit remoteInfoChanged(name, addr);

	index = _pairedList->FindPairedInfoByAddress(addr);
	if(index == INVALID_INDEX)
	{
		_pairedList->AddList(name, addr, true);
	}
	else
	{
		_pairedList->SetConnectedPairedDevice(index, true);

		emit pairedListChanged();
	}
	emit connectedDeviceChanged(addr);
	emit pairedListChanged();
	emit RequestBTGetPairedListCount();
}

void BluetoothManager::OnBTDisconnected(QString addr)
{
	int index;

	BLUETOOTH_MANAGER_PRINTF("ADDRESS(%s)\n", addr.toLatin1().data());

	emit remoteInfoChanged("", "");

	index = _pairedList->FindPairedInfoByAddress(addr);
	if(index != INVALID_INDEX)
	{
		_pairedList->SetConnectedPairedDevice(index, false);

		emit pairedListChanged();
	}

	emit connectedDeviceChanged("");
}

void BluetoothManager::OnBTLocalInfo(QString addr, QString name)
{
	BLUETOOTH_MANAGER_PRINTF("ADDRESS(%s), NAME(%s)\n",
									 addr.toLatin1().data(), name.toLatin1().data());
	_haveLocalinfo = true;

	emit localInfoChanged(name, addr);
}

void BluetoothManager::OnBTOnOff(bool isBTOn)
{
	BLUETOOTH_MANAGER_PRINTF("isBTOn(%d)\n", isBTOn);
	_isBTOn = isBTOn;
	_isBTInit = true;
	emit bluetoothInitChanged(_isBTInit);
	emit bluetoothOnChanged(_isBTOn);

	if(isBTOn == true)
	{
		emit RequestBTGetPairedListCount();
	}
	else
	{
		DeleteAllPairedInfo();
	}
}

void BluetoothManager::OnBluetoothProfileStatus(QString addr, uint32_t hfp, uint32_t a2dp, uint32_t avrcp,
											uint32_t pbap, uint32_t map, uint32_t hidd, uint32_t pan)
{
	int index;

	BLUETOOTH_MANAGER_PRINTF("ADDRESS(%s)\n", addr.toLatin1().data());
	BLUETOOTH_MANAGER_PRINTF("\t HF(%u), A2DP(%u), AVRCP(%u), PBAP(%u)\n", hfp, a2dp, avrcp, pbap);
	BLUETOOTH_MANAGER_PRINTF("\t MAP(%u), HIDD(%u), PAN(%u)\n", map, hidd, pan);

	index = _pairedList->FindPairedInfoByAddress(addr);
	if(index != INVALID_INDEX)
	{
		_pairedList->SetAddrPairedDevice(index, addr);
		_pairedList->SetPropertyPairedDevice(index, hfp, a2dp, avrcp,
												pbap, map, hidd, pan);

		emit pairedListChanged();
	}
	else
	{
		emit RequestBTGetPairedListCount();
	}
}

void BluetoothManager::OnBluetoothPairedListCount(uint32_t totalnum)
{
	BLUETOOTH_MANAGER_PRINTF("Total Count(%u)\n", totalnum);
	if(totalnum != 0)
	{
		emit RequestBTGetPairedList(0, totalnum - 1);
	}
	else
	{
		_pairedList->ClearList();
	}
}

void BluetoothManager::OnBluetoothPairedDeviceList(uint32_t index, QString deviceName)
{
	int idx;

	BLUETOOTH_MANAGER_PRINTF("index(%u), Name(%s)\n", index, deviceName.toLatin1().data());

	idx = _pairedList->FindPairedInfoByName(deviceName);
	if(idx != INVALID_INDEX)
	{
		_pairedList->SetNamePairedDevice(idx, deviceName);
		BLUETOOTH_MANAGER_PRINTF("Exist Paired List index(%d)... replace\n", idx);
	}
	else
	{
		_pairedList->AddList(deviceName, "", false);
		BLUETOOTH_MANAGER_PRINTF("Append Paired List index(%d)\n", index);
	}

	emit pairedListChanged();
	emit RequestBTGetPairedInfo(index);
}

void BluetoothManager::OnBluetoothPairedDeviceInfo(uint32_t deviceClass, QString addr, QString name, uint32_t hfp, uint32_t a2dp,
												uint32_t avrcp, uint32_t pbap, uint32_t map, uint32_t hidd , uint32_t pan)
{
	int index;
	QString cod;

	BLUETOOTH_MANAGER_PRINTF("Class(0x%x), Address(%s), Name(%s)\n", deviceClass, addr.toLatin1().data(), name.toLatin1().data());
	BLUETOOTH_MANAGER_PRINTF("\t HF(%u), A2DP(%u), AVRCP(%u), PBAP(%u)\n", hfp, a2dp, avrcp, pbap);
	BLUETOOTH_MANAGER_PRINTF("\t MAP(%u), HIDD(%u), PAN(%u)\n", map, hidd, pan);

	index = _pairedList->FindPairedInfoByName(name);
	if(index != INVALID_INDEX)
	{
		cod = GetBTCodName(deviceClass);
		BLUETOOTH_MANAGER_PRINTF("CoD(%s)\n", cod.toLatin1().data());

		_pairedList->SetCoDPairedDevice(index, cod);
		_pairedList->SetAddrPairedDevice(index, addr);
		_pairedList->SetPropertyPairedDevice(index, hfp, a2dp, avrcp,
												pbap, map, hidd, pan);

		emit pairedListChanged();
	}
}

void BluetoothManager::OnBluetoothSearchResult(uint32_t deviceClass, QString addr, QString name, int rssi)
{
	int index;

	BLUETOOTH_MANAGER_PRINTF("ADDRESS(%s) Name(%s, %d, %d)\n", addr.toLatin1().data(), name.toLatin1().data(), name.size(), name.isEmpty());
	BLUETOOTH_MANAGER_PRINTF("Class(0x%x), RSSI(%d)\n", deviceClass, rssi);

	index = _searchList->FindSearchInfoByAddress(addr);
	if(index == INVALID_INDEX)
	{
		QString codString, nameString;

		index = _searchList->rowCount();
		codString = GetBTCodName(deviceClass);
		if(name.isEmpty())
		{
			nameString = addr;
		}
		else
		{
			nameString = name;
		}

		_searchList->AddList(codString, nameString, addr, rssi);
	}
}

void BluetoothManager::OnBluetoothSearchComplete(void)
{
	BLUETOOTH_MANAGER_PRINTF("\n");
	_searchStart = false;
	emit searchCompleted();
}

PairedDeviceList::PairedDeviceList(QAbstractListModel *parent) :
	QAbstractListModel(parent)
{
}

PairedDeviceList::~PairedDeviceList()
{
}

void PairedDeviceList::AddList(QString name, QString addr, bool connect)
{
	PairedInfo pairedInfo;

	pairedInfo._cod = "";
	pairedInfo._name = name;
	pairedInfo._addr = addr;
	pairedInfo._hfp = 0;
	pairedInfo._a2dp = 0;
	pairedInfo._avrcp = 0;
	pairedInfo._pbap = 0;
	pairedInfo._map = 0;
	pairedInfo._hidd = 0;
	pairedInfo._pan = 0;
	pairedInfo._connect = connect;

	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_pairedList.push_back(pairedInfo);
	endInsertRows();
}

void PairedDeviceList::ClearList(void)
{
	beginResetModel();
	_pairedList.clear();
	endResetModel();
}

int PairedDeviceList::FindPairedInfoByName(QString name)
{
	int index = INVALID_INDEX;
	int i, count;
	PairedInfo info;

	count = _pairedList.count();
	for (i = 0; i < count; i++)
	{
		info = _pairedList.at(i);
		if(info._name == name)
		{
			index = i;
			break;
		}
	}
	return index;
}

int PairedDeviceList::FindPairedInfoByAddress(QString address)
{
	int index = INVALID_INDEX;
	int i, count;
	PairedInfo info;

	count = _pairedList.count();
	for (i = 0; i < count; i++)
	{
		info = _pairedList.at(i);
		if(info._addr == address)
		{
			index = i;
			break;
		}
	}
	return index;
}

void PairedDeviceList::SetConnectedPairedDevice(int index, bool connected)
{
	PairedInfo info = _pairedList[index];
	info._connect = connected;
	_pairedList.replace(index, info);
}

void PairedDeviceList::SetNamePairedDevice(int index, QString name)
{
	PairedInfo info = _pairedList[index];
	info._name = name;
	_pairedList.replace(index, info);
}

void PairedDeviceList::SetCoDPairedDevice(int index, QString cod)
{
	PairedInfo info = _pairedList[index];
	info._cod = cod;
	_pairedList.replace(index, info);
}

void PairedDeviceList::SetAddrPairedDevice(int index, QString address)
{
	PairedInfo info = _pairedList[index];
	info._addr = address;
	_pairedList.replace(index, info);
}

void PairedDeviceList::SetPropertyPairedDevice(int index, uint32_t hfp, uint32_t a2dp, uint32_t avrcp,
						uint32_t pbap, uint32_t map, uint32_t hidd, uint32_t pan)
{
	PairedInfo info = _pairedList[index];
	info._hfp = hfp;
	info._a2dp = a2dp;
	info._avrcp = avrcp;
	info._pbap = pbap;
	info._map = map;
	info._hidd = hidd;
	info._pan = pan;
	_pairedList.replace(index, info);
}

void PairedDeviceList::RemovePairedDevice(int index)
{
	beginRemoveRows(QModelIndex(), index, index);
	_pairedList.removeAt(index);
	endRemoveRows();
}

int PairedDeviceList::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent)
	return _pairedList.count();
}

QVariant PairedDeviceList::data(const QModelIndex & index, int role) const
{
	QVariant var;

	if (index.row() > 0 || index.row() < _pairedList.count())
	{
		PairedInfo pairedInfo = _pairedList[index.row()];

		if (role == CoD)
		{
			var = pairedInfo._cod;
		}
		else if (role == Name)
		{
			var = pairedInfo._name;
		}
		else if (role == Addr)
		{
			var = pairedInfo._addr;
		}
		else if (role == HFP)
		{
			var = pairedInfo._hfp;
		}
		else if (role == A2DP)
		{
			var = pairedInfo._a2dp;
		}
		else if (role == AVRCP)
		{
			var = pairedInfo._avrcp;
		}
		else if (role == PBAP)
		{
			var = pairedInfo._pbap;
		}
		else if (role == MAP)
		{
			var = pairedInfo._map;
		}
		else if (role == HIDD)
		{
			var = pairedInfo._hidd;
		}
		else if (role == PAN)
		{
			var = pairedInfo._pan;
		}
		else if (role == Connect)
		{
			var = pairedInfo._connect;
		}
	}

	return var;
}

QHash<int, QByteArray> PairedDeviceList::roleNames() const
{
	QHash<int, QByteArray> roles;

	roles[CoD] = "cod";
	roles[Name] = "name";
	roles[Addr] = "addr";
	roles[HFP] = "hfpStat";
	roles[A2DP] = "a2dpStat";
	roles[AVRCP] = "avrcpStat";
	roles[PBAP] = "pbapStat";
	roles[MAP] = "mapStat";
	roles[HIDD] = "hiddStat";
	roles[PAN] = "panStat";
	roles[Connect] = "connect";

	return roles;
}

SearchDeviceList::SearchDeviceList(QAbstractListModel *parent) :
	QAbstractListModel(parent)
{
}

SearchDeviceList::~SearchDeviceList()
{
}

void SearchDeviceList::AddList(QString cod, QString name, QString addr, int rssi)
{
	SearchInfo searchInfo;

	searchInfo._cod = cod;
	searchInfo._name = name;
	searchInfo._addr = addr;
	searchInfo._rssi = rssi;

	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_searchList.push_back(searchInfo);
	endInsertRows();
}

void SearchDeviceList::ClearList(void)
{
	beginResetModel();
	_searchList.clear();
	endResetModel();
}

int SearchDeviceList::FindSearchInfoByAddress(QString address)
{
	int index = INVALID_INDEX;
	int i, count;
	SearchInfo info;

	count = _searchList.count();
	for (i = 0; i < count; i++)
	{
		info = _searchList.at(i);
		if(info._addr == address)
		{
			index = i;
			break;
		}
	}
	return index;
}

int SearchDeviceList::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent)
	return _searchList.count();
}

QVariant SearchDeviceList::data(const QModelIndex & index, int role) const
{
	QVariant var;

	if (index.row() > 0 || index.row() < _searchList.count())
	{
		SearchInfo searchInfo = _searchList[index.row()];

		if (role == CoD)
		{
			var = searchInfo._cod;
		}
		else if (role == Name)
		{
			var = searchInfo._name;
		}
		else if (role == Addr)
		{
			var = searchInfo._addr;
		}
		else if (role == RSSI)
		{
			var = searchInfo._rssi;
		}
	}

	return var;
}

QHash<int, QByteArray> SearchDeviceList::roleNames() const
{
	QHash<int, QByteArray> roles;

	roles[CoD] = "cod";
	roles[Name] = "name";
	roles[Addr] = "addr";
	roles[RSSI] = "rssi";

	return roles;
}
