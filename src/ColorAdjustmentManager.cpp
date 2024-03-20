/****************************************************************************************
 *   FileName    : ColorAdjustmentManager.cpp
 *   Description : Set ColorAdjustment Function
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
#include "Setting.h"
#include "SettingIFManager.h"
#include "ColorAdjustmentManager.h"
#include "tc_api_color_adjustment.h"

extern int g_debug;

#define MAX_OBJECT_COUNT 11

#define CA_MANAGER_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[COLOR_ADJUSTMENT_MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

static const int kelvinValue[MAX_OBJECT_COUNT] = {
	6500, 5970, 5440, 4910, 4380, 3850, 3320, 2790, 2260, 1730, 1200
};

static const int hueValue[MAX_OBJECT_COUNT] = {
	-128, -102, -77, -51, -26, 0, 26, 51, 77, 102, 127
};

static const int saturationValue[MAX_OBJECT_COUNT] = {
	0, 102, 205, 307, 410, 512, 615, 717, 820, 922, 1024
};

ColorAdjustmentManager::ColorAdjustmentManager(QObject *parent) :
	QObject(parent)
{
}

ColorAdjustmentManager::~ColorAdjustmentManager()
{
}

void ColorAdjustmentManager::Initialize(void)
{
	int i;

	tc_api_color_initialize();
	tc_api_set_log_level(2); /* Info log level */

	m_TemperatureComponent = 0;
	m_HSVComponent = 0;
	for (i = 0; i < 4; i++)
	{
		s_TemperatureInfo[i]._value = 1;
		s_TemperatureInfo[i]._onOff = 0;
	}

#ifdef DISABLE_HUESATURATION
	emit hueSaturationDisabled();
#endif

	for (i = 0; i < 2; i++)
	{
		s_HSVInfo[i]._hueValue = 5;
		s_HSVInfo[i]._saturationValue = 0;
		s_HSVInfo[i]._onOff = 0;

		/* Default 3D LUT Set */
		(void) tc_api_color_set_hue_saturation(hueValue[s_HSVInfo[i]._hueValue],
				saturationValue[s_HSVInfo[i]._saturationValue], i);
	}
}

void ColorAdjustmentManager::downTemperature(void)
{
	int ret;
	int plugin_num = 0;
	struct temperatureInfo *info;

	if ((m_TemperatureComponent >= 0) && (m_TemperatureComponent < 4))
	{
		info = &s_TemperatureInfo[m_TemperatureComponent];
		if (info->_value > 0) {
			info->_value--;

			if(m_TemperatureComponent == DISP_COMP0) {
				plugin_num = PLUGIN_RDMA03;
			}
			ret = tc_api_color_set_color_temperature(kelvinValue[info->_value],
					m_TemperatureComponent, plugin_num);
			CA_MANAGER_PROCESS_PRINTF("Set Temperature %d kelvin, %d component Ret:%d\n",
				kelvinValue[info->_value], m_TemperatureComponent, ret);
			if ( ret == 0 ) {
				info->_onOff = 1;
				emit tempOnChanged();
			}
		}
		CA_MANAGER_PROCESS_PRINTF("Temperature(%d, %4d kelvin)\n", info->_value, kelvinValue[info->_value]);
		emit temperatureChanged(info->_value);
	}
}

void ColorAdjustmentManager::upTemperature(void)
{
	int ret;
	int plugin_num = 0;
	struct temperatureInfo *info;

	if ((m_TemperatureComponent >= 0) && (m_TemperatureComponent < 4))
	{
		info = &s_TemperatureInfo[m_TemperatureComponent];
		if (info->_value < (MAX_OBJECT_COUNT-1)) {
			info->_value++;

			if(m_TemperatureComponent == DISP_COMP0) {
				plugin_num = PLUGIN_RDMA03;
			}
			ret = tc_api_color_set_color_temperature(kelvinValue[info->_value],
					m_TemperatureComponent, plugin_num);
			CA_MANAGER_PROCESS_PRINTF("Set Temperature %d kelvin, %d component Ret:%d\n",
				kelvinValue[info->_value], m_TemperatureComponent, ret);
			if ( ret == 0 ) {
				info->_onOff = 1;
				emit tempOnChanged();
			}
		}
		CA_MANAGER_PROCESS_PRINTF("Temperature(%d, %4d kelvin)\n", info->_value, kelvinValue[info->_value]);
		emit temperatureChanged(info->_value);


	}
}

