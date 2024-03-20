/****************************************************************************************
 *   FileName    : BalanceFaderManager.cpp
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
#include <unistd.h>
#include <QMouseEvent>
#include "TCInput.h"
#include "TCPopupManager.h"
#include "Setting.h"
#include "SettingIFManager.h"
#include "EQMacro.h"
#include "BalanceFaderManager.h"

extern int g_debug;

#define BF_MANAGER_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[BALANCE_FADER_MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

static const char *_btnBalanceFaderBGImageName = ":/images/setting_ctrl_seat_bg.png";
static const int _btnBalanceFaderBGRect[4] = { 
	485-EQ_X_OFFSET, 161-EQ_Y_OFFSET, 321,338 
};

static const int _lblBalanceTitleRect[4] = { 
	390-EQ_X_OFFSET, 518-EQ_Y_OFFSET, 76, 19
};

static const int _lblFaderTitleRect[4] = { 
	806-EQ_X_OFFSET, 123-EQ_Y_OFFSET, 76, 19
};


#define BALANCE_STYLE_SHEET_CONFIG	\
	"QSlider::groove:horizontal {" \
	"	background-image: url(:/images/setting_ctrl_bar_h.png);" \
	"	width: 239px;" \
	"	height: 4px;" \
	"	margin-top: 12px;" \
	"	margin-bottom: 6px;" \
	"}" \
	"QSlider::handle:horizontal {" \
	"	background-image: url(:/images/setting_ctrl_guideline_h.png);" \
	"	background-position: bottom left;" \
	"	width: 12px;" \
	"	margin-top: -12px;" \
	"	margin-bottom: -6px;" \
	"}"

#define FADER_STYLE_SHEET_CONFIG	\
	"QSlider::groove:vertical {" \
	"	background-image: url(:/images/setting_ctrl_bar_v.png);" \
	"	width: 4px;" \
	"	height: 256px;" \
	"	margin-left: 12px;" \
	"	margin-right: 6px;" \
	"}" \
	"QSlider::handle:vertical {" \
	"	background-image: url(:/images/setting_ctrl_guideline_v.png);" \
	"	background-position: bottom right;" \
	"	height: 12px;" \
	"	margin-left: -12px;" \
	"	margin-right: -6px;" \
	"}"

#define BALANCE_LINE_STYLE_SHEET_CONFIG	\
	"::handle:horizontal{" \
	"background-image: url(:/images/setting_ctrl_guideline_h.png);" \
	"width: 12px;" \
	"height: 376px;" \
	"}" \
	"::groove:horizontal{" \
	"border: 0px;" \
	"}"

#define FADER_LINE_STYLE_SHEET_CONFIG	\
	"::handle:vertical{" \
	"background-image: url(:/images/setting_ctrl_guideline_v.png);" \
	"width: 359px;" \
	"height: 12px;" \
	"}" \
	"::groove:vertical{" \
	"border: 0px;" \
	"}"

#define BALANCE_STEP			(10)
#define FADER_STEP				(10)

#define BALANCE_MAX_VALUE		(100)
#define BALANCE_MIN_VALUE		(-100)

#define FADER_MAX_VALUE			(100)
#define FADER_MIN_VALUE			(-100)

#define BALANCE_DEFAULT			(0)
#define FADER_DEFAULT			(0)

DEF_RECT_INFO(_sldBalance, 526-EQ_X_OFFSET, 527-12-EQ_Y_OFFSET, 241, 22);
DEF_RECT_INFO(_sldFader, 834-12-EQ_X_OFFSET, 202-EQ_Y_OFFSET, 22, 258);
DEF_RECT_INFO(_sldBalanceLine, 525-EQ_X_OFFSET, 161-EQ_Y_OFFSET, 241, 338);
DEF_RECT_INFO(_sldFaderLine, 485-EQ_X_OFFSET, 201-EQ_Y_OFFSET, 321, 258);

DEF_BTN_INFO(_btnBalancePlus, 
	770-EQ_X_OFFSET, 511-EQ_Y_OFFSET, 36, 36, 
	":/images/setting_bt_ctrl_right_nor.png", 
	":/images/setting_bt_ctrl_right_prsd.png", 
	NULL, NULL); 

DEF_BTN_INFO(_btnBalanceMinus, 
	485-EQ_X_OFFSET, 511-EQ_Y_OFFSET, 36, 36, 
	":/images/setting_bt_ctrl_left_nor.png", 
	":/images/setting_bt_ctrl_left_prsd.png", 
	NULL, NULL); 

DEF_BTN_INFO(_btnFaderPlus, 
	818-EQ_X_OFFSET, 161-EQ_Y_OFFSET, 36, 36, 
	":/images/setting_bt_ctrl_up_nor.png", 
	":/images/setting_bt_ctrl_up_prsd.png", 
	NULL, NULL); 

DEF_BTN_INFO(_btnFaderMinus, 
	818-EQ_X_OFFSET, 463-EQ_Y_OFFSET, 36, 36, 
	":/images/setting_bt_ctrl_down_nor.png", 
	":/images/setting_bt_ctrl_down_prsd.png", 
	NULL, NULL); 

BalanceFaderManager::BalanceFaderManager(QWidget *parent) :
	QWidget(parent)
{
    m_lblBalanceTitle = new TCFontLabel(this);
    m_lblFaderTitle = new TCFontLabel(this);

    m_btnBalanceFaderBG = new TCImageButton(this);

	m_sldBalance = new QSlider(this);
	m_sldFader = new QSlider(this);
	m_sldBalanceLine = new QSlider(this);
	m_sldFaderLine = new QSlider(this);

    m_btnBalancePlus = new TCImageButton(this);
    m_btnBalanceMinus = new TCImageButton(this);
    m_btnFaderPlus = new TCImageButton(this);
    m_btnFaderMinus = new TCImageButton(this);

	m_curBalance = BALANCE_DEFAULT;
	m_curFader	= FADER_DEFAULT;
}

BalanceFaderManager::~BalanceFaderManager()
{
    delete m_lblBalanceTitle;
    delete m_lblFaderTitle;

    delete m_btnBalanceFaderBG;

	delete m_sldBalance;
	delete m_sldFader;
	delete m_sldBalanceLine;
	delete m_sldFaderLine;

    delete m_btnBalancePlus;
    delete m_btnBalanceMinus;
    delete m_btnFaderPlus;
    delete m_btnFaderMinus;

}

void BalanceFaderManager::Initialize(int x, int y, int screenWidth, int screenHeight)
{
	this->setWindowFlags(Qt::FramelessWindowHint);

	this->setGeometry(x, y, screenWidth, screenHeight);
	
	UpdateImages();
    UpdateGeometry();

	m_sldBalance->setOrientation(Qt::Horizontal);
	m_sldBalance->setStyleSheet(BALANCE_STYLE_SHEET_CONFIG); 
	m_sldBalance->setRange(BALANCE_MIN_VALUE, BALANCE_MAX_VALUE);
	m_sldBalance->setTickInterval(BALANCE_STEP);
	m_sldBalance->setValue(BALANCE_DEFAULT);
//	m_sldBalance->setDisabled(true);

	m_sldFader->setOrientation(Qt::Vertical);
	m_sldFader->setStyleSheet(FADER_STYLE_SHEET_CONFIG);
	m_sldFader->setRange(FADER_MIN_VALUE, FADER_MAX_VALUE);
	m_sldFader->setTickInterval(FADER_STEP);
	m_sldFader->setValue(FADER_DEFAULT);
//	m_sldFader->setDisabled(true);

	m_sldBalanceLine->setOrientation(Qt::Horizontal);
	m_sldBalanceLine->setStyleSheet(BALANCE_LINE_STYLE_SHEET_CONFIG); 
	m_sldBalanceLine->setRange(BALANCE_MIN_VALUE, BALANCE_MAX_VALUE);
	m_sldBalanceLine->setValue(BALANCE_DEFAULT);
	m_sldBalanceLine->setDisabled(true);

	m_sldFaderLine->setOrientation(Qt::Vertical);
	m_sldFaderLine->setStyleSheet(FADER_LINE_STYLE_SHEET_CONFIG);
	m_sldFaderLine->setRange(FADER_MIN_VALUE, FADER_MAX_VALUE);
	m_sldFaderLine->setValue(FADER_DEFAULT);
	m_sldFaderLine->setDisabled(true);

	InitializeSignalSlots();
	SetCurruntValues();
}

void BalanceFaderManager::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
}

void BalanceFaderManager::hideEvent(QHideEvent * event)
{
	QWidget::hideEvent(event);
}

void BalanceFaderManager::UpdateGeometry()
{
	m_lblBalanceTitle->setGeometry(_lblBalanceTitleRect[0], _lblBalanceTitleRect[1],
								   _lblBalanceTitleRect[2], _lblBalanceTitleRect[3]);

	m_lblFaderTitle->setGeometry(_lblFaderTitleRect[0], _lblFaderTitleRect[1],
								 _lblFaderTitleRect[2], _lblFaderTitleRect[3]);

	m_btnBalanceFaderBG->setGeometry(_btnBalanceFaderBGRect[0], _btnBalanceFaderBGRect[1],
									 _btnBalanceFaderBGRect[2], _btnBalanceFaderBGRect[3]);

	SET_RECT_GEOMETRY(m_sldBalance, _sldBalance);
	SET_RECT_GEOMETRY(m_sldFader, _sldFader);
	SET_RECT_GEOMETRY(m_sldBalanceLine, _sldBalanceLine);
	SET_RECT_GEOMETRY(m_sldFaderLine, _sldFaderLine);

	SET_RECT_GEOMETRY(m_btnBalancePlus, _btnBalancePlus);
	SET_RECT_GEOMETRY(m_btnBalanceMinus, _btnBalanceMinus);
	SET_RECT_GEOMETRY(m_btnFaderPlus, _btnFaderPlus);
	SET_RECT_GEOMETRY(m_btnFaderMinus, _btnFaderMinus);
}

void BalanceFaderManager::UpdateImages()
{
	m_lblBalanceTitle->SetPixelSize(21);
	m_lblBalanceTitle->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblBalanceTitle->setText("Balance");
	m_lblBalanceTitle->setAlignment(Qt::AlignCenter);

	m_lblFaderTitle->SetPixelSize(21);
	m_lblFaderTitle->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblFaderTitle->setText("Fader");
	m_lblFaderTitle->setAlignment(Qt::AlignCenter);

	m_btnBalanceFaderBG->SetImage(_btnBalanceFaderBGImageName);

	SET_BTN_IMAGES(m_btnBalancePlus, _btnBalancePlus);
	SET_BTN_IMAGES(m_btnBalanceMinus, _btnBalanceMinus);
	SET_BTN_IMAGES(m_btnFaderPlus, _btnFaderPlus);
	SET_BTN_IMAGES(m_btnFaderMinus, _btnFaderMinus);
}

void BalanceFaderManager::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	connect(SETTING_IF_MANAGER, SIGNAL(SigKeyboardClicked(int)), this, SLOT(OnKeyboardClicked(int)), dbusConnectionType);

	connect(m_btnBalancePlus, SIGNAL(mouseClicked()), this, SLOT(OnBalancePlusClicked()));
	connect(m_btnBalanceMinus, SIGNAL(mouseClicked()), this, SLOT(OnBalanceMinusClicked()));
	connect(m_btnFaderPlus, SIGNAL(mouseClicked()), this, SLOT(OnFaderPlusClicked()));
	connect(m_btnFaderMinus, SIGNAL(mouseClicked()), this, SLOT(OnFaderMinusClicked()));

	connect(m_sldBalance, SIGNAL(valueChanged(int)), this, SLOT(OnBalanceChanged(int)));
	connect(m_sldFader, SIGNAL(valueChanged(int)), this, SLOT(OnFaderChanged(int)));

	connect(this, SIGNAL(SigSetBalance(int)), SETTING_IF_MANAGER, SLOT(OnSetBalance(int)));
	connect(this, SIGNAL(SigSetFader(int)), SETTING_IF_MANAGER, SLOT(OnSetFade(int)));
}

void BalanceFaderManager::SetBalanceValue(int val)
{
	m_curBalance = val;

	if (m_curBalance > BALANCE_MAX_VALUE) {
		m_curBalance = BALANCE_MAX_VALUE;
	}

	if (m_curBalance < BALANCE_MIN_VALUE) {
		m_curBalance = BALANCE_MIN_VALUE;
	}

	emit SigSetBalance(m_curBalance);

	m_sldBalance->setValue(m_curBalance);
	m_sldBalanceLine->setValue(m_curBalance);

	BF_MANAGER_PROCESS_PRINTF("m_curBalance: %d\n", m_curBalance);
}

void BalanceFaderManager::SetFaderValue(int val)
{
	m_curFader = val;

	if (m_curFader > FADER_MAX_VALUE) {
		m_curFader = FADER_MAX_VALUE;
	}

	if (m_curFader < FADER_MIN_VALUE) {
		m_curFader = FADER_MIN_VALUE;
	}

	emit SigSetFader(m_curFader);

	m_sldFader->setValue(m_curFader);
	m_sldFaderLine->setValue(m_curFader);

	BF_MANAGER_PROCESS_PRINTF("m_curFader: %d\n", m_curFader);
}

void BalanceFaderManager::OnBalanceChanged(int value)
{
	BF_MANAGER_PROCESS_PRINTF("\n");

	SetBalanceValue(value);
}

void BalanceFaderManager::OnFaderChanged(int value)
{
	BF_MANAGER_PROCESS_PRINTF("\n");

	SetFaderValue(value);
}

void BalanceFaderManager::OnBalancePlusClicked(void)
{
	BF_MANAGER_PROCESS_PRINTF("\n");

	SetBalanceValue(m_curBalance+BALANCE_STEP);
}

void BalanceFaderManager::OnBalanceMinusClicked(void)
{
	BF_MANAGER_PROCESS_PRINTF("\n");

	SetBalanceValue(m_curBalance-BALANCE_STEP);
}

void BalanceFaderManager::OnFaderPlusClicked(void)
{
	BF_MANAGER_PROCESS_PRINTF("\n");

	SetFaderValue(m_curFader+FADER_STEP);
}

void BalanceFaderManager::OnFaderMinusClicked(void)
{
	BF_MANAGER_PROCESS_PRINTF("\n");

	SetFaderValue(m_curFader-FADER_STEP);
}

void BalanceFaderManager::SetCurruntValues(void)
{
	SetBalanceValue(m_curBalance);
	SetFaderValue(m_curFader);
}

void BalanceFaderManager::OnKeyboardClicked(int key)
{
	BF_MANAGER_PROCESS_PRINTF("key: %d\n", key);

	if (this->isVisible()) {
	}
}

