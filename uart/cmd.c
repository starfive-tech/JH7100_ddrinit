/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  cmd.c
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


#include "cmd.h"
#include "comdef.h"
#include "uart.h"

#define			CMD_MAXNUM			4

#define		UART_CMD_BUFFER			    0x18010000

//TAG_CMD_T 	_CmdBuffer;		//:0719 del, use memory directly
P_TAG_CMD_T	StdHdl = ( P_TAG_CMD_T ) UART_CMD_BUFFER;

int	gcurCmd	= CMD_MAXNUM;

/*
 ?: 	  	cmd help
 show: 	display memory in byte/short/long
 do:  	do a procedure, and return to uart control
 run:  	remap, and jump to 0 , and not return to uart control
 */
const CMDTBLITEM	CmdHandle[CMD_MAXNUM] = 
{
	{ "?" 	, 	0x00000000 , 	CmdHelpProc, 	0x01 },
	{ "show"  , 0x00000000 ,	ShowProc 	 , 	0x01 },
	{ "load"  , 0x00000000 ,	LoadProc 	 , 	0x01 },
	{ "do" 	, 	0x00000000 , 	Do32Proc 	 , 	0x01 },		
};

unsigned char	showMode	= 0x10;
void*		pCurShow	= ( void * )0x00 ;
unsigned long	Showlength	= 0x100;


void ShowByte( unsigned char * pbuf  , int length );
void ShowShort( unsigned short * pbuf  , int length );
void ShowLong( unsigned long * pbuf , int length );



/* C720 uart cmd line init*/
void StdInit( P_TAG_CMD_T hdl )
{
	hdl->ver			= 0x100;
	hdl->length		= 0x400;
	hdl->maxlen		= 0x3FF;
	hdl->startbuf		= ( unsigned  char * )hdl->buf + 0x80;
	hdl->curbuf		= hdl->startbuf;
	hdl->curitem		= 0x000000;
	hdl->inputflag	= 0x00000000;

}

void CmdInit( void )
{
	//:StdHdl	= &_CmdBuffer;		// 0719 del
	//PrintIntHex(( unsigned long ) StdHdl , 0x8 );
	StdInit( StdHdl );
}



void RelayProc( void )
{
	int 			item;
	unsigned int 	retflag;
	
	if( CmdString(( char * ) StdHdl->argvList[0] , "" )){
		return;
	}
	
	for( item = 0x00 ; item < gcurCmd ; item++ ){
		if( CmdString(( char * ) StdHdl->argvList[0] , ( char * ) CmdHandle[item].cmdCode )){
			CmdHandle[item].cmdScriptProc( CmdHandle[item].context , ( int ) StdHdl->curArgc , ( char ** )StdHdl->argvList );
			return;
		}
	}

	//*************************************************
	if( StdHdl->curArgc == 0x02 ){
		
		if( CmdString(( char * )( StdHdl->argvList[0x00]) , "rb" )){
			item	= A2Int(( char * ) StdHdl->argvList[0x01] );
			retflag	= (( unsigned char * )item)[0x00];
			rlSendString( "\r\n Read Byte : 0x" );
			PrintIntHex(( unsigned long ) retflag , 0x2 );
			rlSendString( "\r\n" );
			return;
		}

		if( CmdString(( char * )( StdHdl->argvList[0x00]) , "rh" )){
			item	= A2Int(( char * ) StdHdl->argvList[0x01] );
			if( item & 0x0001 ){
				rlSendString( "\r\n Error address: 0x" );
				PrintIntHex(( unsigned long ) item , 0x8 );
				rlSendString( "\r\n" );

				return;
			}
			retflag	= (( unsigned short * )item)[0x00];
			rlSendString( "\r\n Read Half : 0x" );
			PrintIntHex(( unsigned long ) retflag , 0x4 );
			rlSendString( "\r\n" );
			return;
		}
		
		if( CmdString(( char * )( StdHdl->argvList[0x00]) , "rl" )){
			item	= A2Int(( char * ) StdHdl->argvList[0x01] );
			if( item & 0x0003 ){
				rlSendString( "\r\n Error address: 0x" );
				PrintIntHex(( unsigned long ) item , 0x8 );
				rlSendString( "\r\n" );
				return;
			}
			retflag	= (( unsigned long * )item)[0x00];
			rlSendString( "Read Long : 0x" );
			PrintIntHex(( unsigned long ) retflag , 0x8 );
			rlSendString( "\r\n" );
			return;
		}
	}

	if( StdHdl->curArgc == 0x03 ){

		if( CmdString(( char * )( StdHdl->argvList[0x00]) , "wb" )){
			item		= A2Int(( char * ) StdHdl->argvList[0x01] );
			retflag	= A2Int(( char * ) StdHdl->argvList[0x02] );
			(( unsigned char * )item)[0x00]	= ( unsigned char )retflag;
			rlSendString( "\r\n Write Byte: 0x" );
			PrintIntHex(( unsigned long ) retflag , 0x2 );
			rlSendString( "\r\n" );

			return;
		}

		if( CmdString(( char * )( StdHdl->argvList[0x00]) , "wh" )){
			item		= A2Int(( char * ) StdHdl->argvList[0x01] );
			retflag	= A2Int(( char * ) StdHdl->argvList[0x02] );
			if( item & 0x0001 ){
				rlSendString( "\r\n Write Half Error address: 0x" );
				PrintIntHex(( unsigned long ) item , 0x8 );
				rlSendString( "\r\n" );
				return;
			}
			(( unsigned short * )item)[0x00]	= ( unsigned short )retflag;
			rlSendString( "\r\n Write Half: 0x" );
			PrintIntHex(( unsigned long ) retflag , 0x4 );
			rlSendString( "\r\n" );
			return;
		}

		if( CmdString(( char * )( StdHdl->argvList[0x00]) , "wl" )){
			item		= A2Int(( char * ) StdHdl->argvList[0x01] );
			retflag	= A2Int(( char * ) StdHdl->argvList[0x02] );
			if( item & 0x0003 ){
				rlSendString( "\r\n Write Long Error address: 0x" );
				PrintIntHex(( unsigned long ) item , 0x8 );
				rlSendString( "\r\n" );
				return;
			}
			(( unsigned long * )item)[0x00]	= ( unsigned long )retflag;
			rlSendString( "\r\n Write Long: 0x" );
			PrintIntHex(( unsigned long ) retflag , 0x8 );
			rlSendString( "\r\n" );
			return;
		}
	}
	rlSendString( "\r\nWarning : Error command, Type ? or help to get CMD info\r\n" );
}


