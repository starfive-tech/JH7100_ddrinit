/*=====================================================
Copyright (c) 2020 by StarFiveTech Incorporated. All Rights Reserved.
FileName: xmodem.h
Author:              Date:
Description:     
Version:         
History:         

=====================================================*/
#ifndef		_XMODEM_H_
#define		_XMODEM_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "comdef.h"

/* ?returns the number of bytes transferred */
UINT32 xmodem_recv_file(UINT8 *start_addr, UINT32 len);
UINT32 xmodem_recv_128(UINT8 *start_addr, UINT32 len);


#ifdef __cplusplus
}
#endif




#endif	//_XMODEM_H_