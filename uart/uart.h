/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  uart.h
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


#ifndef _UART_H
#define _UART_H

#define UART_REG_ADDR_INTERVAL 4
#define REG_THR 0x00  /* Transmitter holding reg. */
#define REG_RDR 0x00  /* Receiver data reg.       */
#define REG_BRDL 0x00 /* Baud rate divisor (LSB)  */
#define REG_BRDH 0x01 /* Baud rate divisor (MSB)  */
#define REG_IER 0x01  /* Interrupt enable reg.    */
#define REG_IIR 0x02  /* Interrupt ID reg.        */
#define REG_FCR 0x02  /* FIFO control reg.        */
#define REG_LCR 0x03  /* Line control reg.        */
#define REG_MDC 0x04  /* Modem control reg.       */
#define REG_LSR 0x05  /* Line status reg.         */
#define REG_MSR 0x06  /* Modem status reg.        */
#define REG_DLF 0xC0  /* Divisor Latch Fraction   */



#define UART_USR	31
#define UART_USR_BUSY		  (1 << 0) /* UART is busy (1) */
#define UART_USR_Tx_FIFO_NFUL (1 << 1) /* Tx FIFO is not full (1) */
#define UART_USR_Tx_FIFO_NEMP (1 << 2) /* Tx FIFO is empty (1) */
#define UART_USR_Rx_FIFO_NHFL (1 << 3) /* Rx FIFO is not empty (1) */
#define UART_USR_Rx_FIFO_NFUL (1 << 4) /* Rx FIFO is full (1) */


/* equates for interrupt enable register */

#define IER_RXRDY 0x01 /* receiver data ready */
#define IER_TBE 0x02   /* transmit bit enable */
#define IER_LSR 0x04   /* line status interrupts */
#define IER_MSI 0x08   /* modem status interrupts */


/* constants for line control register */

#define LCR_CS5 0x00   /* 5 bits data size */
#define LCR_CS6 0x01   /* 6 bits data size */
#define LCR_CS7 0x02   /* 7 bits data size */
#define LCR_CS8 0x03   /* 8 bits data size */
#define LCR_2_STB 0x04 /* 2 stop bits */
#define LCR_1_STB 0x00 /* 1 stop bit */
#define LCR_PEN 0x08   /* parity enable */
#define LCR_PDIS 0x00  /* parity disable */
#define LCR_EPS 0x10   /* even parity select */
#define LCR_SP 0x20    /* stick parity select */
#define LCR_SBRK 0x40  /* break control bit */
#define LCR_DLAB 0x80  /* divisor latch access enable */

/* constants for line status register */

#define LSR_RXRDY 0x01 /* receiver data available */
#define LSR_OE 0x02    /* overrun error */
#define LSR_PE 0x04    /* parity error */
#define LSR_FE 0x08    /* framing error */
#define LSR_BI 0x10    /* break interrupt */
#define LSR_EOB_MASK 0x1E /* Error or Break mask */
#define LSR_THRE 0x20  /* transmit holding register empty */
#define LSR_TEMT 0x40  /* transmitter empty */

/* equates for FIFO control register */

#define FCR_FIFO 0x01    /* enable XMIT and RCVR FIFO */
#define FCR_RCVRCLR 0x02 /* clear RCVR FIFO */
#define FCR_XMITCLR 0x04 /* clear XMIT FIFO */