int ReadString( unsigned char * buf , int length )
{
	int		curitem;
	int		retflag;
	
	curitem	= 0x00000000;
	length--;
	while( 0x01 ){
		retflag	= serial_getc( );
		if( curitem < length ){
			if( retflag >= 0x20 ){
				buf[curitem] = ( unsigned char )retflag;
				_putc(( char ) buf[curitem] );
				curitem++;
				continue;
			}
		}
		if(( retflag == '\r' ) || ( retflag == '\n' )){
			buf[curitem]	= 0x00;
			_putc( '\r' );
			_putc( '\n' );
			break;
		}
		if( retflag == '\b' ){
			if( curitem > 0x00 ){
				curitem--;
				_putc( '\b' );
				_putc( ' ' );
				_putc( '\b' );
			}
			continue;
		}
		continue;
	}
	return curitem;
}

int CmdString( char * srccmd , char * basecmd )
{
	while(( *srccmd == *basecmd ) && ( *srccmd != 0x00 )){
		srccmd++;
		basecmd++;
	}

	if( *srccmd == 0x00 ){
		return 0x01;
	}
	return 0x00;
}

int piteCmdLine( char * curRead , char **pargv , int maxArgc )
{
	int		curItem;

	curItem	= 0x00000000;

	while( curItem < maxArgc ){
		while(( *curRead ) == ' ' ){
			curRead++;
		}
		pargv[curItem]	= curRead;
		if(( *curRead ) == 0x00 ){
			break;
		}
		
		while((( *curRead ) != ' ' ) && (( *curRead ) != 0x00 )){
			curRead++;
		}
		curItem++;
		if(( *curRead ) == ' ' ){
			*curRead	= 0x00;
			curRead++;
			continue;
		}
		if(( *curRead ) == 0x00 ){
			break;
		}
	}
	return curItem;
}



int CmdHelpProc( void * Context , int argc , char ** argv )
{
	int		item;
	rlSendString( "\r\n Command list :\r\n " );
	for( item = 0x01 ; item < gcurCmd ; item++ ){
		rlSendString( CmdHandle[item].cmdCode );
		rlSendString( " \r\n" );
	}
	rlSendString( "rb/rh/rl addr\r\n" );
	rlSendString( "wb/wh/wl addr val\r\n" );
	return 0x01;
}

int CmdErrorProc( void * Context , int argc , char ** argv )
{
	int	i;
	rlSendString( " msg : error cmd ->\r\n" );
	for( i = 0x00 ; i < argc ; i++ ){
		rlSendString( "\t" );
		rlSendString( argv[i] );
		rlSendString( "\r\n" );
	}
	rlSendString( "\r\n" );
	
	return 0x01;
}


