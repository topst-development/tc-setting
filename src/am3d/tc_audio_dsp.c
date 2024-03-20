/****************************************************************************************
 *   FileName    : tc_audio_dsp.c
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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <unistd.h> 
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <poll.h>

#include "tc_audio_dsp.h"

#define DEV_FILE    MBOX_AUDIO_DEV_FILE

int set_effect_param(int fd, int effect_id, int param_id, int channelmask, int value)
{
	int set_msg[MBOX_AUDIO_CMD_SIZE] = {0,};
    int ret;

	set_msg[0] = AUDIO_MBOX_EFFECT_SET_MESSAGE_SIZE;
	set_msg[1] = effect_id;
	set_msg[2] = param_id;
	set_msg[3] = channelmask;
	set_msg[4] = EFFECT_FOR_MEDIA;
	set_msg[5] = value;

    ret = ioctl(fd, IOCTL_MBOX_AUDIO_EFFECT_SET_CONTROL, set_msg);

    return ret;
}

int get_effect_param(int fd, int effect_id, int param_id, int channelmask, int *value)
{
	int get_msg[MBOX_AUDIO_CMD_SIZE] = {0,};
    int ret;

	get_msg[0] = AUDIO_MBOX_EFFECT_SET_MESSAGE_SIZE;
	get_msg[1] = effect_id;
	get_msg[2] = param_id;
	get_msg[3] = channelmask;
	get_msg[4] = EFFECT_FOR_MEDIA;
	get_msg[5] = 0;

    ret = ioctl(fd, IOCTL_MBOX_AUDIO_EFFECT_GET_CONTROL, get_msg);

    *value = get_msg[5];

    return ret;
}

int set_fader_param(int fd, int balance, int fade)
{
	int set_msg[MBOX_AUDIO_CMD_SIZE] = {0,};
    int ret;

	set_msg[0] = AUDIO_MBOX_PCM_FADE_SET_MESSAGE_SIZE;
	set_msg[1] = PCM_FADER;
	set_msg[2] = ((balance << 16) & 0xFFFF0000) | (fade & 0x0000FFFF);

    ret = ioctl(fd, IOCTL_MBOX_AUDIO_PCM_SET_CONTROL, set_msg);

    return ret;
}

int get_fader_param(int fd, int *balance, int *fade)
{
	int get_msg[MBOX_AUDIO_CMD_SIZE] = {0,};
    int ret;

	get_msg[0] = AUDIO_MBOX_PCM_FADE_GET_MESSAGE_SIZE;
	get_msg[1] = PCM_FADER;
	get_msg[2] = 0;

    ret = ioctl(fd, IOCTL_MBOX_AUDIO_PCM_GET_CONTROL, get_msg);

    *balance = ((get_msg[2] >> 16) & 0x0000FFFF);
	*fade = (get_msg[2] & 0x0000FFFF);

    return ret;
}

int open_audio_dsp(void)
{
    int fd = open(DEV_FILE, O_RDWR|O_NONBLOCK);

    if (fd < 0) {
        printf("%s open failed\n", DEV_FILE);
    }
    else {
        printf("%s open\n", DEV_FILE);
    }

    return fd;
}    

void close_audio_dsp(int fd)
{
	close(fd);
}    

#ifdef __cplusplus
}
#endif

