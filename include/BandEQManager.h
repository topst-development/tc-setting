/****************************************************************************************
 *   FileName    : BandEQManager.h
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

#ifndef BAND_EQ_MANAGER_H
#define BAND_EQ_MANAGER_H

#include <QWidget>
#include <QSlider>
#include <QTimer>
#include "TCFontLabel.h"
#include "TCImageButton.h"
#include "TCPopupManager.h"

enum band_eq_mode_t {
	POP_MODE_IDX = 0,
	ROCK_MODE_IDX,
	JAZZ_MODE_IDX,
	CLASSIC_MODE_IDX,
	HIPHOP_MODE_IDX,
	MANUAL_MODE_IDX,
};

struct band_eq_t {
	int low_bass;
	int bass;
	int mid;
	int low_treble;
	int treble;
};

class BandEQStatusBar : public QWidget {
	public:
		BandEQStatusBar(QWidget *parent = 0);
		~BandEQStatusBar();
		void SetGeometry(int x, int y);
		int SetImage(int value);
	private:
		TCImageButton	*m_btnBar;
		TCFontLabel		*m_lblNumber;
};

class BandEQManager : public QWidget
{
    Q_OBJECT

	public:
		BandEQManager(QWidget *parent = 0);
		~BandEQManager();
		void Initialize(int x, int y, int screenWidth, int screenHeight);

	protected:
		void showEvent(QShowEvent *event);
		void hideEvent(QHideEvent *event);

	private:
		void InitializeSignalSlots(void);
        void UpdateGeometry(void);
        void UpdateImages(void);
		void SetCurruntValues(void);
		int SetPreDefValues(enum band_eq_mode_t idx);

	private slots:
		void OnPopClicked(void);
		void OnRockClicked(void);
		void OnJazzClicked(void);
		void OnClassicClicked(void);
		void OnHiphopClicked(void);
		void OnManualClicked(void);

		void OnLowBassPlusClicked(void);
		void OnLowBassMinusClicked(void);
		void OnBassPlusClicked(void);
		void OnBassMinusClicked(void);
		void OnMidPlusClicked(void);
		void OnMidMinusClicked(void);
		void OnLowTreblePlusClicked(void);
		void OnLowTrebleMinusClicked(void);
		void OnTreblePlusClicked(void);
		void OnTrebleMinusClicked(void);

		void OnBandEQUnControlMode(void);
		void OnBandEQControlMode(void);

		void OnKeyboardClicked(int key);

	signals:
		void SigPopClick(void);
		void SigRockClick(void);
		void SigJazzClick(void);
		void SigClassicClick(void);
		void SigHophopClick(void);
		void SigManualClick(void);

		void SigEQButtonEnable(void);
		void SigSetBandEQLowBass(int val);
		void SigSetBandEQBass(int val);
		void SigSetBandEQMid(int val);
		void SigSetBandEQLowTreble(int val);
		void SigSetBandEQTreble(int val);

	private:
		TCFontLabel				*m_lblLowBassTitle;
		TCFontLabel				*m_lblBassTitle;
		TCFontLabel				*m_lblMidTitle;
		TCFontLabel				*m_lblLowTrebleTitle;
		TCFontLabel				*m_lblTrebleTitle;

		BandEQStatusBar			*m_statLowBass;
		BandEQStatusBar			*m_statBass;
		BandEQStatusBar			*m_statMid;
		BandEQStatusBar			*m_statLowTreble;
		BandEQStatusBar			*m_statTreble;

		TCImageButtonManager	m_btnManualEQManager;
		TCImageButton			*m_btnLowBassPlus;
		TCImageButton			*m_btnLowBassMinus;
		TCImageButton			*m_btnBassPlus;
		TCImageButton			*m_btnBassMinus;
		TCImageButton			*m_btnMidPlus;
		TCImageButton			*m_btnMidMinus;
		TCImageButton			*m_btnLowTreblePlus;
		TCImageButton			*m_btnLowTrebleMinus;
		TCImageButton			*m_btnTreblePlus;
		TCImageButton			*m_btnTrebleMinus;

		TCImageButtonManager	m_btnBandEQManager;
		TCImageButton			*m_btnPop;
		TCImageButton			*m_btnRock;
		TCImageButton			*m_btnJazz;
		TCImageButton			*m_btnClassic;
		TCImageButton			*m_btnHiphop;
		TCImageButton			*m_btnManual;

		struct band_eq_t		m_curValue;
		enum band_eq_mode_t		m_curMode;
};

#endif 
