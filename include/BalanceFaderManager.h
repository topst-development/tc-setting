/****************************************************************************************
 *   FileName    : BalanceFaderManager.h
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

#ifndef BALANCE_FADER_MANAGER_H
#define BALANCE_FADER_MANAGER_H

#include <QWidget>
#include <QSlider>
#include <QTimer>
#include "TCFontLabel.h"
#include "TCImageButton.h"
#include "TCPopupManager.h"

class BalanceFaderManager : public QWidget
{
    Q_OBJECT

	public:
		BalanceFaderManager(QWidget *parent = 0);
		~BalanceFaderManager();
		void Initialize(int x, int y, int screenWidth, int screenHeight);

	protected:
		void showEvent(QShowEvent *event);
		void hideEvent(QHideEvent *event);

	private:
		void InitializeSignalSlots(void);
        void UpdateGeometry();
        void UpdateImages();
		void SetCurruntValues(void);
		void SetBalanceValue(int val);
		void SetFaderValue(int val);

	private slots:
		void OnBalancePlusClicked(void);
		void OnBalanceMinusClicked(void);
		void OnFaderPlusClicked(void);
		void OnFaderMinusClicked(void);

		void OnBalanceChanged(int val);
		void OnFaderChanged(int val);

		void OnKeyboardClicked(int key);

	signals:
		void SigEQButtonEnable(void);
		void SigSetBalance(int val);
		void SigSetFader(int val);

	private:
		TCFontLabel				*m_lblBalanceTitle;
		TCFontLabel				*m_lblFaderTitle;

		QSlider					*m_sldBalance;
		QSlider					*m_sldFader;
		QSlider					*m_sldBalanceLine;
		QSlider					*m_sldFaderLine;

		TCImageButton			*m_btnBalanceFaderBG;
		TCImageButton			*m_btnBalancePlus;
		TCImageButton			*m_btnBalanceMinus;
		TCImageButton			*m_btnFaderPlus;
		TCImageButton			*m_btnFaderMinus;

		int						m_curBalance;
		int						m_curFader;
};

#endif 
