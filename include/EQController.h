/****************************************************************************************
 *   FileName    : EQController.h
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

#ifndef EQ_CONTROLER_H
#define EQ_CONTROLER_H

class EQController
{
	public:
		EQController();
		~EQController();
		int Initialize(void);
		void Deinitialize(void);

		int GetBandEQLowBass(void);
		int GetBandEQBass(void);
		int GetBandEQMid(void);
		int GetBandEQLowTreble(void);
		int GetBandEQTreble(void);
		int GetPowerBass(void);
		int GetTrebleEnhancement(void);
		int GetBalance(void);
		int GetFade(void);

		int SetBandEQLowBass(int value);
		int SetBandEQBass(int value);
		int SetBandEQMid(int value);
		int SetBandEQLowTreble(int value);
		int SetBandEQTreble(int value);
		int SetPowerBass(int val);
		int SetTrebleEnhancement(int val);
		int SetBalance(int val);
		int SetFade(int val);

	private:
		int UpdateValues(void);
		int m_fd;
		struct {
			int low_bass;
			int bass;
			int mid;
			int low_treble;
			int treble;
		} m_band_eq;
		int m_power_bass;
		int m_treble_enhancement;
		int m_balance;
		int m_fade;
};
#endif 
