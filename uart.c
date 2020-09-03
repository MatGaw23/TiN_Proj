/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    uart.c
 *----------------------------------------------------------------------------

 #include "uart.h"
 #include <stdlib.h>
 #include <stdbool.h>
 #include <string.h>
 
 #define UART_RX_PIN						1
 #define UART_TX_PIN						2
 
 #define UART_RX_BUFFER_SIZE				500

 struct uart_ctx
 { 
   char *buffer;
	 uint16_t position;
 };

 static struct uart_ctx ctx;
 
 void uart_init(uint8_t buffer)
 {
	 SIM->SCGC4|= SIM_SCGC4_UART0_MASK;
	 SIM->SCGC5|= SIM_SCGC5_PORTA_MASK;
	 PORTA->PCR[UART_RX_PIN]|= PORT_PCR_MUX(2);
	 PORTA->PCR[UART_TX_PIN]|= PORT_PCR_MUX(2);
	 
	 SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2);//lub and nie wiem kurwa jak to sie robi zeby bylo dobrze w lab2 bylo lub wiec tak dalem
	 
	 UART0->C2 &= ~(UART0_C2_RE_MASK | UART0_C2_TE_MASK);
	 UART0->C4 |= UART0_C4_OSR(31ul);
	 
	 UART0->BDH |= UART0_BDH_SBR(0x00);
	 UART0->BDL = UART0_BDL_SBR(26ul);//nie wiem na ile to ustawic
	 
	 UART0->BDH &= ~UART0_BDH_SBNS_MASK;
	 UART0->C1 &= ~(UART0_C1_M_MASK|UART0_C1_PE_MASK);//no parrity control 8 bit data
		
	 UART0->C2 |= UART0_C2_RIE_MASK | UART0_C2_RE_MASK | UART0_C2_TE_MASK; 

	 /*ARM's Nested Vector Interrupt Controller configuration*/
	 NVIC_ClearPendingIRQ(UART0_IRQn);	 
	 NVIC_EnableIRQ(UART0_IRQn);			
	 NVIC_SetPriority (UART0_IRQn, 3);	 	 
}
 
 bool uart_read(char *buffer, uint16_t size, uint16_t *rd_len)
 {
	 if (ctx.position == 0)
		 return false;
	
	 uint16_t read_size = size > UART_RX_BUFFER_SIZE ? UART_RX_BUFFER_SIZE : size;
	 read_size = ctx.position < read_size ? ctx.position : read_size;
	 
	 memcpy(buffer, ctx.buffer, read_size);
	
	 if (rd_len != NULL)
		 *rd_len = read_size;
	 
	 ctx.position = 0;
	 return true;
 }
  
 void uart_write_string(char *string)
 {
		uint16_t string_len = strlen(string);
		uint16_t iterator = 0;
	 
	  while (string_len > 0)
		{
			if ((UART0->S1 & UART_S1_TDRE_MASK) || (UART0->S1 & UART_S1_TC_MASK))
			{
				UART0->D = string[iterator];
				iterator++;
				string_len--;
			}
		}

 }
void UART0_IRQHandler (void)
{
  char c = 0;
	
  if (UART0->S1 & UART_S1_RDRF_MASK)
  {
    c = UART0->D;
	
	if((UART0->S1 & UART_S1_TDRE_MASK) || (UART0->S1 & UART_S1_TC_MASK))
	{
		UART0->D = c;
	}
  }
}