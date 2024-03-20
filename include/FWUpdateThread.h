/****************************************************************************************
 *   FileName    : FWUpdateThread.h
 *   Description : 
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited 
to re-distribution in source or binary form is strictly prohibited.
This source code is provided ��AS IS�� and nothing contained in this source code 
shall constitute any express or implied warranty of any kind, including without limitation, 
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent, 
copyright or other third party intellectual property right. 
No warranty is made, express or implied, regarding the information��s accuracy, 
completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, 
out of or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************************/

#ifndef FWUPDATE_THREAD_H
#define FWUPDATE_THREAD_H

#include <dbus/dbus.h>
#include <QThread>
#include <QObject>

#define CMD_UPDATE_MAIN_CORE	1
#define CMD_UPDATE_SUB_CORE		2
#define CMD_UPDATE_SNOR			3
#define CMD_UPDATE_ALL			4

class FWUpdateThread : public QThread
{
	Q_OBJECT

	public:
		FWUpdateThread(QObject *parent = 0);
		~FWUpdateThread();
		void Run(int32_t update_cmd);
		void Stop();

	private:
		void run(void);
		void ExitFWUpdateThread(void);

	private slots:


	signals:
		void updateResult(int updatCore,int result);
		void updateDone();

	public:
		char update_main_dir_path[PATH_MAX];
		char update_sub_dir_path[PATH_MAX];
	private:
		int32_t _update_cmd;

};

extern FWUpdateThread *GetfwUpdateThread();
#define FWUPDATE_THREAD (GetfwUpdateThread())

#endif

