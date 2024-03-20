/****************************************************************************************
 *   FileName    : BandEQManager.cpp
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
#include "Setting.h"
#include "SettingIFManager.h"
#include "EQMacro.h"
#include "BandEQManager.h"

extern int g_debug;

#define BAND_EQ_MANAGER_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[BAND_EQ_MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

#define BAND_EQ_MAX_VALUE	(6)
#define BAND_EQ_MIN_VALUE	(-6)
#define BAND_EQ_DEFAULT 	(0)

static struct band_eq_t band_eq_tbl[MANUAL_MODE_IDX] = {
	[POP_MODE_IDX] = {
		.low_bass	= -1, 
		.bass		= 2, 
		.mid		= 5, 
		.low_treble	= 1, 
		.treble		= -2,
	},
	[ROCK_MODE_IDX] = {
		.low_bass	= 5, 
		.bass		= 3, 
		.mid		= -1, 
		.low_treble	= 3, 
		.treble		= 5,
	},
	[JAZZ_MODE_IDX] = {
		.low_bass	= 4, 
		.bass		= 2, 
		.mid		= -2, 
		.low_treble	= 2, 
		.treble		= 5,
	},
	[CLASSIC_MODE_IDX] = {
		.low_bass	= 5, 
		.bass		= 3, 
		.mid		= -2, 
		.low_treble	= 4, 
		.treble		= 4,
	},
	[HIPHOP_MODE_IDX] = {
		.low_bass	= 5, 
		.bass		= 3, 
		.mid		= 0, 
		.low_treble	= 1, 
		.treble		= 3,
	},
};

static const int _lblLowBassTitleRect[4] = { 
	286-EQ_X_OFFSET, 112-EQ_Y_OFFSET, 132, 18
};

static const int _lblBassTitleRect[4] = { 
	431-EQ_X_OFFSET, 112-EQ_Y_OFFSET, 132, 18
};

static const int _lblMidTitleRect[4] = { 
	576-EQ_X_OFFSET, 112-EQ_Y_OFFSET, 132, 18
};

static const int _lblLowTrebleTitleRect[4] = { 
	721-EQ_X_OFFSET, 112-EQ_Y_OFFSET, 132, 18
};

static const int _lblTrebleTitleRect[4] = { 
	866-EQ_X_OFFSET, 112-EQ_Y_OFFSET, 132, 18
};

DEF_RECT_INFO(_btnLowBassPlus, 322-EQ_X_OFFSET, 144-EQ_Y_OFFSET, 99, 53); 
DEF_RECT_INFO(_btnLowBassMinus, 322-EQ_X_OFFSET, 417-EQ_Y_OFFSET, 99, 53); 
DEF_RECT_INFO(_btnBassPlus, 467-EQ_X_OFFSET, 144-EQ_Y_OFFSET, 99, 53); 
DEF_RECT_INFO(_btnBassMinus, 467-EQ_X_OFFSET, 417-EQ_Y_OFFSET, 99, 53);
DEF_RECT_INFO(_btnMidPlus, 612-EQ_X_OFFSET, 144-EQ_Y_OFFSET, 99, 53);
DEF_RECT_INFO(_btnMidMinus, 612-EQ_X_OFFSET, 417-EQ_Y_OFFSET, 99, 53);
DEF_RECT_INFO(_btnLowTreblePlus, 757-EQ_X_OFFSET, 144-EQ_Y_OFFSET, 99, 53);
DEF_RECT_INFO(_btnLowTrebleMinus, 757-EQ_X_OFFSET, 417-EQ_Y_OFFSET, 99, 53);
DEF_RECT_INFO(_btnTreblePlus, 902-EQ_X_OFFSET, 144-EQ_Y_OFFSET, 99, 53);
DEF_RECT_INFO(_btnTrebleMinus, 902-EQ_X_OFFSET, 417-EQ_Y_OFFSET, 99, 53);

DEF_BTN_IMAGES_INFO(_btnBandEQPlus, 
	":/images/setting_bt_ctrl_plus_nor.png", 
	":/images/setting_bt_ctrl_plus_prsd.png", 
	NULL, 
	NULL); 

DEF_BTN_IMAGES_INFO(_btnBandEQMinus, 
	":/images/setting_bt_ctrl_minus_nor.png", 
	":/images/setting_bt_ctrl_minus_prsd.png", 
	NULL, 
	NULL); 

DEF_BTN_INFO(_btnPop, 
	308-EQ_X_OFFSET, 534-EQ_Y_OFFSET, 90, 35, 
	":/images/setting_bt_pop_off.png", 
	":/images/setting_bt_pop_on.png", 
	":/images/setting_bt_pop_on.png", 
	NULL); 

DEF_BTN_INFO(_btnRock, 
	424-EQ_X_OFFSET, 534-EQ_Y_OFFSET, 90, 35, 
	":/images/setting_bt_rock_off.png", 
	":/images/setting_bt_rock_on.png", 
	":/images/setting_bt_rock_on.png", 
	NULL); 

DEF_BTN_INFO(_btnJazz, 
	540-EQ_X_OFFSET, 534-EQ_Y_OFFSET, 90, 35, 
	":/images/setting_bt_jazz_off.png", 
	":/images/setting_bt_jazz_on.png", 
	":/images/setting_bt_jazz_on.png", 
	NULL); 

DEF_BTN_INFO(_btnClassic, 
	665-EQ_X_OFFSET, 534-EQ_Y_OFFSET, 90, 35, 
	":/images/setting_bt_classic_off.png", 
	":/images/setting_bt_classic_on.png", 
	":/images/setting_bt_classic_on.png", 
	NULL); 

DEF_BTN_INFO(_btnHiphop, 
	770-EQ_X_OFFSET, 534-EQ_Y_OFFSET, 90, 35, 
	":/images/setting_bt_hiphop_off.png", 
	":/images/setting_bt_hiphop_on.png", 
	":/images/setting_bt_hiphop_on.png", 
	NULL); 

DEF_BTN_INFO(_btnManual, 
	887-EQ_X_OFFSET, 534-EQ_Y_OFFSET, 90, 35, 
	":/images/setting_bt_manual_off.png", 
	":/images/setting_bt_manual_on.png", 
	":/images/setting_bt_manual_on.png", 
	NULL); 

BandEQStatusBar::BandEQStatusBar(QWidget *parent) : 
	QWidget(parent)
{
	m_btnBar = new TCImageButton(this);
	m_lblNumber = new TCFontLabel(this);
}

BandEQStatusBar::~BandEQStatusBar() {
	delete m_btnBar;
	delete m_lblNumber;
}

#define BAND_EQ_PLUS_X_OFFSET	(0)
#define BAND_EQ_PLUS_Y_OFFSET	(0)
#define BAND_EQ_PLUS_WIDTH		(35)
#define BAND_EQ_PLUS_HEIGHT		(195)

#define BAND_EQ_NUM_X_OFFSET	(0)
#define BAND_EQ_NUM_Y_OFFSET	(87)
#define BAND_EQ_NUM_WIDTH		(35)
#define BAND_EQ_NUM_HEIGHT		(19)

static const char *_BandEQPlusStatusImageNames[] =
{
	":/images/setting_gage2_0.png", 
	":/images/setting_gage2_1.png", 
	":/images/setting_gage2_2.png", 
	":/images/setting_gage2_3.png", 
	":/images/setting_gage2_4.png", 
	":/images/setting_gage2_5.png", 
	":/images/setting_gage2_6.png", 
	":/images/setting_gage2_7.png", 
	":/images/setting_gage2_8.png", 
	":/images/setting_gage2_9.png", 
	":/images/setting_gage2_10.png", 
	":/images/setting_gage2_11.png", 
	":/images/setting_gage2_12.png", 
};

static const char *_BandEQNumStatusTexts[] =
{
	"-6", 
	"-5", 
	"-4", 
	"-3", 
	"-2", 
	"-1", 
	" 0", 
	"+1", 
	"+2", 
	"+3", 
	"+4", 
	"+5", 
	"+6", 
};


void BandEQStatusBar::SetGeometry(int x, int y)
{
	m_btnBar->setGeometry(x+BAND_EQ_PLUS_X_OFFSET, y+BAND_EQ_PLUS_Y_OFFSET, BAND_EQ_PLUS_WIDTH, BAND_EQ_PLUS_HEIGHT);
	m_lblNumber->setGeometry(x+BAND_EQ_NUM_X_OFFSET, y+BAND_EQ_NUM_Y_OFFSET, BAND_EQ_NUM_WIDTH, BAND_EQ_NUM_HEIGHT);
}

int BandEQStatusBar::SetImage(int value)
{
	int num_idx = 0;

	if (value < BAND_EQ_MIN_VALUE) {
		return -1;
	}

	if (value > BAND_EQ_MAX_VALUE) {
		return -1;
	}

	num_idx = value - BAND_EQ_MIN_VALUE;

	m_btnBar->SetImage(_BandEQPlusStatusImageNames[num_idx]);
	m_lblNumber->setText(_BandEQNumStatusTexts[num_idx]);
	m_lblNumber->SetPixelSize(25);
	m_lblNumber->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblNumber->setAlignment(Qt::AlignCenter);

	return 0;
}

BandEQManager::BandEQManager(QWidget *parent) :
	QWidget(parent)
{
    m_lblLowBassTitle = new TCFontLabel(this);
    m_lblBassTitle = new TCFontLabel(this);
    m_lblMidTitle = new TCFontLabel(this);
    m_lblLowTrebleTitle = new TCFontLabel(this);
    m_lblTrebleTitle = new TCFontLabel(this);

	m_statLowBass = new BandEQStatusBar(this);
	m_statBass = new BandEQStatusBar(this);
	m_statMid = new BandEQStatusBar(this);
	m_statLowTreble = new BandEQStatusBar(this);
	m_statTreble = new BandEQStatusBar(this);

    m_btnLowBassPlus = new TCImageButton(this);
    m_btnLowBassMinus = new TCImageButton(this);
    m_btnBassPlus = new TCImageButton(this);
    m_btnBassMinus = new TCImageButton(this);
    m_btnMidPlus = new TCImageButton(this);
    m_btnMidMinus = new TCImageButton(this);
    m_btnLowTreblePlus = new TCImageButton(this);
    m_btnLowTrebleMinus = new TCImageButton(this);
    m_btnTreblePlus = new TCImageButton(this);
    m_btnTrebleMinus = new TCImageButton(this);

    m_btnPop = new TCImageButton(this);
    m_btnRock = new TCImageButton(this);
    m_btnJazz = new TCImageButton(this);
    m_btnClassic = new TCImageButton(this);
    m_btnHiphop = new TCImageButton(this);
    m_btnManual = new TCImageButton(this);

	m_curValue.low_bass = BAND_EQ_DEFAULT;
	m_curValue.bass = BAND_EQ_DEFAULT;
	m_curValue.mid = BAND_EQ_DEFAULT;
	m_curValue.low_treble = BAND_EQ_DEFAULT;
	m_curValue.treble = BAND_EQ_DEFAULT;

	m_curMode = MANUAL_MODE_IDX;
}

BandEQManager::~BandEQManager()
{
    delete m_lblLowBassTitle;
    delete m_lblBassTitle;
    delete m_lblMidTitle;
    delete m_lblLowTrebleTitle;
    delete m_lblTrebleTitle;

	delete m_statLowBass;
	delete m_statBass;
	delete m_statMid;
	delete m_statLowTreble;
	delete m_statTreble;

	delete m_btnLowBassPlus;
	delete m_btnLowBassMinus;
	delete m_btnBassPlus;
	delete m_btnBassMinus;
	delete m_btnMidPlus;
	delete m_btnMidMinus;
	delete m_btnLowTreblePlus;
	delete m_btnLowTrebleMinus;
	delete m_btnTreblePlus;
	delete m_btnTrebleMinus;

	delete m_btnPop;
	delete m_btnRock;
	delete m_btnJazz;
	delete m_btnClassic;
	delete m_btnHiphop;
	delete m_btnManual;
}

void BandEQManager::Initialize(int x, int y, int screenWidth, int screenHeight)
{
	this->setWindowFlags(Qt::FramelessWindowHint);

	this->setGeometry(x, y, screenWidth, screenHeight);
	
    UpdateGeometry();
	UpdateImages();

	m_btnManualEQManager.ClearButtons();
	m_btnManualEQManager.AddButton(m_btnLowBassPlus);
	m_btnManualEQManager.AddButton(m_btnLowBassMinus);
	m_btnManualEQManager.AddButton(m_btnBassPlus);
	m_btnManualEQManager.AddButton(m_btnBassMinus);
	m_btnManualEQManager.AddButton(m_btnMidPlus);
	m_btnManualEQManager.AddButton(m_btnMidMinus);
	m_btnManualEQManager.AddButton(m_btnLowTreblePlus);
	m_btnManualEQManager.AddButton(m_btnLowTrebleMinus);
	m_btnManualEQManager.AddButton(m_btnTreblePlus);
	m_btnManualEQManager.AddButton(m_btnTrebleMinus);
	m_btnManualEQManager.SetEnable(false);

	m_btnBandEQManager.ClearButtons();
	m_btnBandEQManager.AddButton(m_btnPop);
	m_btnBandEQManager.AddButton(m_btnRock);
	m_btnBandEQManager.AddButton(m_btnJazz);
	m_btnBandEQManager.AddButton(m_btnClassic);
	m_btnBandEQManager.AddButton(m_btnHiphop);
	m_btnBandEQManager.AddButton(m_btnManual);
	m_btnBandEQManager.SetJogKeys(g_knobKeys[TCKeyVolumeUp], g_knobKeys[TCKeyVolumeDown], g_knobKeys[TCKeyHome]);
	m_btnBandEQManager.SetEnable(false);
    
	InitializeSignalSlots();
	SetCurruntValues();
}

void BandEQManager::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);

	m_btnBandEQManager.SetEnable(true);
	m_btnBandEQManager.SetEnableJogKeys(true);

	
	if (m_curMode == POP_MODE_IDX) {
		emit SigPopClick();
	} else if (m_curMode == ROCK_MODE_IDX) {
		emit SigRockClick();
	} else if (m_curMode == JAZZ_MODE_IDX) {
		emit SigJazzClick();
	} else if (m_curMode == CLASSIC_MODE_IDX) {
		emit SigClassicClick();
	} else if (m_curMode == HIPHOP_MODE_IDX) {
		emit SigHophopClick();
	} else if (m_curMode == MANUAL_MODE_IDX) {
		emit SigManualClick();
	}
}

void BandEQManager::hideEvent(QHideEvent * event)
{
	QWidget::hideEvent(event);

	m_btnBandEQManager.SetEnable(false);
}

void BandEQManager::UpdateGeometry()
{
	m_lblLowBassTitle->setGeometry(_lblLowBassTitleRect[0], _lblLowBassTitleRect[1],
								   _lblLowBassTitleRect[2], _lblLowBassTitleRect[3]);
	m_lblBassTitle->setGeometry(_lblBassTitleRect[0], _lblBassTitleRect[1],
								_lblBassTitleRect[2], _lblBassTitleRect[3]);
	m_lblMidTitle->setGeometry(_lblMidTitleRect[0], _lblMidTitleRect[1],
							   _lblMidTitleRect[2], _lblMidTitleRect[3]);
	m_lblLowTrebleTitle->setGeometry(_lblLowTrebleTitleRect[0], _lblLowTrebleTitleRect[1],
									 _lblLowTrebleTitleRect[2], _lblLowTrebleTitleRect[3]);
	m_lblTrebleTitle->setGeometry(_lblTrebleTitleRect[0], _lblTrebleTitleRect[1],
								  _lblTrebleTitleRect[2], _lblTrebleTitleRect[3]);

	m_statLowBass->SetGeometry(334-EQ_X_OFFSET, 213-EQ_Y_OFFSET);
	m_statBass->SetGeometry(479-EQ_X_OFFSET, 213-EQ_Y_OFFSET);
	m_statMid->SetGeometry(624-EQ_X_OFFSET, 213-EQ_Y_OFFSET);
	m_statLowTreble->SetGeometry(769-EQ_X_OFFSET, 213-EQ_Y_OFFSET);
	m_statTreble->SetGeometry(914-EQ_X_OFFSET, 213-EQ_Y_OFFSET);

	SET_RECT_GEOMETRY(m_btnLowBassPlus, _btnLowBassPlus);
	SET_RECT_GEOMETRY(m_btnLowBassMinus, _btnLowBassMinus);
	SET_RECT_GEOMETRY(m_btnBassPlus, _btnBassPlus);
	SET_RECT_GEOMETRY(m_btnBassMinus, _btnBassMinus);
	SET_RECT_GEOMETRY(m_btnMidPlus, _btnMidPlus);
	SET_RECT_GEOMETRY(m_btnMidMinus, _btnMidMinus);
	SET_RECT_GEOMETRY(m_btnLowTreblePlus, _btnLowTreblePlus);
	SET_RECT_GEOMETRY(m_btnLowTrebleMinus, _btnLowTrebleMinus);
	SET_RECT_GEOMETRY(m_btnTreblePlus, _btnTreblePlus);
	SET_RECT_GEOMETRY(m_btnTrebleMinus, _btnTrebleMinus);
                       
	SET_RECT_GEOMETRY(m_btnPop, _btnPop);
	SET_RECT_GEOMETRY(m_btnRock, _btnRock);
	SET_RECT_GEOMETRY(m_btnJazz, _btnJazz);
	SET_RECT_GEOMETRY(m_btnClassic, _btnClassic);
	SET_RECT_GEOMETRY(m_btnHiphop, _btnHiphop);
	SET_RECT_GEOMETRY(m_btnManual, _btnManual);

}

void BandEQManager::UpdateImages()
{
	m_lblLowBassTitle->SetPixelSize(21);
	m_lblLowBassTitle->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblLowBassTitle->setText("Low Bass");
	m_lblLowBassTitle->setAlignment(Qt::AlignCenter);

	m_lblBassTitle->SetPixelSize(21);
	m_lblBassTitle->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblBassTitle->setText("Bass");
	m_lblBassTitle->setAlignment(Qt::AlignCenter);

	m_lblMidTitle->SetPixelSize(21);
	m_lblMidTitle->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblMidTitle->setText("Mid");
	m_lblMidTitle->setAlignment(Qt::AlignCenter);

	m_lblLowTrebleTitle->SetPixelSize(21);
	m_lblLowTrebleTitle->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblLowTrebleTitle->setText("Low Treble");
	m_lblLowTrebleTitle->setAlignment(Qt::AlignCenter);

	m_lblTrebleTitle->SetPixelSize(21);
	m_lblTrebleTitle->SetColor(QColor(0xd4, 0xd5, 0xd8));
	m_lblTrebleTitle->setText("Treble");
	m_lblTrebleTitle->setAlignment(Qt::AlignCenter);

	m_statLowBass->SetImage(m_curValue.low_bass);
	m_statBass->SetImage(m_curValue.bass);
	m_statMid->SetImage(m_curValue.mid);
	m_statLowTreble->SetImage(m_curValue.low_treble);
	m_statTreble->SetImage(m_curValue.treble);

	SET_BTN_IMAGES(m_btnLowBassPlus, _btnBandEQPlus);
	SET_BTN_IMAGES(m_btnLowBassMinus, _btnBandEQMinus);
	SET_BTN_IMAGES(m_btnBassPlus, _btnBandEQPlus);
	SET_BTN_IMAGES(m_btnBassMinus, _btnBandEQMinus);
	SET_BTN_IMAGES(m_btnMidPlus, _btnBandEQPlus);
	SET_BTN_IMAGES(m_btnMidMinus, _btnBandEQMinus);
	SET_BTN_IMAGES(m_btnLowTreblePlus, _btnBandEQPlus);
	SET_BTN_IMAGES(m_btnLowTrebleMinus, _btnBandEQMinus);
	SET_BTN_IMAGES(m_btnTreblePlus, _btnBandEQPlus);
	SET_BTN_IMAGES(m_btnTrebleMinus, _btnBandEQMinus);
                    
	SET_BTN_IMAGES(m_btnPop, _btnPop);
	SET_BTN_IMAGES(m_btnRock, _btnRock);
	SET_BTN_IMAGES(m_btnJazz, _btnJazz);
	SET_BTN_IMAGES(m_btnClassic, _btnClassic);
	SET_BTN_IMAGES(m_btnHiphop, _btnHiphop);
	SET_BTN_IMAGES(m_btnManual, _btnManual);
}

void BandEQManager::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	connect(SETTING_IF_MANAGER, SIGNAL(SigKeyboardPressed(int)), &m_btnBandEQManager, SLOT(OnKeyboardPressed(int)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(SigKeyboardClicked(int)), &m_btnBandEQManager, SLOT(OnKeyboardClicked(int)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(SigKeyboardReleased(int)), &m_btnBandEQManager, SLOT(OnKeyboardReleased(int)), dbusConnectionType);
	connect(SETTING_IF_MANAGER, SIGNAL(SigKeyboardClicked(int)), this, SLOT(OnKeyboardClicked(int)), dbusConnectionType);

	connect(m_btnPop, SIGNAL(mouseClicked()), this, SLOT(OnPopClicked()));
	connect(m_btnRock, SIGNAL(mouseClicked()), this, SLOT(OnRockClicked()));
	connect(m_btnJazz, SIGNAL(mouseClicked()), this, SLOT(OnJazzClicked()));
	connect(m_btnClassic, SIGNAL(mouseClicked()), this, SLOT(OnClassicClicked()));
	connect(m_btnHiphop, SIGNAL(mouseClicked()), this, SLOT(OnHiphopClicked()));
	connect(m_btnManual, SIGNAL(mouseClicked()), this, SLOT(OnManualClicked()));

	connect(m_btnLowBassPlus, SIGNAL(mouseClicked()), this, SLOT(OnLowBassPlusClicked()));
	connect(m_btnLowBassMinus, SIGNAL(mouseClicked()), this, SLOT(OnLowBassMinusClicked()));
	connect(m_btnBassPlus, SIGNAL(mouseClicked()), this, SLOT(OnBassPlusClicked()));
	connect(m_btnBassMinus, SIGNAL(mouseClicked()), this, SLOT(OnBassMinusClicked()));
	connect(m_btnMidPlus, SIGNAL(mouseClicked()), this, SLOT(OnMidPlusClicked()));
	connect(m_btnMidMinus, SIGNAL(mouseClicked()), this, SLOT(OnMidMinusClicked()));
	connect(m_btnLowTreblePlus, SIGNAL(mouseClicked()), this, SLOT(OnLowTreblePlusClicked()));
	connect(m_btnLowTrebleMinus, SIGNAL(mouseClicked()), this, SLOT(OnLowTrebleMinusClicked()));
	connect(m_btnTreblePlus, SIGNAL(mouseClicked()), this, SLOT(OnTreblePlusClicked()));
	connect(m_btnTrebleMinus, SIGNAL(mouseClicked()), this, SLOT(OnTrebleMinusClicked()));

	connect(m_btnLowBassPlus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));
	connect(m_btnLowBassMinus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));
	connect(m_btnBassPlus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));
	connect(m_btnBassMinus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));
	connect(m_btnMidPlus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));
	connect(m_btnMidMinus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));
	connect(m_btnLowTreblePlus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));
	connect(m_btnLowTrebleMinus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));
	connect(m_btnTreblePlus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));
	connect(m_btnTrebleMinus, SIGNAL(mouseClicked()), m_btnManual, SIGNAL(mouseClicked()));

	connect(this, SIGNAL(SigPopClick()), m_btnPop, SIGNAL(mouseClicked()));
	connect(this, SIGNAL(SigRockClick()), m_btnRock, SIGNAL(mouseClicked()));
	connect(this, SIGNAL(SigJazzClick()), m_btnJazz, SIGNAL(mouseClicked()));
	connect(this, SIGNAL(SigClassicClick()), m_btnClassic, SIGNAL(mouseClicked()));
	connect(this, SIGNAL(SigHophopClick()), m_btnHiphop, SIGNAL(mouseClicked()));
	connect(this, SIGNAL(SigManualClick()), m_btnManual, SIGNAL(mouseClicked()));

	connect(this, SIGNAL(SigSetBandEQLowBass(int)), SETTING_IF_MANAGER, SLOT(OnSetBandEQLowBass(int)));
	connect(this, SIGNAL(SigSetBandEQBass(int)), SETTING_IF_MANAGER, SLOT(OnSetBandEQBass(int)));
	connect(this, SIGNAL(SigSetBandEQMid(int)), SETTING_IF_MANAGER, SLOT(OnSetBandEQMid(int)));
	connect(this, SIGNAL(SigSetBandEQLowTreble(int)), SETTING_IF_MANAGER, SLOT(OnSetBandEQLowTreble(int)));
	connect(this, SIGNAL(SigSetBandEQTreble(int)), SETTING_IF_MANAGER, SLOT(OnSetBandEQTreble(int)));
}

void BandEQManager::OnPopClicked(void)
{
	BAND_EQ_MANAGER_PROCESS_PRINTF("\n");

	SetPreDefValues(POP_MODE_IDX);
}

void BandEQManager::OnRockClicked(void)
{
	BAND_EQ_MANAGER_PROCESS_PRINTF("\n");

	SetPreDefValues(ROCK_MODE_IDX);
}

void BandEQManager::OnJazzClicked(void)
{
	BAND_EQ_MANAGER_PROCESS_PRINTF("\n");

	SetPreDefValues(JAZZ_MODE_IDX);
}

void BandEQManager::OnClassicClicked(void)
{
	BAND_EQ_MANAGER_PROCESS_PRINTF("\n");

	SetPreDefValues(CLASSIC_MODE_IDX);
}

void BandEQManager::OnHiphopClicked(void)
{
	BAND_EQ_MANAGER_PROCESS_PRINTF("\n");

	SetPreDefValues(HIPHOP_MODE_IDX);
}

void BandEQManager::OnManualClicked(void)
{
	BAND_EQ_MANAGER_PROCESS_PRINTF("\n");

	SetPreDefValues(MANUAL_MODE_IDX);

//	m_btnManualEQManager.SetEnableButton(true);
	m_btnManualEQManager.SetEnable(false);
}

void BandEQManager::OnLowBassPlusClicked(void)
{
	m_curValue.low_bass++;
	if (m_curValue.low_bass > BAND_EQ_MAX_VALUE) {
		m_curValue.low_bass = BAND_EQ_MAX_VALUE;
	}

	emit SigSetBandEQLowBass(m_curValue.low_bass);
	m_statLowBass->SetImage(m_curValue.low_bass);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::OnLowBassMinusClicked(void)
{
	m_curValue.low_bass--;
	if (m_curValue.low_bass < BAND_EQ_MIN_VALUE) {
		m_curValue.low_bass = BAND_EQ_MIN_VALUE;
	}

	emit SigSetBandEQLowBass(m_curValue.low_bass);
	m_statLowBass->SetImage(m_curValue.low_bass);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::OnBassPlusClicked(void)
{
	m_curValue.bass++;
	if (m_curValue.bass > BAND_EQ_MAX_VALUE) {
		m_curValue.bass = BAND_EQ_MAX_VALUE;
	}

	emit SigSetBandEQBass(m_curValue.bass);
	m_statBass->SetImage(m_curValue.bass);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::OnBassMinusClicked(void)
{
	m_curValue.bass--;
	if (m_curValue.bass < BAND_EQ_MIN_VALUE) {
		m_curValue.bass = BAND_EQ_MIN_VALUE;
	}

	emit SigSetBandEQBass(m_curValue.bass);
	m_statBass->SetImage(m_curValue.bass);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::OnMidPlusClicked(void)
{
	m_curValue.mid++;
	if (m_curValue.mid > BAND_EQ_MAX_VALUE) {
		m_curValue.mid = BAND_EQ_MAX_VALUE;
	}

	emit SigSetBandEQMid(m_curValue.mid);
	m_statMid->SetImage(m_curValue.mid);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::OnMidMinusClicked(void)
{
	m_curValue.mid--;
	if (m_curValue.mid < BAND_EQ_MIN_VALUE) {
		m_curValue.mid = BAND_EQ_MIN_VALUE;
	}

	emit SigSetBandEQMid(m_curValue.mid);
	m_statMid->SetImage(m_curValue.mid);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::OnLowTreblePlusClicked(void)
{
	m_curValue.low_treble++;
	if (m_curValue.low_treble > BAND_EQ_MAX_VALUE) {
		m_curValue.low_treble = BAND_EQ_MAX_VALUE;
	}

	emit SigSetBandEQLowTreble(m_curValue.low_treble);
	m_statLowTreble->SetImage(m_curValue.low_treble);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::OnLowTrebleMinusClicked(void)
{
	m_curValue.low_treble--;
	if (m_curValue.low_treble < BAND_EQ_MIN_VALUE) {
		m_curValue.low_treble = BAND_EQ_MIN_VALUE;
	}

	emit SigSetBandEQLowTreble(m_curValue.low_treble);
	m_statLowTreble->SetImage(m_curValue.low_treble);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::OnTreblePlusClicked(void)
{
	m_curValue.treble++;
	if (m_curValue.treble > BAND_EQ_MAX_VALUE) {
		m_curValue.treble = BAND_EQ_MAX_VALUE;
	}

	emit SigSetBandEQTreble(m_curValue.treble);
	m_statTreble->SetImage(m_curValue.treble);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::OnTrebleMinusClicked(void)
{
	m_curValue.treble--;
	if (m_curValue.treble < BAND_EQ_MIN_VALUE) {
		m_curValue.treble = BAND_EQ_MIN_VALUE;
	}

	emit SigSetBandEQTreble(m_curValue.treble);
	m_statTreble->SetImage(m_curValue.treble);

	BAND_EQ_MANAGER_PROCESS_PRINTF("m_curValue : %d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

void BandEQManager::SetCurruntValues(void)
{
	emit SigSetBandEQLowBass(m_curValue.low_bass);
	emit SigSetBandEQBass(m_curValue.bass);
	emit SigSetBandEQMid(m_curValue.mid);
	emit SigSetBandEQLowTreble(m_curValue.low_treble);
	emit SigSetBandEQTreble(m_curValue.treble);

	m_statLowBass->SetImage(m_curValue.low_bass);
	m_statBass->SetImage(m_curValue.bass);
	m_statMid->SetImage(m_curValue.mid);
	m_statLowTreble->SetImage(m_curValue.low_treble);
	m_statTreble->SetImage(m_curValue.treble);

//	m_btnManualEQManager.SetEnableButton(true);

	BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
			m_curValue.low_bass, 
			m_curValue.bass, 
			m_curValue.mid, 
			m_curValue.low_treble, 
			m_curValue.treble);
}

int BandEQManager::SetPreDefValues(enum band_eq_mode_t idx)
{
	if (idx > MANUAL_MODE_IDX) {
		return -1;
	}

	m_curMode = idx;

	if (idx < MANUAL_MODE_IDX) {
		emit SigSetBandEQLowBass(band_eq_tbl[idx].low_bass);
		emit SigSetBandEQBass(band_eq_tbl[idx].bass);
		emit SigSetBandEQMid(band_eq_tbl[idx].mid);
		emit SigSetBandEQLowTreble(band_eq_tbl[idx].low_treble);
		emit SigSetBandEQTreble(band_eq_tbl[idx].treble);

		memcpy(&m_curValue, &band_eq_tbl[idx], sizeof(m_curValue));

		m_statLowBass->SetImage(band_eq_tbl[idx].low_bass);
		m_statBass->SetImage(band_eq_tbl[idx].bass);
		m_statMid->SetImage(band_eq_tbl[idx].mid);
		m_statLowTreble->SetImage(band_eq_tbl[idx].low_treble);
		m_statTreble->SetImage(band_eq_tbl[idx].treble);

		//	m_btnManualEQManager.SetEnableButton(false);

		BAND_EQ_MANAGER_PROCESS_PRINTF("%d, %d, %d, %d, %d\n", 
				m_curValue.low_bass, 
				m_curValue.bass, 
				m_curValue.mid, 
				m_curValue.low_treble, 
				m_curValue.treble);
	}

	return 0;
}

void BandEQManager::OnBandEQUnControlMode(void)
{
	BAND_EQ_MANAGER_PROCESS_PRINTF("On Band EQ Mode\n");

	m_btnBandEQManager.SetEnableJogKeys(false);
}
void BandEQManager::OnBandEQControlMode(void)
{
	BAND_EQ_MANAGER_PROCESS_PRINTF("On Band EQ Mode\n");

	m_btnBandEQManager.SetEnableJogKeys(true);
}

void BandEQManager::OnKeyboardClicked(int key)
{
	BAND_EQ_MANAGER_PROCESS_PRINTF("key: %d\n", key);

	if (this->isVisible()) {
		if (key == g_knobKeys[TCKeyHome] && !m_btnBandEQManager.IsFocusing()) {
			emit SigEQButtonEnable();
		}
	}
}
