/****************************************************************************************
 *   FileName    : FWUpdateManager.h
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

#ifndef FWUPDATE_MANAGER_H
#define FWUPDATE_MANAGER_H

#include <QObject>
#include "FWUpdateThread.h"

class FWUpdate : public QObject
{
    Q_OBJECT

	Q_PROPERTY(bool coreSpecUpdate MEMBER m_coreSpecUpdate NOTIFY coreSpecUpdateChanged)
	Q_PROPERTY(bool allUpdate MEMBER m_allUpdate NOTIFY allUpdateChanged)
	Q_PROPERTY(bool mainUpdate MEMBER m_mainUpdate NOTIFY mainUpdateChanged)
	Q_PROPERTY(bool subUpdate MEMBER m_subUpdate NOTIFY subUpdateChanged)
	Q_PROPERTY(bool snorUpdate MEMBER m_snorUpdate NOTIFY snorUpdateChanged)
	Q_PROPERTY(QString resultLog MEMBER m_resultLog NOTIFY resultLogChanged)
	Q_PROPERTY(QString notifyLog MEMBER m_notifyLog NOTIFY notifyLogChanged)

	public:
		FWUpdate(QObject *parent = 0);
		~FWUpdate();
		void Initialize(void);

		Q_INVOKABLE void showUpdateMenu(void);
		Q_INVOKABLE void updateAllFW(void);
		Q_INVOKABLE void updateMainFW(void);
		Q_INVOKABLE void updateSubFW(void);
		Q_INVOKABLE void updateSnorFW(void);

	private:
		void InitializeSignalSlots(void);
		int find_update_dir(void);

	private slots:
		void OnFWUpdateResult(int updateCore,int result);
		void OnFWUpdateDone();

	signals:
		void coreSpecUpdateChanged(void);
		void allUpdateChanged(void);
		void mainUpdateChanged(void);
		void subUpdateChanged(void);
		void snorUpdateChanged(void);
		void resultLogChanged(void);
		void notifyLogChanged(void);

	private:
		bool								m_coreSpecUpdate;
		bool								m_allUpdate;
		bool								m_mainUpdate;
		bool								m_subUpdate;
		bool								m_snorUpdate;
		bool								m_initFWUpdate;
		bool								m_nowUpdating;
		QString								m_resultLog;
		QString								m_notifyLog;
		FWUpdateThread						*m_FWUpdateThread;
};
#endif
