 /*      
 *----------------------------------------------------------------------------
 *      Name:    uart.h
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *      Author: MATI I TOMEK AGH University of Science and Technology
 *---------------------------------------------------------------------------*/
 
 #include "MKL46Z4.h"
 #include <stdbool.h>

 void uart_init(void);
 
 bool uart_read(char *buffer, uint16_t size, uint16_t *rd_len);
 
 void uart_write_string(char *string);

