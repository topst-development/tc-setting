/****************************************************************************************
 *   FileName    : EQMacro.h
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

#ifndef EQ_MACRO_H
#define EQ_MACRO_H

#define EQ_X_OFFSET 	(249)
#define EQ_Y_OFFSET 	(65)

#define DEF_RECT_INFO(var,x,y,w,h) \
	static const int var##Rect[4] = {x,y,w,h}\

#define DEF_BTN_IMAGES_INFO(var,i,j,k,l) \
	static const char* var##ImageNames[4] = {i,j,k,l}

#define DEF_BTN_INFO(var,x,y,w,h,i,j,k,l) \
	DEF_RECT_INFO(var,x,y,w,h); \
	DEF_BTN_IMAGES_INFO(var,i,j,k,l)

#define SET_RECT_GEOMETRY(x,y) ((x)->setGeometry(y##Rect[0],y##Rect[1],y##Rect[2],y##Rect[3]))
#define SET_BTN_IMAGES(x,y) ((x)->SetImage(y##ImageNames[0],y##ImageNames[1],y##ImageNames[2],y##ImageNames[3]))

#endif