//file:length <= 0x08
int U2Hex( unsigned long value , char * buf , int length )
{
 	int   temp;
 	int   retflag;
	buf[0x00] 	= 0x00;
 	buf[length] 	= 0x00;

 	retflag  = 0x00;
 	while( length > 0x00 ){
  		temp 	= 	value & 0x0F;
  		value 	>>= 0x04;
  		length--;
  		retflag++;
  		if( temp < 0x0A ){
   			buf[length] 	= ( char )temp + '0';
   			continue;
  		}
  		buf[length] = ( char )temp + ('A' - 0x0A );
 	}
 	return retflag;
}

 
int A2Int( char * buf )
{
 	int 	retflag;
 	int 	flag;
 	
 	retflag 	= 0x00;
 	do{
  		if(( buf[0x00] == '0' ) && (( buf[0x01] == 'x' ) || ( buf[0x01] == 'X' ))){
   			buf += 0x02;
   			while( 0x01 ){
	    			if(( *buf >= '0' ) && ( *buf <= '9' )){
	     				retflag <<= 0x04;
	     				retflag += *buf - '0';
	     				buf++;
	     				continue;
	    			}
	    			if(( *buf >= 'a' ) && ( *buf <= 'f' )){
	     				retflag 	<<= 0x04;
	     				retflag 	+= 	*buf - ( 'a' - 0x0a );
	     				buf++;
	     				continue;
	    			}
	    			if(( *buf >= 'A' ) && ( *buf <= 'F' )){
	     				retflag 	<<= 0x04;
	     				retflag 	+= *buf - ( 'A' - 0x0A );
	     				buf++;
	     				continue;
	    			}
    				break;
   			}
   			break;
  		}

  		flag = 0x01;
  		if( *buf == '+' ){
   			buf++;
  		}else{
   			if( *buf == '-' ){
    				flag = -1;
    				buf++;
   			}
  		}
 
  		while( 0x01 ){
   			if(( *buf >= '0' ) && ( *buf <= '9' )){
    				retflag 	*= 	0x0A;
    				retflag 	+= 	*buf - '0';
    				buf++;
    				continue;
   			}
   			retflag 	*= flag;
   			break;
  		}
 	}while( 0x00 );
 	return retflag;
}

/* load addr	: use xmodem protocol, to load a binary file to memory */
int LoadProc( void * Context , int argc , char ** argv )
{
	unsigned long pStart;
	int			retflag;

	retflag = 0;
	if( argc == 0x02 )
	{
		pStart	= A2Int(( char * ) StdHdl->argvList[0x01] );
		if( pStart & 0x03 ){
			rlSendString( "\r\n Error : load error for 0x" );
			PrintIntHex(( unsigned long ) pStart , 0x8 );
			rlSendString( "\r\n" );
			retflag = 2;
		}
		else
		{
//			retflag = xmodem_recv_file((unsigned char *)pStart, 0);
			retflag = xmodemReceive((unsigned char *)pStart, 0);
			rlSendString( "\r\nLoad file " );
			if(retflag == TRUE)
			{
				rlSendString( " ok\r\n" );
			}
			else
			{
				rlSendString( " Error\r\n" );
			}
		}	
	}
	else
	{
		rlSendString( "\r\nCMD format: load startaddr\r\n" );
		retflag = 2;
	}
	return retflag;
}	


