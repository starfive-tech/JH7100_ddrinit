#include <stdarg.h>
#include <comdef.h>
#include <uart.h>
#include <sys.h>
#include <ezGPIO_fullMux_ctrl_macro.h>
#include <clkgen_ctrl_macro.h>
#include <rstgen_ctrl_macro.h>
#include <vic_module_reset_clkgen.h>


#define UART_CLK	(100000000UL)

/* CLK 32M */
#define UART_BUADRATE_32MCLK_9600	9600
#define UART_BUADRATE_32MCLK_57600	57600
#define UART_BUADRATE_32MCLK_115200	115200
#define UART_BUADRATE_32MCLK_125000	125000
#define UART_BUADRATE_32MCLK_222222	222222	///230400
#define UART_BUADRATE_32MCLK_333333	333333	///380400
#define UART_BUADRATE_32MCLK_400000	400000	///460800
#define UART_BUADRATE_32MCLK_500000	500000 
#define UART_BUADRATE_32MCLK_666666	666666	///921600
#define UART_BUADRATE_32MCLK_1M	1000000
#define UART_BUADRATE_32MCLK_2M	2000000


/* CLK 64M */
#define UART_BUADRATE_64MCLK_38400	38400
#define UART_BUADRATE_64MCLK_57600	57600
#define UART_BUADRATE_64MCLK_115200	115200
#define UART_BUADRATE_64MCLK_230400	230400
#define UART_BUADRATE_64MCLK_380400	380400
#define UART_BUADRATE_64MCLK_444444	444444	///460800
#define UART_BUADRATE_64MCLK_500000	500000
#define UART_BUADRATE_64MCLK_571428	571428
#define UART_BUADRATE_64MCLK_666666	666666	///921600

#define UART_BUADRATE_64MCLK_800000	800000	///921600
#define UART_BUADRATE_64MCLK_1M	1000000
#define UART_BUADRATE_64MCLK_2M	2000000

////#define UART_BUADRATE_2_5M	2500000	///
////#define UART_BUADRATE_3M	3000000	///
#define UART_BUADRATE_64MCLK_4M	4000000	///shiboqi


unsigned int uart_id=0;

static const u64 uart_base[4] = {
    UART0_HS_BASE_ADDR,
    UART1_HS_BASE_ADDR,
    UART2_BASE_ADDR,
    UART3_BASE_ADDR,
        
};

static unsigned int serial_in(int offset)
{
	offset <<= 2;
	return readl((volatile void *)(uart_base[uart_id] + offset));
}

static void serial_out(int offset, int value)
{
	offset <<= 2;
	writel(value, (volatile void *)(uart_base[uart_id] + offset));
}

int __serial_tstc()
{
	return ((serial_in(REG_LSR)) & (1 << 0));
}
 int serial_tstc()
{
	return __serial_tstc();
}

void uart_init(int id)
{
	unsigned int divisor;
	unsigned char lcr_cache;
#if 0 //repeat init
	switch(id)
	{
		case 0:
			vic_uart0_reset_clk_gpio_misc_enable;
			break;
		
		case 1:
			vic_uart1_reset_clk_gpio_misc_enable;
			break;
		
		case 2:
			vic_uart2_reset_clk_gpio_misc_enable
			break;
			
		case 3:
			_ENABLE_CLOCK_clk_uart3_apb_;
			_ENABLE_CLOCK_clk_uart3_core_;

			_ASSERT_RESET_rstgen_rstn_uart3_apb_;
			_ASSERT_RESET_rstgen_rstn_uart3_core_;
			_CLEAR_RESET_rstgen_rstn_uart3_core_;
			_CLEAR_RESET_rstgen_rstn_uart3_apb_;
			SET_GPIO_14_dout_uart3_pad_sout;
			SET_GPIO_14_doen_LOW;
			SET_GPIO_13_doen_HIGH;
			SET_GPIO_uart3_pad_sin(13);
			break;
			
		default:
			return;
	}
#endif
 	uart_id = id;
	
	divisor = (UART_CLK / UART_BUADRATE_32MCLK_115200) >> 4;

	lcr_cache = serial_in(REG_LCR);
	serial_out(REG_LCR, (LCR_DLAB | lcr_cache));
	serial_out(REG_BRDL,(unsigned char)(divisor & 0xff));
	serial_out(REG_BRDH,(unsigned char)((divisor >> 8) & 0xff));
	

	/* restore the DLAB to access the baud rate divisor registers */
	serial_out(REG_LCR, lcr_cache);

	/* 8 data bits, 1 stop bit, no parity, clear DLAB */
	serial_out(REG_LCR, (LCR_CS8 | LCR_1_STB | LCR_PDIS));
	
	serial_out(REG_MDC, 0); /*disable flow control*/
	
	/*
	 * Program FIFO: enabled, mode 0 (set for compatibility with quark),
	 * generate the interrupt at 8th byte
	 * Clear TX and RX FIFO
	 */
	serial_out(REG_FCR, (FCR_FIFO | FCR_MODE1 | /*FCR_FIFO_1*/FCR_FIFO_8 | FCR_RCVRCLR | FCR_XMITCLR));
	
	serial_out(REG_IER, 0);//dis the ser interrupt
}

