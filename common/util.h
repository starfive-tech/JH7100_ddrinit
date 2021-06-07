/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  util.h
  * @author  StarFive Technology
  * @version  V1.0
  * @date  07/24/2020
  * @brief
  ******************************************************************************
  * @copy
  *
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STARFIVE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  *  COPYRIGHT 2020 Shanghai StarFive Technology Co., Ltd.
  */


#ifndef UTIL_H_
#define UTIL_H_

extern void *sys_memcpy(void *p_des,const void * p_src,unsigned long size);
extern void sys_memcpy_32(void *p_des,const void * p_src,unsigned long size);
extern void sys_memset(void *p_des,unsigned char c,unsigned long size);
extern void sys_memset32(void *p_des,int c,unsigned long size);

extern  int sys_memcmp(const void * cs,const void * ct,unsigned int count);

extern void * _memcpy(void * dest,const void *src,unsigned int count);

#endif
