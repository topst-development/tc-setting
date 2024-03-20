/****************************************************************************************
 *	 FileName	 : ColorAdjustmentManager.h
 *	 Description : ColorAdjustment Header File
 ****************************************************************************************
 *
 *	 TCC Version 1.0
 *	 Copyright (c) Telechips Inc.
 *	 All rights reserved 
 
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

#ifndef COLOR_ADJUSTMENT_MANAGER_H
#define COLOR_ADJUSTMENT_MANAGER_H

#include <QObject>

class ColorAdjustmentManager : public QObject
{
	Q_OBJECT

	struct temperatureInfo {
		int _value;
		int _onOff;
	};

	struct hsvInfo {
		int _hueValue;
		int _saturationValue;
		int _onOff;
	};

	public:
		ColorAdjustmentManager(QObject *parent = 0);
		~ColorAdjustmentManager();
		void Initialize(void);

		Q_INVOKABLE void downTemperature(void);
		Q_INVOKABLE void upTemperature(void);
		Q_INVOKABLE void downHue(void);
		Q_INVOKABLE void upHue(void);
		Q_INVOKABLE void downSaturation(void);
		Q_INVOKABLE void upSaturation(void);
		Q_INVOKABLE void clickTempLUTDISP0(void);
		Q_INVOKABLE void clickTempLUTDISP1(void);
		Q_INVOKABLE void clickTempLUTDISP2(void);
		Q_INVOKABLE void clickTempLUTVIDEO(void);
		Q_INVOKABLE void clickTempLUTOnOff(void);
		Q_INVOKABLE void clickHSV3DLUTDISP0(void);
		Q_INVOKABLE void clickHSV3DLUTDISP1(void);
		Q_INVOKABLE void clickHSV3DLUTOnOff(void);

	signals:
#ifdef DISABLE_HUESATURATION
		void hueSaturationDisabled(void);
#endif
		void temperatureChanged(int value);
		void hueChanged(int value);
		void saturationChanged(int value);
		void tempDisp0Changed(void);
		void tempDisp1Changed(void);
		void tempDisp2Changed(void);
		void tempVideoChanged(void);
		void tempOnChanged(void);
		void tempOffChanged(void);
		void hsvDisp0Changed(void);
		void hsvDisp1Changed(void);
		void hsvOnChanged(void);
		void hsvOffChanged(void);

	private:
		int									m_TemperatureComponent;
		int									m_HSVComponent;
		struct temperatureInfo				s_TemperatureInfo[4];
		struct hsvInfo						s_HSVInfo[2];
};
#endif
