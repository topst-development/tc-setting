/****************************************************************************************
 *   FileName    : EQManager.cpp
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
#include "SettingDBusManager.h"
#include "EQMacro.h"
#include "PowerBassTrebleEnhancementManager.h"

extern int g_debug;
extern int g_appID;

#define PT_MANAGER_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[PT_MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

#define	ARRAY_SIZE(x)	((int)(sizeof(x)/sizeof(x[0])))

int PowerBassValues[] = {0, 5, 10, 14, 19, 24};
int TrebleEnhancementValues[] = {0, 6, 12, 18, 24, 30};

#define POWERBASS_MIN_STEP				(0)
#define POWERBASS_MAX_STEP				(ARRAY_SIZE(PowerBassValues) - 1)
#define TREBLEENHANCEMENT_MIN_STEP		(0)
#define TREBLEENHANCEMENT_MAX_STEP		(ARRAY_SIZE(TrebleEnhancementValues) - 1)

#define POWERBASS_DEFAULT_IDX			(3)
#define TREBLEENHANCEMENT_DEFAULT_IDX	(3)

static const int _lblPowerBassTitleRect[4] = { 
	402-EQ_X_OFFSET, 291-EQ_Y_OFFSET, 110, 19
};

static const int _lblTrebleEnhancementTitleRect[4] = { 
	717-EQ_X_OFFSET, 291-EQ_Y_OFFSET, 199, 19
};

DEF_BTN_INFO(_btnPowerBassPlus, 
	510-EQ_X_OFFSET, 324-EQ_Y_OFFSET, 60, 60, 
	":/images/setting_bt_ctrl_plus_nor.png", 
	":/images/setting_bt_ctrl_plus_prsd.png", 
	NULL, NULL); 

DEF_BTN_INFO(_btnPowerBassMinus, 
	343-EQ_X_OFFSET, 324-EQ_Y_OFFSET, 60, 60, 
	":/images/setting_bt_ctrl_minus_nor.png", 
	":/images/setting_bt_ctrl_minus_prsd.png", 
	NULL, NULL); 

DEF_BTN_INFO(_btnTrebleEnhancementPlus, 
	870-EQ_X_OFFSET, 324-EQ_Y_OFFSET, 60, 60, 
	":/images/setting_bt_ctrl_plus_nor.png", 
	":/images/setting_bt_ctrl_plus_prsd.png", 
	NULL, NULL); 

DEF_BTN_INFO(_btnTrebleEnhancementMinus, 
	703-EQ_X_OFFSET, 324-EQ_Y_OFFSET, 60, 60, 
	":/images/setting_bt_ctrl_minus_nor.png", 
	":/images/setting_bt_ctrl_minus_prsd.png", 
	NULL, NULL); 

DEF_RECT_INFO(_btnPowerBassStatus, 420-EQ_X_OFFSET, 336-EQ_Y_OFFSET, 74, 36);
DEF_RECT_INFO(_btnTrebleEnhancementStatus, 780-EQ_X_OFFSET, 336-EQ_Y_OFFSET, 74, 36);

static const char *_PowerBassTrebleEnhancementStatusImageNames[] =
{
	":/images/setting_gage3_0.png", 
	":/images/setting_gage3_1.png", 
	":/images/setting_gage3_2.png", 
	":/images/setting_gage3_3.png", 
	":/images/setting_gage3_4.png", 
	":/images/setting_gage3_5.png", 
};

PowerBassTrebleEnhancementManager::PowerBassTrebleEnhancementManager(QWidget *parent) :
	QWidget(parent)
{
    m_lblPowerBassTitle = new TCFontLabel(this);
    m_btnPowerBassPlus = new TCImageButton(this);
    m_btnPowerBassMinus = new TCImageButton(this);
    m_btnPowerBassStatus = new TCImageButton(this);

    m_lblTrebleEnhancementTitle = new TCFontLabel(this);
    m_btnTrebleEnhancementPlus = new TCImageButton(this);
    m_btnTrebleEnhancementMinus = new TCImageButton(this);
    m_btnTrebleEnhancementStatus = new TCImageButton(this);

	m_curPowerBassIdx = POWERBASS_DEFAULT_IDX;
	m_curTrebleEnhancementIdx = TREBLEENHANCEMENT_DEFAULT_IDX;
}

PowerBassTrebleEnhancementManager::~PowerBassTrebleEnhancementManager()
{
	delete m_lblPowerBassTitle;
	delete m_btnPowerBassPlus;
	delete m_btnPowerBassMinus;
	delete m_btnPowerBassStatus;

	delete m_lblTrebleEnhancementTitle;
	delete m_btnTrebleEnhancementPlus;
	delete m_btnTrebleEnhancementMinus;
	delete m_btnTrebleEnhancementStatus;
}

void PowerBassTrebleEnhancementManager::Initialize(int x, int y, int screenWidth, int screenHeight)
{
	this->setWindowFlags(Qt::FramelessWindowHint);

	this->setGeometry(x, y, screenWidth, screenHeight);
	
	UpdateImages();
    UpdateGeometry();

	InitializeSignalSlots();
	SetCurruntValues();
}

void PowerBassTrebleEnhancementManager::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
}

void PowerBassTrebleEnhancementManager::hideEvent(QHideEvent * event)
{
	QWidget::hideEvent(event);
}

void PowerBassTrebleEnhancementManager::UpdateGeometry()
{
	m_lblPowerBassTitle->setGeometry(_lblPowerBassTitleRect[0], _lblPowerBassTitleRect[1],
									_lblPowerBassTitleRect[2], _lblPowerBassTitleRect[3]);

	m_lblTrebleEnhancementTitle->setGeometry(_lblTrebleEnhancementTitleRect[0], _lblTrebleEnhancementTitleRect[1],
											_lblTrebleEnhancementTitleRect[2], _lblTrebleEnhancementTitleRect[3]);

	SET_RECT_GEOMETRY(m_btnPowerBassPlus, _btnPowerBassPlus);
	SET_RECT_GEOMETRY(m_btnPowerBassMinus, _btnPowerBassMinus);
	SET_RECT_GEOMETRY(m_btnTrebleEnhancementPlus, _btnTrebleEnhancementPlus);
	SET_RECT_GEOMETRY(m_btnTrebleEnhancementMinus, _btnTrebleEnhancementMinus);

	SET_RECT_GEOMETRY(m_btnPowerBassStatus, _btnPowerBassStatus);
	SET_RECT_GEOMETRY(m_btnTrebleEnhancementStatus, _btnTrebleEnhancementStatus);
}

void PowerBassTrebleEnhancementManager::UpdateImages()
{
	m_lblPowerBassTitle->SetPixelSize(21);
	m_lblPowerBassTitle->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblPowerBassTitle->setText("Power Bass");
	m_lblPowerBassTitle->setAlignment(Qt::AlignCenter);

	m_lblTrebleEnhancementTitle->SetPixelSize(21);
	m_lblTrebleEnhancementTitle->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblTrebleEnhancementTitle->setText("Treble Enhancement");
	m_lblTrebleEnhancementTitle->setAlignment(Qt::AlignCenter);

	SET_BTN_IMAGES(m_btnPowerBassPlus, _btnPowerBassPlus);
	SET_BTN_IMAGES(m_btnPowerBassMinus, _btnPowerBassMinus);
	SET_BTN_IMAGES(m_btnTrebleEnhancementPlus, _btnTrebleEnhancementPlus);
	SET_BTN_IMAGES(m_btnTrebleEnhancementMinus, _btnTrebleEnhancementMinus);

	SetStatusImage(m_btnPowerBassStatus, m_curPowerBassIdx);
	SetStatusImage(m_btnTrebleEnhancementStatus, m_curTrebleEnhancementIdx);
}

void PowerBassTrebleEnhancementManager::SetStatusImage(TCImageButton *btn, int idx)
{
	btn->SetImage(_PowerBassTrebleEnhancementStatusImageNames[idx]);
}

void PowerBassTrebleEnhancementManager::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	connect(SETTING_IF_MANAGER, SIGNAL(SigKeyboardClicked(int)), this, SLOT(OnKeyboardClicked(int)), dbusConnectionType);

	connect(m_btnPowerBassPlus, SIGNAL(mouseClicked()), this, SLOT(OnPowerBassPlusClicked()));
	connect(m_btnPowerBassMinus, SIGNAL(mouseClicked()), this, SLOT(OnPowerBassMinusClicked()));
	connect(m_btnTrebleEnhancementPlus, SIGNAL(mouseClicked()), this, SLOT(OnTrebleEnhancementPlusClicked()));
	connect(m_btnTrebleEnhancementMinus, SIGNAL(mouseClicked()), this, SLOT(OnTrebleEnhancementMinusClicked()));

	connect(this, SIGNAL(SigSetPowerBass(int)), SETTING_IF_MANAGER, SLOT(OnSetPowerBass(int)));
	connect(this, SIGNAL(SigSetTrebleEnhancement(int)), SETTING_IF_MANAGER, SLOT(OnSetTrebleEnhancement(int)));
}

void PowerBassTrebleEnhancementManager::OnPowerBassPlusClicked(void)
{
	PT_MANAGER_PROCESS_PRINTF("POWERBASS_MAX_STEP : %d\n", POWERBASS_MAX_STEP);
	m_curPowerBassIdx++;
	if (m_curPowerBassIdx > POWERBASS_MAX_STEP) {
		m_curPowerBassIdx = POWERBASS_MAX_STEP;
	}

	emit SigSetPowerBass(PowerBassValues[m_curPowerBassIdx]);

	SetStatusImage(m_btnPowerBassStatus, m_curPowerBassIdx);

	PT_MANAGER_PROCESS_PRINTF("m_curPowerBassIdx : %d\n", m_curPowerBassIdx);
}

void PowerBassTrebleEnhancementManager::OnPowerBassMinusClicked(void)
{
	m_curPowerBassIdx--;
	if (m_curPowerBassIdx < POWERBASS_MIN_STEP) {
		m_curPowerBassIdx = POWERBASS_MIN_STEP;
	}

	emit SigSetPowerBass(PowerBassValues[m_curPowerBassIdx]);

	SetStatusImage(m_btnPowerBassStatus, m_curPowerBassIdx);

	PT_MANAGER_PROCESS_PRINTF("m_curPowerBassIdx : %d\n", m_curPowerBassIdx);
}

void PowerBassTrebleEnhancementManager::OnTrebleEnhancementPlusClicked(void)
{
	PT_MANAGER_PROCESS_PRINTF("TREBLEENHANCEMENT_MAX_STEP : %d\n", TREBLEENHANCEMENT_MAX_STEP);
	m_curTrebleEnhancementIdx++;
	if (m_curTrebleEnhancementIdx > TREBLEENHANCEMENT_MAX_STEP) {
		m_curTrebleEnhancementIdx = TREBLEENHANCEMENT_MAX_STEP;
	}

	emit SigSetTrebleEnhancement(TrebleEnhancementValues[m_curTrebleEnhancementIdx]);

	SetStatusImage(m_btnTrebleEnhancementStatus, m_curTrebleEnhancementIdx);

	PT_MANAGER_PROCESS_PRINTF("m_curTrebleEnhancementIdx : %d\n", m_curTrebleEnhancementIdx);
}

void PowerBassTrebleEnhancementManager::OnTrebleEnhancementMinusClicked(void)
{
	m_curTrebleEnhancementIdx--;
	if (m_curTrebleEnhancementIdx < TREBLEENHANCEMENT_MIN_STEP) {
		m_curTrebleEnhancementIdx = TREBLEENHANCEMENT_MIN_STEP;
	}

	emit SigSetTrebleEnhancement(TrebleEnhancementValues[m_curTrebleEnhancementIdx]);

	SetStatusImage(m_btnTrebleEnhancementStatus, m_curTrebleEnhancementIdx);

	PT_MANAGER_PROCESS_PRINTF("m_curTrebleEnhancementIdx : %d\n", m_curTrebleEnhancementIdx);
}

void PowerBassTrebleEnhancementManager::SetCurruntValues(void)
{
	emit SigSetPowerBass(PowerBassValues[m_curPowerBassIdx]);
	emit SigSetTrebleEnhancement(TrebleEnhancementValues[m_curTrebleEnhancementIdx]);

	SetStatusImage(m_btnPowerBassStatus, m_curPowerBassIdx);
	SetStatusImage(m_btnTrebleEnhancementStatus, m_curTrebleEnhancementIdx);

	PT_MANAGER_PROCESS_PRINTF("m_curPowerBassIdx : %d, m_curTrebleEnhancementIdx : %d\n", 
		m_curPowerBassIdx, m_curTrebleEnhancementIdx);
}

void PowerBassTrebleEnhancementManager::OnKeyboardClicked(int key)
{
	PT_MANAGER_PROCESS_PRINTF("key: %d\n", key);

	if (this->isVisible()) {
	}
}