int Do32Proc( void * Context , int argc , char ** argv )
{
	unsigned long pStart;
	int			retflag;
	UINT32		return_pc;
		
	retflag = 0;
	if( argc == 0x02 ){
		pStart	=  A2Int(( char * ) StdHdl->argvList[0x01] );
		//rlSendString( "\r\nDo procedure at : 0x" );
		//PrintIntHex(( unsigned int ) pStart , 0x8);
		//rlSendString( "\r\n" );

		if((pStart &0x3)==0)
		{
			/* importent@20060615 */
			/* save the return PC here */
			return_pc =(UINT32 )(&sys_cmd_main_loop);
			//MA_OUTW(EXIT_MINIBOOT_PC, return_pc);
			start2run32(pStart);
			//do_go_exec ((void *)pStart, argc - 1, argv + 1);
			retflag = 0;
		}
		else
		{
			rlSendString( "\r\nError, it's invalid Address: 0x" );
			PrintIntHex(( unsigned long ) pStart , 0x8 );
			rlSendString( "\r\n" );
			retflag = 1; 
		}	
	}
	else
	{
		rlSendString( "\r\nCMD format: do startaddr\r\n" );
		retflag = 2;
	}	
	return retflag;
}	
int ShowProc( void * Context , int argc , char ** argv )
{
	unsigned long pStart;
	unsigned long pEnd;
	int			retflag;
	unsigned char	mode;

	mode	= 0x000000;
	if( argc <= 0x03 ){
		do{
			if( argc == 0x01 ){
				pStart	= ( unsigned long )pCurShow;
				pEnd	= Showlength;
				break;
			}

			if( argc == 0x02 ){
				if( CmdString( argv[1] , "byte" )){
					mode	= 0x01;
				}

				if( CmdString( argv[1] , "half" )){
					mode	= 0x02;
				}

				if( CmdString( argv[1] , "long" )){
					mode	= 0x04;
				}

				if(( mode == 0x00 ) && (( argv[0x01][0x00] < '0' ) || ( argv[0x01][0x00] > '9' ))){
					mode	= 0x10;
				}
				pStart		= ( unsigned long )pCurShow;
				pEnd		= Showlength;
				if( mode == 0x00 ){
					pStart	= A2Int( argv[1] );
				}
				break;
			}
			
			if( CmdString( argv[1] , "byte" )){
				mode	= 0x01;
			}

			if( CmdString( argv[1] , "half" )){
				mode	= 0x02;
			}

			if( CmdString( argv[1] , "long" )){
				mode	= 0x04;
			}
			if(( mode == 0x00 ) && (( argv[0x01][0x00] < '0' ) || ( argv[0x01][0x00] > '9' ))){
				mode	= 0x10;
			}
			
			pEnd		= Showlength;
			if( mode == 0x00 ){
				pStart	= A2Int( argv[1] );
				pEnd	= A2Int( argv[2] );
			}else{
				pStart	= A2Int( argv[2] );
			}
			break;
			
		}while( 0x00 );

		if( mode == 0x00 ){
			mode		= showMode;
		}
		
		pEnd	+= pStart;
		pEnd	+= 0x0f;
		pStart	&= 0xFFFFFFF0;
		pEnd	&= 0xFFFFFFF0;
		pEnd	-= pStart;

		if( mode != 0x10 ){
			pCurShow	= ( void * )( pStart + pEnd );
			Showlength	= pEnd;
			showMode	= mode;
			
		}

		switch( mode ){

			case 0x01:
				ShowByte(( void * ) pStart , pEnd );
				break;

			case 0x02:
				ShowShort(( void * ) pStart , pEnd );
				break;

			case 0x04:
				ShowLong(( void * ) pStart , pEnd );
				break;

			default:
				rlSendString( "\r\nCMD format: show [ byte | half | long ] startAddress [ length ]" );
				break;
		}
		return 1;
	}
	pStart	= A2Int( argv[2] );

	pEnd	= 0x0100;
	if( argc >= 0x04 ){
		pEnd	= A2Int( argv[3] );
	}

	retflag	= 0x00000001;
	do{
		if( CmdString( argv[1] , "byte" )){
			mode	=	0x01;
			pEnd	+= pStart;
			pEnd	+= 0x0f;
			pStart	&= 0xFFFFFFF0;
			pEnd	&= 0xFFFFFFF0;
			pEnd	-= pStart;
			ShowByte(( void * ) pStart , pEnd );
			break;
		}
		if( CmdString( argv[1] , "half" )){
			mode	= 	0x02;
			//pEnd	<<= 0x0001;
			pEnd	+= pStart;
			pEnd	+= 0x0f;
			pStart	&= 0xFFFFFFF0;
			pEnd	&= 0xFFFFFFF0;
			pEnd	-= pStart;
			ShowShort(( void * ) pStart , pEnd );
			break;
		}
		if( CmdString( argv[1] , "long" )){
			mode	= 	0x04;
			//pEnd	<<= 0x0002;
			pEnd	+= pStart;
			pEnd	+= 0x0f;
			pStart	&= 0xFFFFFFF0;
			pEnd	&= 0xFFFFFFF0;
			pEnd	-= pStart;
			ShowLong(( void * ) pStart , pEnd );
			break;
		}
		rlSendString( "\r\n show [ byte | half | long ] startAddress [ length ]" );
		retflag	= -2;

	}while( FALSE );

	if( retflag > 0x000000 ){
		pCurShow	= ( void * )( pStart + pEnd );
		Showlength	= pEnd;
		showMode	= mode;
	}
	return retflag;
}


