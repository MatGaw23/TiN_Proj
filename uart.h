/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    uart.h
 *----------------------------------------------------------------------------

 #ifndef UART_H
 #define UART_H	
 
 #include "MKL46Z4.h"
 #include <stdbool.h>

 /* Initializes uart interface */ 
 void uart_init(void);
 
 /* 
  *@brief Reads data from UART buffer 
  *@param [out]buffer - output buffer
  *@param [in] size - size of data to read
  *@param [out]rd_len - bytes read from UART buffer
  *@return true if operation successfully executed, otherwise false
  */ 
 bool uart_read(char *buffer, uint16_t size, uint16_t *rd_len);
 
 /* 
  *@brief Writes string from UART buffer 
  *@param [in]string - string to write via UART interface
 */ 
 void uart_write_string(char *string);

 #endif
