/*=====================================================
Copyright (c) 2020 by StarFiveTech  Incorporated. All Rights Reserved.
FileName: xmodem.c
Author:              Date:
Description:     
Version:        
History:        
<author>  <time>    <version>        <desc>
      ABC     49/10/01     1.0        build this moudle  
=====================================================*/

#include "xmodem.h"
#include "uart.h"
#include "util.h"

//Define Xmodem control char
#define XMODEM_NUL 0x00 
#define XMODEM_SOH 0x01 
#define XMODEM_STX 0x02 
#define XMODEM_EOT 0x04 
#define XMODEM_ACK 0x06 
#define XMODEM_NAK 0x15 
#define XMODEM_CAN 0x18 
#define XMODEM_EOF 0x1A 

/* receive 128 or 1k byte data */
#define	DATA_BUFFER_SIZE_128	128	
#define	DATA_BUFFER_SIZE_1k	1024	


/* timeout value and retry times */
#define TIMEOUT_VAL			0x3000
#define	RETRY_TIMES			0x400

#define		XMODEM_BUFFER				0x18016000


//: 0719 del
//:UINT8 rbuf[DATA_BUFFER_SIZE_128+1+2+2+1]; 


/* uart send out a char */
void uart_putchar(char c) 
{ 
	_putc(c);
} 

/* uart get in a char */
int uart_getchar(void) 
{ 
	int c;
	c=serial_getc();

    return (char)c;

} 


/* uart wait for a char */
char uart_waitchar(void) 
{ 
	int c; 
	do{
		c = uart_getchar();
	}while(c==-1);
	return (char)c; 
}


/* calculate the CRC value */
int calcrc(char *ptr, int count) 
{ 
    int crc = 0; 
    char i; 
     
    while (--count >= 0) 
    { 
        crc = crc ^ (int) *ptr++ << 8; 
        i = 8; 
        do 
        { 
        if (crc & 0x8000) 
            crc = crc << 1 ^ 0x1021; 
        else 
            crc = crc << 1; 
        } while(--i); 
    } 
    return (crc); 
} 


/* receive file via Xmodem protocol, by 128 or 1k */
UINT32 xmodem_recv_file(UINT8 *start_addr, UINT32 len)
{
	UINT32 timeout;
	UINT32 timeout_max;

	UINT32 i;
	char mych;
	UINT8 *s_ptr;
	UINT8 *t_ptr;
    int ret = 0;

	/* initialize var */
	i = 0;
	timeout = 0;
	timeout_max = TIMEOUT_VAL;	// max timeout value
	
	//s_ptr = &rbuf[0];			
	s_ptr = (UINT8 *)(XMODEM_BUFFER);		//: 20060719, size is (DATA_BUFFER_SIZE_128+1+2+2+1)
	t_ptr = start_addr;			// init the start point of memory
	

	/* clear buffer */
	for(i=0;i<(DATA_BUFFER_SIZE_128+1+2+2+1); i++)
	{
		*(s_ptr + i) = 0xff;
	}
	  
	/* wait for SOH */
	while(1)
	{
		mych = serial_nowait_getc();		
		switch(mych)
		{
			case XMODEM_SOH:
                *(UINT8 *)(s_ptr+0)= mych;		// store SOH
				ret = xmodem_recv_128(s_ptr+1, len);
				if(ret == 1)
				{
					sys_memcpy(t_ptr, (s_ptr+1+2), DATA_BUFFER_SIZE_128);	// copy data to memory
					t_ptr += DATA_BUFFER_SIZE_128;										// update the memory point
				}
				break;
			case XMODEM_EOT:
				//	case XMODEM_EOF:	//: the remained filled as EOF(0x1A)
				*(UINT8 *)(s_ptr+DATA_BUFFER_SIZE_128+1+2+2)= mych;		// store EOT/EOF
				uart_putchar(XMODEM_ACK);
				return TRUE;
				break;
			case XMODEM_CAN:
				*(UINT8 *)(s_ptr+DATA_BUFFER_SIZE_128+1+2+2)= mych;		// store CAN
				uart_putchar(XMODEM_CAN);
				return FALSE;		
				break;
			default:
				timeout++;
				if(timeout >= timeout_max)
				{
					timeout=0;
					uart_putchar('C');
				} 			
				break;
		}
 	}	
 	return TRUE;	 
}


UINT32 xmodem_recv_128(UINT8 *start_addr, UINT32 len)
{
	UINT8 *ptr;
	UINT8 fr1, fr2;
	UINT16 crc1, crc2;
	UINT32 i;
	char mych;
	
	ptr = start_addr;

 	 for(i=0;i<(2+DATA_BUFFER_SIZE_128+2);i++)                // receive 128 byte data
	{ 
		mych=uart_waitchar();
		*(UINT8 *)(ptr+i)= mych;    
	}       		   	
	fr1 = *(UINT8 *)(ptr+0);
	fr2 = *(UINT8 *)(ptr+1);
	
	/* check frame */
	if( (fr1+fr2) != 0xff)
	{
		uart_putchar(XMODEM_NAK);    
		return FALSE;
	}
	/* check crc */
	fr1 = *(UINT8 *)(ptr+DATA_BUFFER_SIZE_128+2+0);
	fr2 = *(UINT8 *)(ptr+DATA_BUFFER_SIZE_128+2+1);
	crc1 = (fr1<<8)|fr2;
	crc2 = calcrc((char *)(ptr+2),128);
	if(crc1 != crc2)
	{
		uart_putchar(XMODEM_NAK);    
		return FALSE;
	}
	uart_putchar(XMODEM_ACK); 
	return TRUE;
}

 
