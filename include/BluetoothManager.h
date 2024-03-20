/****************************************************************************************
 *   FileName    : BluetoothManager.h
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

#ifndef BLUETOOTH_SETTING_H
#define BLUETOOTH_SETTING_H

#include <QObject>
#include <QTimer>
#include <QAbstractListModel>

typedef enum {
	BTSettingTypeDefault,
	BTSettingTypePaired,
	BTSettingTypeAvailable
} BTSettingType;

class PairedDeviceList;
class SearchDeviceList;

class BluetoothManager : public QObject
{
    Q_OBJECT

	Q_PROPERTY(bool bluetoothInit READ bluetoothInit NOTIFY bluetoothInitChanged)
	Q_PROPERTY(bool bluetoothOn READ bluetoothOn NOTIFY bluetoothOnChanged)
	Q_PROPERTY(QAbstractListModel *pairedList READ pairedList NOTIFY pairedListChanged)
	Q_PROPERTY(QAbstractListModel *searchList READ searchList NOTIFY searchListChanged)

	public:
		BluetoothManager(QObject *parent = 0);
		~BluetoothManager();
		void Initialize(void);

		Q_INVOKABLE void prepareBluetooth(void);
		Q_INVOKABLE void enableBluetooth(void);
		Q_INVOKABLE void connectDevice(QString addr);

		Q_INVOKABLE void disconnectPairedDevice(QString addr);
		Q_INVOKABLE void deletePairedDevice(QString addr);

		bool bluetoothInit(void) const;
		bool bluetoothOn(void) const;
		QString localAddr(void) const;
		QString remoteName(void) const;
		QString remoteAddr(void) const;
		QAbstractListModel *pairedList(void) const;
		QAbstractListModel *searchList(void) const;

		Q_INVOKABLE void setDiscoverable(bool discoverable);
		Q_INVOKABLE void searchDevice(bool search);

	private:
		void InitializeSignalSlots(void);
		QString GetBTCodName(uint32_t deviceClass);
		int FindPairedInfoByIndex(uint32_t index);
		void DeleteAllPairedInfo(void);
		void DeleteAllSearchInfo(void);

	private slots:
		void OnBTConnected(QString addr, QString name);
		void OnBTDisconnected(QString addr);
		void OnBTLocalInfo(QString localAddr, QString localName);
		void OnBTOnOff(bool isBTOn);

		void OnBluetoothProfileStatus(QString addr, uint32_t hfp, uint32_t a2dp, uint32_t avrcp,
											uint32_t pbap, uint32_t map, uint32_t hidd, uint32_t pan);
		void OnBluetoothPairedListCount(uint32_t totalnum);
		void OnBluetoothPairedDeviceList(uint32_t index, QString deviceName);
		void OnBluetoothPairedDeviceInfo(uint32_t deviceClass, QString addr, QString name, uint32_t hfp, uint32_t a2dp,
											uint32_t avrcp, uint32_t pbap, uint32_t map, uint32_t hidd , uint32_t pan);

		void OnBluetoothSearchResult(uint32_t deviceClass, QString addr, QString name, int rssi);
		void OnBluetoothSearchComplete(void);

	signals:
		void RequestBTLocalInfo(void);
		void RequestBTGetStatus(void);
		void RequestBTOnOff(bool);

		void bluetoothInitChanged(bool on);
		void bluetoothOnChanged(bool on);
		void localInfoChanged(QString localName, QString localAddr);
		void remoteInfoChanged(QString remoteName, QString remoteAddr);

		void RequestBTConnect(QString);

		void RequestBTDisconnect(QString);
		void RequestBTDelete(QString);
		void RequestBTGetPairedListCount(void);
		void RequestBTGetPairedList(uint32_t startIdx, uint32_t endIdx);
		void RequestBTGetPairedInfo(uint32_t index);

		void RequestBTDiscoverable(bool);
		void RequestBTSearch(bool);

		void connectedDeviceChanged(QString connAddr);
		void pairedListChanged(void);

		void searchCompleted(void);
		void searchListChanged(void);

	private:
		bool _haveLocalinfo;
		bool _isBTInit;
		bool _isBTOn;

		uint32_t _currentPairedIndex;
		PairedDeviceList *_pairedList;

		bool _discoverable;
		bool _searchStart;
		uint32_t _currentSearchIndex;
		SearchDeviceList *_searchList;
};

class PairedDeviceList : public QAbstractListModel
{
	Q_OBJECT

	enum PairedDeviceListRoles {
		CoD = Qt::UserRole + 1,
		Name,
		Addr,
		HFP,
		A2DP,
		AVRCP,
		PBAP,
		MAP,
		HIDD,
		PAN,
		Connect
	};

	public:
		explicit PairedDeviceList(QAbstractListModel *parent = 0);
		~PairedDeviceList();
		void AddList(QString name, QString addr, bool connect);
		void ClearList(void);
		int FindPairedInfoByName(QString name);
		int FindPairedInfoByAddress(QString address);
		void SetConnectedPairedDevice(int index, bool connected);
		void SetNamePairedDevice(int index, QString name);
		void SetCoDPairedDevice(int index, QString cod);
		void SetAddrPairedDevice(int index, QString address);
		void SetPropertyPairedDevice(int index, uint32_t hfp, uint32_t a2dp, uint32_t avrcp,
									uint32_t pbap, uint32_t map, uint32_t hidd, uint32_t pan);
		void RemovePairedDevice(int index);

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	protected:
		QHash<int, QByteArray> roleNames() const;

	public:
		struct PairedInfo {
			QString _cod;
			QString _name;
			QString _addr;
			uint32_t _hfp;
			uint32_t _a2dp;
			uint32_t _avrcp;
			uint32_t _pbap;
			uint32_t _map;
			uint32_t _hidd;
			uint32_t _pan;
			bool _connect;
		};

	private:
		QList<PairedInfo> _pairedList;
};

class SearchDeviceList : public QAbstractListModel
{
	Q_OBJECT

	enum SearchDeviceListRoles {
		CoD = Qt::UserRole + 1,
		Name,
		Addr,
		RSSI
	};

	public:
		explicit SearchDeviceList(QAbstractListModel *parent = 0);
		~SearchDeviceList();
		void AddList(QString cod, QString name, QString addr, int rssi);
		void ClearList(void);
		int FindSearchInfoByAddress(QString address);

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	protected:
		QHash<int, QByteArray> roleNames() const;

	public:
		struct SearchInfo {
			QString _cod;
			QString _name;
			QString _addr;
			int32_t _rssi;
		};

	private:
		QList<SearchInfo> _searchList;
};
#endif
