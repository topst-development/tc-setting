/****************************************************************************************
 *   FileName    : AudioManager.h
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

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <QObject>

#define MAX_VOLUME_COUNT    11

class AudioManager : public QObject
{
    Q_OBJECT

	public:
		AudioManager(QObject *parent = 0);
		~AudioManager();
		void Initialize(void);

		Q_INVOKABLE void increaseVolume(int type);
		Q_INVOKABLE void decreaseVolume(int type);

	private:
		void InitializeSignalSlots(void);
		void OnMediaVolumeDownClicked(void);
		void OnTextVolumeDownClicked(void);
		void OnNaviVolumeDownClicked(void);
		void OnCallVolumeDownClicked(void);
		void OnAlertVolumeDownClicked(void);
		void OnMediaVolumeUpClicked(void);
		void OnTextVolumeUpClicked(void);
		void OnNaviVolumeUpClicked(void);
		void OnCallVolumeUpClicked(void);
		void OnAlertVolumeUpClicked(void);

	private slots:
		void OnVolumeChanged(unsigned short sourceID, short volume);

	signals:
		void SetVolume(unsigned short sourceID, short volume);
		void changedVolume(unsigned short sourceID, short volume);

	private:
		std::map<unsigned short, short> 	_sourceIDtoVolume;
};
#endif