void ShowHexChar( unsigned char * pbuf )
{
	int	item;
	rlSendString( "    " );
	for( item = 0x00 ; item < 0x10 ; item++ ){
		if(( pbuf[item] >= 0x20 ) && ( pbuf[item] < 0x80 )) {
			_putc( pbuf[item] );
			continue;
		}
		_putc( '.' );
	}
}



//******************************************
//start , length = nBytes
void ShowByte( unsigned char * pbuf  , int length )
{
	unsigned long	endPbuf;
	unsigned char *pCurBuf;
	pCurBuf		= pbuf;
	for( endPbuf = (( unsigned long ) pbuf ) + length ; ( unsigned long )pbuf < endPbuf ; pbuf++ ){
		if(((( unsigned long ) pbuf ) & 0x0000000F ) == 0x00000000 ){
			PrintIntHex(( unsigned long ) pbuf , 0x08 );
			pCurBuf	= pbuf;
		}
		_putc( ' ' );
		PrintIntHex(( unsigned long )( *pbuf ) , 0x00000002 );
		if(((( unsigned long ) pbuf ) & 0x0000000F ) == 0x0000000F ){
			ShowHexChar( pCurBuf );
			rlSendString( "\r\n" );
			if( CtrlBreak( )){
				break;
			}
		}
	}
}



//******************************************
//start , length = nBytes
void ShowShort( unsigned short * pbuf  , int length )
{
	unsigned long	endPbuf;
	unsigned char *pCurBuf;
	pCurBuf		= ( unsigned char * )pbuf;
	for( endPbuf = (( unsigned long ) pbuf ) + length ; ( unsigned long )pbuf < endPbuf ; pbuf++ ){
		if(((( unsigned long ) pbuf ) & 0x0000000F ) == 0x00000000 ){
			PrintIntHex(( unsigned long ) pbuf , 0x08 );
			pCurBuf	= ( unsigned char * )pbuf;
		}
		_putc( ' ' );
		PrintIntHex(( unsigned long )( *pbuf ) , 0x00000004 );
		if(((( unsigned long ) pbuf ) & 0x0000000E ) == 0x0000000E ){
			ShowHexChar( pCurBuf );
			rlSendString( "\r\n" );
			if( CtrlBreak( )){
				break;
			}
		}
	}
}


void ShowLong( unsigned long * pbuf , int length )
{
	unsigned long	endPbuf;
	unsigned char *pCurBuf;
	pCurBuf		= ( unsigned char * )pbuf;
	for( endPbuf = (( unsigned long ) pbuf ) + length ; ( unsigned long )pbuf < endPbuf ; pbuf++ ){
		if(((( unsigned long ) pbuf ) & 0x0000000F ) == 0x00000000 ){
			PrintIntHex(( unsigned long ) pbuf , 0x08 );
			pCurBuf	= ( unsigned char * )pbuf;
		}
		_putc( ' ' );
		PrintIntHex(( unsigned long )( *pbuf ) , 0x00000008 );
		if(((( unsigned long ) pbuf ) & 0x0000000C ) == 0x0000000C ){
			ShowHexChar( pCurBuf );
			rlSendString( "\r\n" );
			if( CtrlBreak( )){
				break;
			}
		}
	}
}


void PrintIntHex( unsigned long hexData , int length )
{
	int				curItem;
	unsigned int	temp;
	char			msgbuf[0x10];

	msgbuf[0x0A] = 0x00;

	curItem	= 0x09;
	while( curItem > 0x01 ){
		temp	= hexData & 0x0F;
		hexData	>>= 0x04;
		if( temp <= 0x09 ){
			msgbuf[curItem]	= '0' + temp;
			curItem--;
			continue;
		}
		msgbuf[curItem]	= ( 'A' - 0x0A ) + temp;
		curItem--;
	}
	rlSendString( msgbuf + ( 0x0A - length ));
}


void sys_cmd_proc( void )
{
    StdHdl = ( P_TAG_CMD_T ) UART_CMD_BUFFER;
    gcurCmd	= CMD_MAXNUM;
	CmdInit( );
	rlSendString( "\r\n" );
	rlSendString( "(C)SiFive" );
	/* command line main loop */
	sys_cmd_main_loop();
}

void sys_cmd_main_loop(void)
{
	unsigned int retflag;
	rlSendString( "\r\n# " );
	while( TRUE){
		retflag	= ReadString(( unsigned char * ) StdHdl->curbuf , 0x3FF );
		retflag	= piteCmdLine(( char * ) StdHdl->curbuf , ( char ** )StdHdl->argvList , 0x20 );
		StdHdl->curArgc	= retflag;
		RelayProc( );
		rlSendString( "\r\n# " );
	}

}
