/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====**
*		Copyright (c) 2009 by UVCHIP Inc. All rights reserved.
*	This source code contains confidential, trade secret material of
*	UVCHIP Inc. Any attempt or participation in deciphering, decoding,
*	reverse engineering or in any way altering the source code is strictly
*	prohibited.
*
*	Branch/Tag Name:	$Name$
*	FileName: 		$RCSfile$
*	Description:  
*			
*		This file is used to build test drive firmware for fpga drive ip modele
*
*	$Author$ 
*	$Date$
*	$Revision$
*   
**====*====*====*====*====*====*====*====*====*====*====*====*====*====*====**
* Revisions History
*
*	$Log$
**====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef UTIL_H_
#define UTIL_H_

extern void *sys_memcpy(void *p_des,const void * p_src,unsigned long size);
extern void sys_memcpy_32(void *p_des,const void * p_src,unsigned long size);
extern void sys_memset(void *p_des,unsigned char c,unsigned long size);
extern void sys_memset32(void *p_des,int c,unsigned long size);

extern  int sys_memcmp(const void * cs,const void * ct,unsigned int count);

extern void * _memcpy(void * dest,const void *src,unsigned int count);

#endif
