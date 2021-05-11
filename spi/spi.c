/**
  ******************************************************************************
  * @file  spi.c
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

#include <comdef.h>
#include <spi.h>

#define SPI_CONTROLLER_NUM	1
struct spi_operation *operations[SPI_CONTROLLER_NUM];

int spi_register(unsigned int bus, struct spi_operation *operation)
{
	if(bus> SPI_CONTROLLER_NUM-1)
		return -1;

	operations[bus] = operation;

	return 0;
}

int spi_unregister(unsigned int bus)
{
	if(bus> SPI_CONTROLLER_NUM-1)
		return -1;

	operations[bus] = 0;

	return 0;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode, unsigned int bus_width)
{
	if(bus> SPI_CONTROLLER_NUM-1)
		return NULL;

	if(operations[bus]->setup_slave)
	{
		return operations[bus]->setup_slave(bus,cs,max_hz,mode,bus_width);
	}
	return NULL;
}
int  spi_xfer(struct spi_slave *slave, unsigned int bitlen, void *dout,
		void *din, unsigned long flags,int bit_mode)
{
	unsigned int bus = slave->bus;
	int ret = -1;

	if(bus> SPI_CONTROLLER_NUM-1)
		return -1;

	if(operations[bus]->spi_xfer)
		ret = operations[bus]->spi_xfer(slave, bitlen, dout, din, flags, bit_mode);

	return ret;
}

