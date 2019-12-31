#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

static void rcc_init(void){

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB 
							| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);
}

static void gpio_init(void){

  GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = RX_PIN;
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void usart_send_char(char ch){

	/* Loop until the end of transmission */
	//while (USART_GetFlagStatus(COM_PORT, USART_FLAG_TC) == RESET){}
	while((COM_PORT->SR & USART_FLAG_TC) != USART_FLAG_TC){
	
	}	
	USART_SendData(COM_PORT, (uint8_t) ch);
}

uint8_t usart_recv_char(){
	/* Wait the byte is entirely received by USARTy */
    //while(USART_GetFlagStatus(COM_PORT, USART_FLAG_RXNE) == RESET){}
	while((COM_PORT->SR & USART_FLAG_RXNE) != USART_FLAG_RXNE){
	
	}
	
    /* Store the received byte in the RxBuffer1 */
    return (uint8_t)USART_ReceiveData(COM_PORT);
}

void usart_init(void){

	USART_InitTypeDef USART_InitStructure;
	rcc_init ();
	gpio_init ();
	
	/* USARTx configured as follow:
		- BaudRate = 115200 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* USART configuration */
	USART_Init(COM_PORT, &USART_InitStructure);
	USART_Cmd(COM_PORT, ENABLE);
}
/*
int usart_printf(const char *fmt, ... )
{
    uint8_t i = 0;
    uint8_t usart_tx_buf[128] = { 0 };
    va_list ap;

    va_start(ap, fmt );
    vsprintf((char*)usart_tx_buf, fmt, ap);
    va_end(ap);
	
	while(usart_tx_buf[i] && i < 128){
		usart_send_char(usart_tx_buf[i]);		 
		i++;
	}	
   	usart_send_char('\0');
	return 0;
}
*/

