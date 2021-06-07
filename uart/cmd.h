/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  cmd.h
  * @author  StarFive Technology
  * @version  V1.0
  * @date  07/20/2020
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


#ifndef		_MB_CMD_H_
#define		_MB_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct TAG_CMD_{
	unsigned short		ver;
	unsigned short		length;
	void *				cmdAppHdl;

	//**************************************
	void *			reserved[0x10];	//dev route : init , ioctrl , read , write , rflute , wflute , getchar , tgetchar 
	
	unsigned char *	startbuf;
	unsigned long		inputflag;
	unsigned long		curitem;
	unsigned long		maxlen;

	//**************************************
	unsigned char *	curbuf;
	unsigned long		curCH;
	unsigned long		temp;
	unsigned long		curArgc;
	
	unsigned char *	argvTemp[0x06];
	unsigned char *	argvList[0x20];
	unsigned char		buf[0x100 + 0x400 ];
} *P_TAG_CMD_T;

	
typedef int (*CMDSCRIPTPROC)( void *Context , int argc , char ** argv );

typedef struct tagCMDTBLITEM{
	char *				cmdCode;
	void *				context;
	CMDSCRIPTPROC		cmdScriptProc;
	int					flag;
}CMDTBLITEM , *PCMDTBLITEM;


int CmdString( char * srccmd , char * basecmd );
void RelayProc( void );
void StdInit( P_TAG_CMD_T hdl );
int ReadString( unsigned char * buf , int length );
int piteCmdLine( char * curRead , char **pargv , int maxArgc );

int CmdHelpProc( void * Context , int argc , char ** argv );
int CmdErrorProc( void * Context , int argc , char ** argv );


int ShowProc( void * Context , int argc , char ** argv );
int LoadProc( void * Context , int argc , char ** argv );
int Do32Proc( void * Context , int argc , char ** argv );
int SetBaud32Proc( void * Context , int argc , char ** argv );

int U2Hex( unsigned long value , char * buf , int length );
int A2Int( char * buf );
void PrintIntHex( unsigned long hexData , int length );

void sys_cmd_proc( void );
void sys_cmd_main_loop(void);


#ifdef __cplusplus
}
#endif



#endif 		//_MB_CMD_H_