void ColorAdjustmentManager::downHue(void)
{
	int ret;
	struct hsvInfo *info;

	if ((m_HSVComponent >= 0) && (m_HSVComponent < 4))
	{
		info = &s_HSVInfo[m_HSVComponent];
		if (info->_hueValue > 0) {
			info->_hueValue--;
			ret = tc_api_color_set_hue_saturation(hueValue[info->_hueValue],
					saturationValue[info->_saturationValue], m_HSVComponent);
			CA_MANAGER_PROCESS_PRINTF("Set Hue(%d angle) Saturation(%d gain), %d component Ret:%d\n",
				hueValue[info->_hueValue], saturationValue[info->_saturationValue], m_HSVComponent, ret);
			if(info->_onOff == 0)
			{
				info->_onOff = 1;
				ret = tc_api_color_on_off_3d_lut(info->_onOff, m_HSVComponent);
				CA_MANAGER_PROCESS_PRINTF("HSV 3D LUT On %d component Ret:%d\n",
					m_HSVComponent, ret);
				emit hsvOnChanged();
			}
		}
		CA_MANAGER_PROCESS_PRINTF("HUE(%d, %4d angle)\n", info->_hueValue, hueValue[info->_hueValue]);
		emit hueChanged(info->_hueValue);
	}
}

void ColorAdjustmentManager::upHue(void)
{
	int ret;
	struct hsvInfo *info;

	if ((m_HSVComponent >= 0) && (m_HSVComponent < 4))
	{
		info = &s_HSVInfo[m_HSVComponent];
		if (info->_hueValue < (MAX_OBJECT_COUNT-1)) {
			info->_hueValue++;
			ret = tc_api_color_set_hue_saturation(hueValue[info->_hueValue],
					saturationValue[info->_saturationValue], m_HSVComponent);
			CA_MANAGER_PROCESS_PRINTF("Set Hue(%d angle) Saturation(%d gain), %d component Ret:%d\n",
				hueValue[info->_hueValue], saturationValue[info->_saturationValue], m_HSVComponent, ret);
			if(info->_onOff == 0)
			{
				info->_onOff = 1;
				ret = tc_api_color_on_off_3d_lut(info->_onOff, m_HSVComponent);
				CA_MANAGER_PROCESS_PRINTF("HSV 3D LUT On %d component Ret:%d\n",
					m_HSVComponent, ret);
				emit hsvOnChanged();
			}
		}
		CA_MANAGER_PROCESS_PRINTF("HUE(%d, %4d angle)\n", info->_hueValue, hueValue[info->_hueValue]);
		emit hueChanged(info->_hueValue);
	}
}

void ColorAdjustmentManager::downSaturation(void)
{
	int ret;
	struct hsvInfo *info;

	if ((m_HSVComponent >= 0) && (m_HSVComponent < 4))
	{
		info = &s_HSVInfo[m_HSVComponent];
		if (info->_saturationValue > 0) {
			info->_saturationValue--;
			ret = tc_api_color_set_hue_saturation(hueValue[info->_hueValue],
					saturationValue[info->_saturationValue], m_HSVComponent);
			CA_MANAGER_PROCESS_PRINTF("Set Hue(%d angle) Saturation(%d gain), %d component Ret:%d\n",
				hueValue[info->_hueValue], saturationValue[info->_saturationValue], m_HSVComponent, ret);
			if(info->_onOff == 0)
			{
				info->_onOff = 1;
				ret = tc_api_color_on_off_3d_lut(info->_onOff, m_HSVComponent);
				CA_MANAGER_PROCESS_PRINTF("HSV 3D LUT On %d component Ret:%d\n",
					m_HSVComponent, ret);
				emit hsvOnChanged();
			}
		}
		CA_MANAGER_PROCESS_PRINTF("Saturation(%d, %4d gain)\n", info->_saturationValue, saturationValue[info->_saturationValue]);
		emit saturationChanged(info->_saturationValue);
	}
}

void ColorAdjustmentManager::upSaturation(void)
{
	int ret;
	struct hsvInfo *info;

	if ((m_HSVComponent >= 0) && (m_HSVComponent < 4))
	{
		info = &s_HSVInfo[m_HSVComponent];
		if (info->_saturationValue < (MAX_OBJECT_COUNT-1)) {
			info->_saturationValue++;
			ret = tc_api_color_set_hue_saturation(hueValue[info->_hueValue],
					saturationValue[info->_saturationValue], m_HSVComponent);
			CA_MANAGER_PROCESS_PRINTF("Set Hue(%d angle) Saturation(%d gain), %d component Ret:%d\n",
				hueValue[info->_hueValue], saturationValue[info->_saturationValue], m_HSVComponent, ret);
			if(info->_onOff == 0)
			{
				info->_onOff = 1;
				ret = tc_api_color_on_off_3d_lut(info->_onOff, m_HSVComponent);
				CA_MANAGER_PROCESS_PRINTF("HSV 3D LUT On %d component Ret:%d\n",
					m_HSVComponent, ret);
				emit hsvOnChanged();
			}
		}
		CA_MANAGER_PROCESS_PRINTF("Saturation(%d, %4d gain)\n", info->_saturationValue, saturationValue[info->_saturationValue]);
		emit saturationChanged(info->_saturationValue);
	}
}

