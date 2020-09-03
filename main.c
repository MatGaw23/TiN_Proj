/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    main.c
 *      Purpose: Microprocessors Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
 #include <string.h>
 
 #include "MKL46Z4.h"
 #include "uart.h"
 #include "lowpower.h"
 #include <stdlib.h> 
 #include <stdio.h>
 
 #define BUFFER_SIZE	500
 
 static void delay(uint16_t ticks);
 static void parse_response(char *response);
 static void log_pmstat(void);
 
 struct main_ctx
 {
	 char buffer[BUFFER_SIZE];
	 uint16_t position;
	 bool is_sleep_mode_enabled;
 };
 static struct main_ctx ctx;
 
 int main(void)
 {
    uart_init();
		lowPowerInitialize();
	 
	  while(1)
	  {
			
			uint16_t rd_len = 0;
			uart_read(ctx.buffer + ctx.position, BUFFER_SIZE, &rd_len);
			ctx.position += rd_len;
			
		
			if(ctx.position > 0)
			{
				parse_response(ctx.buffer);
			}
			
		 if(ctx.is_sleep_mode_enabled)
			enterVLPMode();
		 else
				delay(10);
		}
 }
 
static void parse_response(char *response)
{
	if(strstr(response, "Sleep 1") != NULL)
	{
		uart_write_string("\n Sleep mode enabled\n");

		ctx.is_sleep_mode_enabled = true;
		ctx.position = 0;
		memset(ctx.buffer, 0x00, BUFFER_SIZE);
		log_pmstat();
	}
	
	if(strstr(response, "Sleep 0") != NULL)
	{
		uart_write_string("\n Sleep mode disabled\n");
		ctx.is_sleep_mode_enabled = false;
		ctx.position = 0;
		memset(ctx.buffer, 0x00, BUFFER_SIZE);
		log_pmstat();
	}
}

static void delay(uint16_t ticks)
{
	for(uint8_t i = 0; i < ticks; i++)
		asm volatile ("nop");
}

static void log_pmstat(void)
{
		char string[20];
		uint8_t read_byte = SMC->PMSTAT;

		sprintf(string, "PMSTAT :%d \n", read_byte); 
		uart_write_string(string);
}