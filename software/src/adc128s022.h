#ifndef _ADC128S022_H_
#define _ADC128S022_H_

#include <alt_types.h>
#include <io.h>


#define ADC0 (0<<3)
#define ADC1 (1<<3)
#define ADC2 (2<<3)
#define ADC3 (3<<3)
#define ADC4 (4<<3)
#define ADC5 (5<<3)
#define ADC6 (6<<3)
#define ADC7 (7<<3)

alt_u32 adc_base_addr = 0;
alt_u32 adc_spi_data = 0;


void adc_init(alt_u32 base)
{
	adc_base_addr = base;
	IOWR(adc_base_addr, 5, 0x01); // set slave select
}

/*the selected channel takes effect after the NEXT converion*/
void adc_set_channel(alt_u8 channel)
{
	adc_spi_data = channel;
}

alt_u32 adc_get_value()
{
		IOWR(adc_base_addr, 1, adc_spi_data); // TX data
		while ( ! ( IORD(adc_base_addr, 2) & 0x80)); // busy wait until transaction finished
		return IORD(adc_base_addr, 0); // RX data
}

#endif

