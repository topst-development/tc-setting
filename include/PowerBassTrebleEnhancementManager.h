/****************************************************************************************
 *   FileName    : PowerBassTrebleEnhancementManager.h
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

#ifndef POWERBASS_TREBLEENHANCE_MANAGER_H
#define POWERBASS_TREBLEENHANCE_MANAGER_H

#include <QWidget>
#include <QSlider>
#include <QTimer>
#include "TCFontLabel.h"
#include "TCImageButton.h"
#include "TCPopupManager.h"

class PowerBassTrebleEnhancementManager : public QWidget
{
    Q_OBJECT

	public:
		PowerBassTrebleEnhancementManager(QWidget *parent = 0);
		~PowerBassTrebleEnhancementManager();
		void Initialize(int x, int y, int screenWidth, int screenHeight);

	protected:
		void showEvent(QShowEvent *event);
		void hideEvent(QHideEvent *event);

	private:
		void InitializeSignalSlots(void);
        void UpdateGeometry();
        void UpdateImages();
		void SetCurruntValues(void);
		void SetStatusImage(TCImageButton *btn, int idx);

	private slots:
		void OnPowerBassPlusClicked(void);
		void OnPowerBassMinusClicked(void);
		void OnTrebleEnhancementPlusClicked(void);
		void OnTrebleEnhancementMinusClicked(void);

		void OnKeyboardClicked(int key);

	signals:
		void SigEQButtonEnable(void);
		void SigSetPowerBass(int val);
		void SigSetTrebleEnhancement(int val);

	private:
		TCFontLabel							*m_lblPowerBassTitle;
		TCImageButton						*m_btnPowerBassPlus;
		TCImageButton						*m_btnPowerBassMinus;
		TCImageButton						*m_btnPowerBassStatus;

		TCFontLabel							*m_lblTrebleEnhancementTitle;
		TCImageButton						*m_btnTrebleEnhancementPlus;
		TCImageButton						*m_btnTrebleEnhancementMinus;
		TCImageButton						*m_btnTrebleEnhancementStatus;
                                            
		int									m_curPowerBassIdx;
		int									m_curTrebleEnhancementIdx;
};

#endif 
