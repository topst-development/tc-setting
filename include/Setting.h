/****************************************************************************************
 *   FileName    : Setting.h
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

#ifndef SETTING_H
#define SETTING_H

#include <QObject>

#define AUDIO "vol"
#define COLORADJUSTMENT "ca"
#define BLUETOOTH "bluetooth"
#define FWUPDATE "fwup"

class Setting : public QObject
{
    Q_OBJECT

	Q_PROPERTY(QStringList menu MEMBER m_menu NOTIFY menuChanged)

	public:
		Setting(QObject *parent = 0);
		~Setting();
		void Initialize(int tsound);
		void AddMenu(QString menu);
		void CompleteMenu(void);

		Q_INVOKABLE void goHome(void);
		Q_INVOKABLE void goBack(void);
		Q_INVOKABLE void goSetting(void);

	private:
		void InitializeSignalSlots(void);
		void ShowSetting();
		void HideSetting();

	private slots:
		void OnKeyboardClicked(int key);
		void OnLauncherEnableKeyEvent(int app);
		void OnLauncherDisableKeyEvent(int app);
		void OnChangedMode(int mode, int app);
		void OnSuspendMode();
		void OnResumeMode();
		void OnReleaseResource(int resource, int app);

	signals:
		void ChangeMode(int mode, int app);
		void ReleaseResourceDone(int resource, int app);
		void showHideChanged(bool show);
		void homePressed(void);
		void backPressed(void);
		void settingPressed(void);
		void menuChanged(void);

	private:
		bool					m_show;
		bool					m_enableKey;
		QStringList				m_menu;
};
#endif
