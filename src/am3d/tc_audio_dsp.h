/****************************************************************************************
 *   FileName    : tc_audio_dsp.h
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

#ifndef _TCC_AUDIO_DSP_H_
#define _TCC_AUDIO_DSP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <asm/ioctl.h>
#include "AM3DOSAL.h"
#include "AM3DZirene.h"

#include "tcc_mbox_audio_pcm_def.h"
#include "tcc_mbox_am3d_effect_def.h"
#include "tcc_mbox_audio_ioctl.h"

#define IOCTL_ERROR (-1)
#define TC_BALANCEFADE_FADE_IN_PERCENT_MIN	(-100)
#define TC_BALANCEFADE_FADE_IN_PERCENT_MAX	(100)

int open_audio_dsp(void);
void close_audio_dsp(int fd);

int set_effect_param(int fd, int effect_id, int param_id, int channelmask, int value);
int get_effect_param(int fd, int effect_id, int param_id, int channelmask, int *value);
int set_fader_param(int fd, int balance, int fade);
int get_fader_param(int fd, int *balance, int *fade);

#ifdef __cplusplus
}
#endif

#endif


