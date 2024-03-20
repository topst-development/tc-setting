/****************************************************************************************
 *   FileName    : EQController.cpp
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

#include "EQController.h"
#include "SettingIFManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <libxml/parser.h>

extern int g_debug;

#define EQ_CTRL_MANAGER_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[EQ_CTRL_MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}


#ifdef __cplusplus
extern "C" {
#endif
#ifdef ENABLE_TSOUND
    #include "am3d/tc_audio_dsp.h"
#endif
#ifdef __cplusplus
}
#endif

#define CHANNEL_MASK		(ZIRENE_PB_ALL_CHANNELS)

EQController::EQController()
{
	m_fd = -1;
}

EQController::~EQController()
{
}

int EQController::Initialize(void)
{
	m_fd = open_audio_dsp();
	if (m_fd < 0) 
		return -1;
	
	if (UpdateValues() < 0)
		return -1;
	
	return 0;
}

void EQController::Deinitialize(void)
{
	close_audio_dsp(m_fd);
}

int EQController::UpdateValues(void)
{
	int ret;
	int value = 0;

	if (m_fd < 0) 
		return -1;

	ret = get_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND2_GAIN_IN_DB, CHANNEL_MASK, &value);
	if(ret != IOCTL_ERROR)
	{
		m_band_eq.low_bass = value / 2;
	}
	ret = get_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND4_GAIN_IN_DB, CHANNEL_MASK, &value);
	if(ret != IOCTL_ERROR)
	{
		m_band_eq.bass = value / 2;
	}
	ret = get_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND6_GAIN_IN_DB, CHANNEL_MASK, &value);
	if(ret != IOCTL_ERROR)
	{
		m_band_eq.mid = value / 2;
	}
	ret = get_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND8_GAIN_IN_DB, CHANNEL_MASK, &value);
	if(ret != IOCTL_ERROR)
	{
		m_band_eq.low_treble = value / 2;
	}
	ret = get_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND12_GAIN_IN_DB, CHANNEL_MASK, &value);
	if(ret != IOCTL_ERROR)
	{
		m_band_eq.treble = value / 2;
	}
	ret = get_effect_param(m_fd, ZIRENE_POWERBASS, ZIRENE_POWERBASS_MAX_GAIN_IN_DB, CHANNEL_MASK, &value);
	if(ret != IOCTL_ERROR)
	{
		m_power_bass = value;
	}
	ret = get_effect_param(m_fd, ZIRENE_TREBLEENHANCEMENT, ZIRENE_TREBLEENHANCEMENT_MAX_GAIN_IN_DB, CHANNEL_MASK, &value);
	if(ret != IOCTL_ERROR)
	{
		m_treble_enhancement = value;
	}

	(void)get_fader_param(m_fd, &m_balance, &m_fade);

	return 0;
}


int EQController::GetBandEQLowBass(void)
{
	return m_band_eq.low_bass;
}

int EQController::GetBandEQBass(void)
{
	return m_band_eq.bass;
}

int EQController::GetBandEQMid(void)
{
	return m_band_eq.mid;
}

int EQController::GetBandEQLowTreble(void)
{
	return m_band_eq.low_treble;
}

int EQController::GetBandEQTreble(void)
{
	return m_band_eq.treble;
}

int EQController::GetPowerBass(void)
{
	return m_power_bass;
}

int EQController::GetTrebleEnhancement(void)
{
	return m_treble_enhancement;
}

int EQController::GetBalance(void)
{
	return m_balance;
}

int EQController::GetFade(void)
{
	return m_fade;
}

int EQController::SetBandEQLowBass(int value)
{
	int val = value * 2;

	if (val < ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MIN)
		return -1;

	if (val > ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MAX)
		return -1;

	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND1_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;
	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND2_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;
	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND3_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;

	m_band_eq.low_bass = value;
	EQ_CTRL_MANAGER_PROCESS_PRINTF("val:%d\n", value);

	return 0;
}

int EQController::SetBandEQBass(int value)
{
	int val = value * 2;

	if (val < ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MIN)
		return -1;

	if (val > ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MAX)
		return -1;

	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND4_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;
	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND5_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;

	m_band_eq.bass = value;
	EQ_CTRL_MANAGER_PROCESS_PRINTF("val:%d\n", value);

	return 0;
}

int EQController::SetBandEQMid(int value)
{
	int val = value * 2;

	if (val < ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MIN)
		return -1;

	if (val > ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MAX)
		return -1;

	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND6_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;
	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND7_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;

	m_band_eq.mid = value;
	EQ_CTRL_MANAGER_PROCESS_PRINTF("val:%d\n", value);

	return 0;
}

int EQController::SetBandEQLowTreble(int value)
{
	int val = value * 2;

	if (val < ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MIN)
		return -1;

	if (val > ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MAX)
		return -1;

	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND8_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;
	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND9_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;

	m_band_eq.low_treble = value;
	EQ_CTRL_MANAGER_PROCESS_PRINTF("val:%d\n", value);

	return 0;
}

int EQController::SetBandEQTreble(int value)
{
	int val = value * 2;

	if (val < ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MIN)
		return -1;

	if (val > ZIRENE_GRAPHICEQ_BAND_GAIN_IN_DB_MAX)
		return -1;

	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND10_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;
	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND11_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;
	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND12_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;
	if (set_effect_param(m_fd, ZIRENE_GRAPHICEQ, ZIRENE_GRAPHICEQ_BAND13_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;

	m_band_eq.treble = value;
	EQ_CTRL_MANAGER_PROCESS_PRINTF("val:%d\n", value);

	return 0;
}

int EQController::SetPowerBass(int val)
{
	if (val < ZIRENE_POWERBASS_MAX_GAIN_IN_DB_MIN)
		return -1;

	if (val > ZIRENE_POWERBASS_MAX_GAIN_IN_DB_MAX)
		return -1;

	if (set_effect_param(m_fd, ZIRENE_POWERBASS, ZIRENE_POWERBASS_MAX_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;

	m_power_bass = val;
	EQ_CTRL_MANAGER_PROCESS_PRINTF("val:%d\n", val);

	return 0;
}

int EQController::SetTrebleEnhancement(int val)
{
	if (val < ZIRENE_TREBLEENHANCEMENT_MAX_GAIN_IN_DB_MIN)
		return -1;

	if (val > ZIRENE_TREBLEENHANCEMENT_MAX_GAIN_IN_DB_MAX)
		return -1;

	if (set_effect_param(m_fd, ZIRENE_TREBLEENHANCEMENT, ZIRENE_TREBLEENHANCEMENT_MAX_GAIN_IN_DB, CHANNEL_MASK, val) < 0)
		return -1;

	m_treble_enhancement= val;
	EQ_CTRL_MANAGER_PROCESS_PRINTF("val:%d\n", val);

	return 0;
}

int EQController::SetBalance(int val)
{
	if (val < TC_BALANCEFADE_FADE_IN_PERCENT_MIN)
		return -1;

	if (val > TC_BALANCEFADE_FADE_IN_PERCENT_MAX)
		return -1;

	if (set_fader_param(m_fd, val, m_fade) < 0)
		return -1;

	m_balance = val;
	EQ_CTRL_MANAGER_PROCESS_PRINTF("val:%d\n", val);

	return 0;
}

int EQController::SetFade(int val)
{
	if (val < TC_BALANCEFADE_FADE_IN_PERCENT_MIN)
		return -1;

	if (val > TC_BALANCEFADE_FADE_IN_PERCENT_MAX)
		return -1;

	if (set_fader_param(m_fd, m_balance, val) < 0)
		return -1;

	m_fade = val;
	EQ_CTRL_MANAGER_PROCESS_PRINTF("val:%d\n", val);

	return 0;
}