int _putc(char c) {
	do
	{}while((serial_in(REG_LSR) & LSR_THRE) == 0);

	serial_out(REG_THR, c);
	return 0;
}

void rlSendString(char *s)
{
	while (*s){
		_putc(*s++);
	}
}

int CtrlBreak( void )
{
	int retflag;

	do{
		retflag	= serial_getc();
		if( retflag == 0x03 ){
			break;
		}
	}while( retflag );
	return retflag;
}
int serial_getc()
{	
	/* Wait here until the the FIFO is not full */
    while (!(serial_in(REG_LSR) & (1 << 0))){};

	return serial_in(REG_RDR);
}

void serial_gets(char *pstr)
{
	char c;
	char *pstrorg;
	
	pstrorg = (char *) pstr;

	while ((c = (char)serial_getc()) != '\r')
	{
		if (c == '\b'){
			if (pstrorg < pstr){
				rlSendString("\b \b");
				pstr--;
			}
		}else{
			*pstr++ = c;
			_putc(c);
		}
	}

	*pstr = '\0';

	rlSendString("\r\n");
		
}


void _puts(const char * s) {
  while (*s != '\0'){
    _putc(*s++);
  }
}

void print_ubyte_hex(unsigned char bval)
{
	static const char digits[16] = "0123456789ABCDEF";
	char tmp[2];
	int dig=0;

	dig = ((bval&0xf0)>>4);
	tmp[0] = digits[dig];
	dig = (bval&0x0f);
	tmp[1] = digits[dig];
	_putc(tmp[0]);
	_putc(tmp[1]);
}
int serial_nowait_getc()
{
	unsigned int status;

	status = serial_in(REG_LSR);
	if (!(status & (1 << 0))) {
		status = 0;//NO_POLL_CHAR;
		goto out;
	}
	status = serial_in(REG_RDR);
out:
	return status;
}
#if 1
int vnprintf(char* out, size_t n, const char* s, va_list vl)
{
  bool format = false;
  bool longarg = false;
  size_t pos = 0;
  for( ; *s; s++)
  {
    if(format)
    {
      switch(*s)
      {
        case 'l':
          longarg = true;
          break;
        case 'p':
          longarg = true;
          if (++pos < n) out[pos-1] = '0';
          if (++pos < n) out[pos-1] = 'x';
        case 'x':
        {
          long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
          for(int i = 2*(longarg ? sizeof(long) : sizeof(int))-1; i >= 0; i--) {
            int d = (num >> (4*i)) & 0xF;
            if (++pos < n) out[pos-1] = (d < 10 ? '0'+d : 'a'+d-10);
          }
          longarg = false;
          format = false;
          break;
        }
        case 'd':
        {
          long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
          if (num < 0) {
            num = -num;
            if (++pos < n) out[pos-1] = '-';
          }
          long digits = 1;
          for (long nn = num; nn /= 10; digits++)
            ;
          for (int i = digits-1; i >= 0; i--) {
            if (pos + i + 1 < n) out[pos + i] = '0' + (num % 10);
            num /= 10;
          }
          pos += digits;
          longarg = false;
          format = false;
          break;
        }
        case 's':
        {
          const char* s2 = va_arg(vl, const char*);
          while (*s2) {
            if (++pos < n)
              out[pos-1] = *s2;
            s2++;
          }
          longarg = false;
          format = false;
          break;
        }
        case 'c':
        {
          if (++pos < n) out[pos-1] = (char)va_arg(vl,int);
          longarg = false;
          format = false;
          break;
        }
        default:
          break;
      }
    }
    else if(*s == '%')
      format = true;
    else
      if (++pos < n) out[pos-1] = *s;
  }
  if (pos < n)
    out[pos] = 0;
  else if (n)
    out[n-1] = 0;
  return pos;
}

static void vprintk(const char* s, va_list vl)
{
  char out[256]; 
  int res = vnprintf(out, sizeof(out), s, vl);
  _puts(out);
}

void printk(const char* s, ...)
{
  va_list vl;
  va_start(vl, s);

  vprintk(s, vl);

  va_end(vl);
}
#endif

int _inbyte(unsigned short timeout) // msec timeout
{
    unsigned int c;
    unsigned int delay = timeout*20;

    while (!(serial_in(REG_LSR) & LSR_RXRDY)) {
        udelay(50);
        if (timeout && (--delay == 0)) {
            return -1;
        }
    }
    c = serial_in(REG_RDR);
    return c;
}

void _outbyte(int c)
{
    while(!(serial_in(REG_LSR) & LSR_TEMT))
        ;

    serial_out(REG_THR, c);
}

