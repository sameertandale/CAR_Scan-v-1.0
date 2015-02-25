/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "string.h"

#define LED_PIN	PIO_PB17_IDX
#define BLINK_INTERVAL	500

#define USART_SERIAL_ISR_HANDLER     USART0_Handler
#define		TOTAL_PIDS 15
#define     TOTAL_HEADERS 8
#define  PID_LENGTH 5
#define HEADER_LENGTH 8

uint8_t enter = '\r';
uint8_t delim_flag=0;
uint32_t received_byte_t=0;

//------------------------------------FUNCTION TO SEND COMMAND--------------------------------------------------------------
void Send_cmd(const char* command)
{
	
	usart_write_line(USART1,"SENDING PID :");
	usart_write_line(USART1,command);
	usart_write_line(USART1,"-- ");
	
	usart_write_line(USART0,command);
	usart_putchar(USART0,enter);
	delay_ms(50);
	
}

//------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------ACTIVATE LISTENING MODE---------------------------------------------------------------
void listening_mode()
{
	usart_write_line(USART1,"PRESS L TO ACTIVATE LISTENING MODE");
	usart_read(USART1,&received_byte_t);
	
	
	if(received_byte_t=='l' || received_byte_t =='L')
		{
		usart_write_line(USART0,"ATH1");
		delay_ms(500);
		usart_write_line(USART0,"ATS1");
		delay_ms(500);
		usart_write_line(USART0,"ATAL");
		delay_ms(500);
		usart_write_line(USART0,"ATMA");
		usart_write_line(USART1,"\n ------------LISTENING MODE ACTIVATED-------------");
	   }
}

//--------------------------------------------------------------------------------------------------------------------------------------



int main (void)
{
	while(1)
	{
		
	
	
	sysclk_init();
	board_init();
	ioport_init();

	uint32_t received_byte;
	uint8_t str[50];
	int i,j;
   	memset(str,'\0',50);

//---------------------------------------------initialize pids-------------------------------------------------------------	
	
	const char PIDS[TOTAL_PIDS][PID_LENGTH]={
		"0100",
		"0101",
	    "0102",
	    "0103",
	    "0104",
	    "0105",
	    "0106",
	    "0107",
	    "0108",
	    "0109",
	    "010a",
	    "010b",
	    "010c",
	    "010d",
	    "010e"
	};
	
//---------------------DECLARE HEADERS-----------------------------------------------	
	const char Headers[TOTAL_HEADERS][HEADER_LENGTH]={
		"ATSH7E0",
		"ATSH7E1",
		"ATSH7E2",
		"ATSH7E3",
		"ATSH7E4",
		"ATSH7E5",
		"ATSH7E6",
		"ATSH7E7",
		
	};
	
	/* Set output direction on the given LED IOPORTs */
	ioport_set_pin_dir(LED_PIN, IOPORT_DIR_OUTPUT);


//------------------------------------USART COMMUNCATION SETTINGS--------------------------------------------------------//	
	
	
	const sam_usart_opt_t usart_console_settings = {
		9600,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL
	};
//---------------------INITIALIZE USART0-STN-------------------------------------------------------------//	
	sysclk_enable_peripheral_clock(ID_USART0);
	usart_init_rs232(USART0, &usart_console_settings,sysclk_get_main_hz());
	usart_enable_tx(USART0);
	usart_enable_rx(USART0);
	
//-------------------------INTIALIZE USART0 INTERRUPT-STN------------------------------------------------//	
	usart_enable_interrupt(USART0, US_IER_RXRDY);
	NVIC_EnableIRQ(USART0_IRQn);
	

//--------------------------INITIALIZE USART1--TERMINAL------------------------------------------------------------------	
	sysclk_enable_peripheral_clock(ID_USART1);
	usart_init_rs232(USART1, &usart_console_settings,sysclk_get_main_hz());
	usart_enable_tx(USART1);
	usart_enable_rx(USART1);
	
	received_byte = sysclk_get_cpu_hz();
	
	//sprintf(str,"frequency: %u",received_byte);

//-------------------------------BASIC USART & STN TEST----------------------------------------------------------
	usart_write_line(USART1,"Welcome To CARiQ!!!\n");
	delay_ms(BLINK_INTERVAL);
	
	usart_write_line(USART0,"ATRV");
	usart_putchar(USART0,enter);
	delay_ms(500);
    
	usart_write_line(USART0,"010c");
    usart_putchar(USART0,enter);
    delay_ms(1000);
    	
	for(i=0;i<4;i++)
		{
		ioport_set_pin_level(LED_PIN,IOPORT_PIN_LEVEL_LOW);
		delay_ms(BLINK_INTERVAL);
		ioport_set_pin_level(LED_PIN,IOPORT_PIN_LEVEL_HIGH);
		delay_ms(BLINK_INTERVAL);
			
		}
	
	usart_write_line(USART1,"\n done with basic testing!!! press any key to proceed");	
	received_byte_t=usart_read(USART1,&received_byte_t);
	while(!received_byte_t)
	{
		
	}
	
	
	
//----------------------------------------------fire PIDS USING DEFAULT HEADER-------------------------------------------------

for(i=0;i<TOTAL_PIDS;i++)
{
	if(delim_flag=1)
	{
		Send_cmd(PIDS[i]);
	}
}

	
//---------------------------------------------------SET HEADER AND FIRE PIDS-------------------------------------------------------------

    for( j=0;j< TOTAL_HEADERS;j++)
		{
		
		usart_write_line(USART1,"HEADER SET  : ");
		usart_write_line(USART1,Headers[j]);
		
		
		usart_write_line(USART0,Headers[j]);
		usart_putchar(USART0,'\r');
		
		delay_ms(500);
		
		for(i=0;i<TOTAL_PIDS;i++)
		{
			if(delim_flag=1)
			{
			Send_cmd(PIDS[i]);
			}
        }
	 }

//------------------------------------ACTIVATE LISTENING MODE---------------------------------------------------------------
//------------------------------------GLOW LED TO INDICATE SCANNING IS DONE-----------------------------------------------

		ioport_set_pin_level(LED_PIN,IOPORT_PIN_LEVEL_LOW);
		delay_ms(BLINK_INTERVAL);


//listening_mode();
     }
//-------------------------------END OF MAIN-----------------------------------------------------------------------------------
}


void USART_SERIAL_ISR_HANDLER(void)
{
	uint32_t dw_status = usart_get_status(USART0);
	
	if (dw_status & US_CSR_RXRDY) {
		uint32_t received_byte;
		usart_read(USART0, &received_byte);
		while(!received_byte)
		{
			
		}
		
		usart_write(USART1, received_byte);
	    usart_write_line(USART1,"ENTERED INTERRRUPT"); 
       if(received_byte=='>')
		{
		 delim_flag=1;
		 usart_write(USART1,"\r\n");	
		}	
	   else
		{
		
		delim_flag=0;
		}
	}
}


//---------------------------------------------END-----------------------------------------------------------------------------