/*
 * Per PC16550D (Literature Number: SNLS378B):
 *
 * RXRDY, Mode 0: When in the 16450 Mode (FCR0 = 0) or in
 * the FIFO Mode (FCR0 = 1, FCR3 = 0) and there is at least 1
 * character in the RCVR FIFO or RCVR holding register, the
 * RXRDY pin (29) will be low active. Once it is activated the
 * RXRDY pin will go inactive when there are no more charac-
 * ters in the FIFO or holding register.
 *
 * RXRDY, Mode 1: In the FIFO Mode (FCR0 = 1) when the
 * FCR3 = 1 and the trigger level or the timeout has been
 * reached, the RXRDY pin will go low active. Once it is acti-
 * vated it will go inactive when there are no more characters
 * in the FIFO or holding register.
 *
 * TXRDY, Mode 0: In the 16450 Mode (FCR0 = 0) or in the
 * FIFO Mode (FCR0 = 1, FCR3 = 0) and there are no charac-
 * ters in the XMIT FIFO or XMIT holding register, the TXRDY
 * pin (24) will be low active. Once it is activated the TXRDY
 * pin will go inactive after the first character is loaded into the
 * XMIT FIFO or holding register.
 *
 * TXRDY, Mode 1: In the FIFO Mode (FCR0 = 1) when
 * FCR3 = 1 and there are no characters in the XMIT FIFO, the
 * TXRDY pin will go low active. This pin will become inactive
 * when the XMIT FIFO is completely full.
 */
#define FCR_MODE0 0x00 /* set receiver in mode 0 */
#define FCR_MODE1 0x08 /* set receiver in mode 1 */

/* RCVR FIFO interrupt levels: trigger interrupt with this bytes in FIFO */
#define FCR_FIFO_1 0x00  /* 1 byte in RCVR FIFO */
#define FCR_FIFO_4 0x40  /* 4 bytes in RCVR FIFO */
#define FCR_FIFO_8 0x80  /* 8 bytes in RCVR FIFO */
#define FCR_FIFO_14 0xC0 /* 14 bytes in RCVR FIFO */

/*
 * UART NS16750 supports 64 bytes FIFO, which can be enabled
 * via the FCR register
 */
#define FCR_FIFO_64 0x20 /* Enable 64 bytes FIFO */

/* constants for line control register */

#define LCR_CS5 0x00   /* 5 bits data size */
#define LCR_CS6 0x01   /* 6 bits data size */
#define LCR_CS7 0x02   /* 7 bits data size */
#define LCR_CS8 0x03   /* 8 bits data size */
#define LCR_2_STB 0x04 /* 2 stop bits */
#define LCR_1_STB 0x00 /* 1 stop bit */
#define LCR_PEN 0x08   /* parity enable */
#define LCR_PDIS 0x00  /* parity disable */
#define LCR_EPS 0x10   /* even parity select */
#define LCR_SP 0x20    /* stick parity select */
#define LCR_SBRK 0x40  /* break control bit */
#define LCR_DLAB 0x80  /* divisor latch access enable */

/* constants for the modem control register */

#define MCR_DTR 0x01  /* dtr output */
#define MCR_RTS 0x02  /* rts output */
#define MCR_OUT1 0x04 /* output #1 */
#define MCR_OUT2 0x08 /* output #2 */
#define MCR_LOOP 0x10 /* loop back */
#define MCR_AFCE 0x20 /* auto flow control enable */

/* constants for line status register */

#define LSR_RXRDY 0x01 /* receiver data available */
#define LSR_OE 0x02    /* overrun error */
#define LSR_PE 0x04    /* parity error */
#define LSR_FE 0x08    /* framing error */
#define LSR_BI 0x10    /* break interrupt */
#define LSR_EOB_MASK 0x1E /* Error or Break mask */
#define LSR_THRE 0x20  /* transmit holding register empty */
#define LSR_TEMT 0x40  /* transmitter empty */

/* constants for modem status register */

#define MSR_DCTS 0x01 /* cts change */
#define MSR_DDSR 0x02 /* dsr change */
#define MSR_DRI 0x04  /* ring change */
#define MSR_DDCD 0x08 /* data carrier change */
#define MSR_CTS 0x10  /* complement of cts */
#define MSR_DSR 0x20  /* complement of dsr */
#define MSR_RI 0x40   /* complement of ring signal */
#define MSR_DCD 0x80  /* complement of dcd */

extern void uart_init();
extern int  _putc(char c);

extern void rlSendString(char *s);
extern int serial_getc();
extern int CtrlBreak( void );
extern int serial_nowait_getc();
extern void print_ubyte_hex(unsigned char bval);
extern void printk(const char* s, ...);
extern int _inbyte(unsigned short timeout);
extern void _outbyte(int c);

#endif // _UART_H
