/*=====================================================
Copyright (c) 2003 by CHIPNUTS Incorporated. All Rights Reserved.
FileName: cmd.h
Author:              Date:
Description:     // 功能说明
Version:         // 版本信息
History:         // 历史修改记录
<author>  <time>    <version>        <desc>
      ABC     49/10/01     1.0        build this moudle  
=====================================================*/

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