void ColorAdjustmentManager::clickTempLUTDISP0(void)
{
	struct temperatureInfo *info;

	m_TemperatureComponent = DISP_DEV0;
	info = &s_TemperatureInfo[m_TemperatureComponent];

	CA_MANAGER_PROCESS_PRINTF("Temperature Disp0\n");

	emit temperatureChanged(info->_value);
	emit tempDisp0Changed();
	if (info->_onOff == 1) {
		emit tempOnChanged();
	}
	else {
		emit tempOffChanged();
	}
}

void ColorAdjustmentManager::clickTempLUTDISP1(void)
{
	struct temperatureInfo *info;

	m_TemperatureComponent = DISP_DEV1;
	info = &s_TemperatureInfo[m_TemperatureComponent];

	CA_MANAGER_PROCESS_PRINTF("Temperature Disp1\n");

	emit temperatureChanged(info->_value);
	emit tempDisp1Changed();
	if (info->_onOff == 1) {
		emit tempOnChanged();
	}
	else {
		emit tempOffChanged();
	}
}

void ColorAdjustmentManager::clickTempLUTDISP2(void)
{
	struct temperatureInfo *info;

	m_TemperatureComponent = DISP_DEV2;
	info = &s_TemperatureInfo[m_TemperatureComponent];

	CA_MANAGER_PROCESS_PRINTF("Temperature Disp2\n");

	emit temperatureChanged(info->_value);
	emit tempDisp2Changed();
	if (info->_onOff == 1) {
		emit tempOnChanged();
	}
	else {
		emit tempOffChanged();
	}
}

void ColorAdjustmentManager::clickTempLUTVIDEO(void)
{
	struct temperatureInfo *info;

	m_TemperatureComponent = DISP_COMP0;
	info = &s_TemperatureInfo[m_TemperatureComponent];

	CA_MANAGER_PROCESS_PRINTF("Temperature Component Video\n");

	emit temperatureChanged(info->_value);
	emit tempVideoChanged();
	if (info->_onOff == 1) {
		emit tempOnChanged();
	}
	else {
		emit tempOffChanged();
	}
}

void ColorAdjustmentManager::clickTempLUTOnOff(void)
{
	int ret;
	int plugin_num = 0;
	int value;
	struct temperatureInfo *info;

	if ((m_TemperatureComponent >= 0) && (m_TemperatureComponent < 4))
	{
		info = &s_TemperatureInfo[m_TemperatureComponent];
		if(info->_onOff == 0)
		{
			value = kelvinValue[info->_value];
			info->_onOff = 1;
			emit tempOnChanged();
		}
		else {
			value = 0;
			info->_onOff = 0;
			emit tempOffChanged();
		}
		if(m_TemperatureComponent == DISP_COMP0) {
			plugin_num = PLUGIN_RDMA03;
		}
		ret = tc_api_color_set_color_temperature(value, m_TemperatureComponent, plugin_num);
		CA_MANAGER_PROCESS_PRINTF("Set Temperature OnOff, %d component Ret:%d\n",
			m_TemperatureComponent, ret);
	}
}

void ColorAdjustmentManager::clickHSV3DLUTDISP0()
{
	struct hsvInfo *info;

	m_HSVComponent = DISP_DEV0;
	info = &s_HSVInfo[m_HSVComponent];

	CA_MANAGER_PROCESS_PRINTF("HSV Disp0\n");

	emit hueChanged(info->_hueValue);
	emit saturationChanged(info->_saturationValue);
	emit hsvDisp0Changed();
	if (info->_onOff == 1) {
		emit hsvOnChanged();
	}
	else {
		emit hsvOffChanged();
	}
}

void ColorAdjustmentManager::clickHSV3DLUTDISP1()
{
	struct hsvInfo *info;

	m_HSVComponent = DISP_DEV1;
	info = &s_HSVInfo[m_HSVComponent];

	CA_MANAGER_PROCESS_PRINTF("HSV Disp1\n");

	emit hueChanged(info->_hueValue);
	emit saturationChanged(info->_saturationValue);
	emit hsvDisp1Changed();
	if (info->_onOff == 1) {
		emit hsvOnChanged();
	}
	else {
		emit hsvOffChanged();
	}
}

void ColorAdjustmentManager::clickHSV3DLUTOnOff()
{
	int ret;
	struct hsvInfo *info;

	if ((m_HSVComponent >= 0) && (m_HSVComponent < 2))
	{
		info = &s_HSVInfo[m_HSVComponent];
		if(info->_onOff == 0)
		{
			info->_onOff = 1;
			emit hsvOnChanged();
		}
		else {
			info->_onOff = 0;
			emit hsvOffChanged();
		}
		ret = tc_api_color_on_off_3d_lut(info->_onOff, m_HSVComponent);
		CA_MANAGER_PROCESS_PRINTF("HSV 3D LUT OnOff %d component Ret:%d\n",
			m_HSVComponent, ret);
	}
}
