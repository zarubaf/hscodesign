#include <alt_types.h>
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include <io.h>
#include <system.h>
#include <malloc.h>
#include <ctype.h>
#include <stdio.h>
#include "display.h"
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_regs.h"

#define CTRL_REG              0x00
#define CTRL_REG_KITT            0

#define STATUS_REG            0x01
#define STATUS_REG_IRQ           0

#define READ_BLOCK 17
void demo()
{

}

int main()
{ 
	  alt_putstr("Hello from Nios II!\n");

	  void *fb = malloc(480*800*4*2);

	  display_init((alt_u32)fb, (alt_u32)(fb+480*800*4));

	  select_framebuffer(0);
	  show_framebuffer(0);

	  set_color(0xff,0xff,0xff);
	  clear();

	  set_color(0,0,0);
	  set_pixel(0,0);
	  draw_line(0,0,799,479);

	  set_color(0,0xff,0);
	  draw_line(0,240,799,240);

	  set_color(0,0xff,0xff);
	  draw_line(0,479,799,0);

	  alt_putstr("press any key to continue\n");
	  alt_getchar();

	  select_framebuffer(1); // draw to the second framebuffer
	  set_color(0xff,0,0);
	  clear();
	  set_color(0,0,0xff);
	  fill_rect(100,100,100,100);

	  show_framebuffer(1); // show second framebuffer

	  alt_putstr("press any key to continue\n");
	  alt_getchar();
	/* Sdcard device reference */
	alt_up_sd_card_dev * device_reference = NULL;
	/* Audio Video config device reference */
	alt_up_av_config_dev *av_conf_dev;
	/* Audio device reference (wm8731) */
	alt_up_audio_dev *audio_dev;

	printf("Hello from Nios2\n");
	unsigned int block = 10;
	unsigned int databyte = 0;
	unsigned int data=0;
	unsigned int result=0;

	device_reference = alt_up_sd_card_open_dev("/dev/sd_card_interface");
	av_conf_dev = alt_up_av_config_open_dev("/dev/audio_config");

	int *command_argument_register = ((int *)(0x022C+device_reference->base));
	short int *command_register = ((short int *)(0x0230+device_reference->base));
	short int *aux_status_register = ((short int *)(0x0234+device_reference->base));
	short int *data_buffer = ((short int *)(0+device_reference->base));
	short int status;

	printf("init done\n");
	if(alt_up_av_config_write_audio_cfg_register(av_conf_dev, 0x8, 0x20) == 0)
	{
		printf("Set audio sample config success\n");
	}else{
		printf("Set audio sample config error\n");
	}
	if(alt_up_av_config_write_audio_cfg_register(av_conf_dev, 0x2, 0x55) == 0)
	{
		printf("Set audio left headphone volume success\n");
	}else{
		printf("Set audio left headphone volume error\n");
	}
	if(alt_up_av_config_write_audio_cfg_register(av_conf_dev, 0x3, 0x55) == 0)
	{
		printf("Set audio right headphone volume success\n");
	}else{
		printf("Set audio right headphone error\n");
	}
	if(alt_up_av_config_write_audio_cfg_register(av_conf_dev,  0x04, 0x10) == 0)
	{
		printf("Set DAC select config success\n");
	}else{
		printf("Set DAC select interface config error\n");
	}
	if(alt_up_av_config_write_audio_cfg_register(av_conf_dev, 0x9, 0x1) == 0)
	{
		printf("Set active control interface config success\n");
	}else{
		printf("Set active control interface config error\n");
	}	
	/* wait until sd-card ready */
	do {
		status = (short int) IORD_16DIRECT(aux_status_register, 0);
	} while (!alt_up_sd_card_is_Present());

	/* open the audio interface */
	audio_dev = alt_up_audio_open_dev ("/dev/audio_chip");
	/* reset the audio core in order to clear the fifo */
	alt_up_audio_reset_audio_core(audio_dev);	
	
	/* check the existence of the audio device */
	if ( audio_dev == NULL)
	{
		alt_printf ("Error: could not open audio device \n");
	}else{
		alt_printf ("Opened audio device \n");
	}

	/* read audio data, first read from the sd-card */		
	IOWR_32DIRECT(command_argument_register, 0, (block) * 512);
	IOWR_16DIRECT(command_register, 0, READ_BLOCK);

	/* wait until the write commands are executed */
	do {
		status = (short int) IORD_16DIRECT(aux_status_register, 0);
	} while ((status & 0x04)!=0);

	alt_32 pos_x=400, pos_y=240;
	alt_32 size=50;
	alt_32 dx=8, dy = 8;

	alt_32 fb_f = 0;

	volatile alt_u32 c;

	while (1)
	{
		data = (unsigned int) IORD_16DIRECT(data_buffer,databyte);
	
		do {
			if((databyte / 2) % 2 == 0){
				result = alt_up_audio_write_fifo(audio_dev,&data,1,ALT_UP_AUDIO_LEFT);
			}else {
				alt_up_audio_write_fifo(audio_dev,&data,1,ALT_UP_AUDIO_RIGHT);
			} 
		} while (result == 0);

//	printf("mocht wos\n");
		if(block == 90000)
		{
			block=0;
		}else{
			if(databyte < 510)
			{
				databyte += 2;
			}else{
				IOWR_32DIRECT(command_argument_register, 0, (block) * 512);
				IOWR_16DIRECT(command_register, 0, READ_BLOCK);

				do {
					status = (short int) IORD_16DIRECT(aux_status_register, 0);
				} while ((status & 0x04)!=0);

				databyte=0;
				block++;
			}
		}		

/*		for (c=0; c<10000; c++);

		show_framebuffer(0x01 & (~fb_f));
		select_framebuffer(fb_f);

		set_color(0xff,0xff,0xff);
		clear();


	    pos_x += dx;
	    pos_y += dy;

	    if(pos_y < size/2 || pos_y > 480 - size/2)
	      dy *= -1;


	    if(pos_x < size/2 || pos_x > 800 - size/2)
	      dx *= -1;


		set_color(0xff, 0, 0xff);
		fill_rect(pos_x-size/2, pos_y-size/2, size, size);

		fb_f = 0x01 & (~fb_f);*/
	}


  return 0;
}



