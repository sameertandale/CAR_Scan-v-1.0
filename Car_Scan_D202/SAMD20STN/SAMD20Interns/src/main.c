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
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <string.h>

void configure_usart_CONSOLE(void);
void write_DEBUG(const char *tx_data);
void write_DEBUG(const char *tx_data);
int configure_usart_STN(void);
void stn_rx_handler(uint8_t instance);
void RcvResponseFromSTN(uint8_t RcvedData);
void sendPID(const char *cmd);
void LedInit(void);
//void start_scan();
//------------------------------------------------------------------------------------------------------------------------------------

struct usart_module usart_instance_CONSOLE;
struct usart_module usart_instance_STN;
static char STNResBuff[200];
static uint8_t RcvCount = 0;
static volatile bool STNRcvCompleteFlag = false;


#define		TOTAL_PIDS 15
#define     TOTAL_HEADERS 8
#define  PID_LENGTH 5
#define HEADER_LENGTH 8


int i=0;
int j=0;


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

//--------------------------------------configure console------------------------------------------------------------------------------
void configure_usart_CONSOLE(void)
{
	
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);

	config_usart.baudrate    = 115200;

	/* Settings For New SAM Board. */
	config_usart.mux_setting = USART_RX_3_TX_2_XCK_3;//
	config_usart.pinmux_pad0 = PINMUX_UNUSED;
	config_usart.pinmux_pad1 = PINMUX_UNUSED;
	config_usart.pinmux_pad2 = PINMUX_PA30D_SERCOM1_PAD2;
	config_usart.pinmux_pad3 = PINMUX_PA31D_SERCOM1_PAD3;
	
	while (usart_init(&usart_instance_CONSOLE, SERCOM1, &config_usart) != STATUS_OK) {	}
	stdio_serial_init(&usart_instance_CONSOLE, SERCOM1, &config_usart);
	
	usart_enable(&usart_instance_CONSOLE);
	usart_enable_transceiver(&usart_instance_CONSOLE, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&usart_instance_CONSOLE, USART_TRANSCEIVER_RX);
	
	}

//-------------------------------------------------------------------------------------------------------------------------------------
void write_DEBUG(const char *tx_data)
{
	usart_write_buffer_wait(&usart_instance_CONSOLE, (uint8_t *)tx_data, strlen(tx_data));
}
//-----------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------CONFIGURE STN------------------------------------------------------------------------------
int configure_usart_STN(void)
{
	int retVal = 0;
	
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);

	config_usart.baudrate    = 9600;

	/* Settings For New SAM Board. */
	config_usart.mux_setting = USART_RX_1_TX_0_XCK_1;
	config_usart.pinmux_pad0 = PINMUX_PB08D_SERCOM4_PAD0;
	config_usart.pinmux_pad1 = PINMUX_PB09D_SERCOM4_PAD1;
	config_usart.pinmux_pad2 = PINMUX_UNUSED;
	config_usart.pinmux_pad3 = PINMUX_UNUSED;

	while (usart_init(&usart_instance_STN,SERCOM4, &config_usart) != STATUS_OK) {
	}

	usart_enable(&usart_instance_STN);
	usart_enable_transceiver(&usart_instance_STN, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&usart_instance_STN, USART_TRANSCEIVER_RX);
	
	_sercom_set_handler(_sercom_get_sercom_inst_index(SERCOM4),	stn_rx_handler);
	SERCOM4->USART.INTENSET.reg = SERCOM_USART_INTFLAG_RXC;
	
	retVal = 1;
	
	return retVal;
}

//---------------------------------------------------STN INTERRUPT VECTOR-------------------------------------------------------------

void stn_rx_handler(uint8_t instance)
{
	SercomUsart *const usart_hw = &SERCOM4->USART;
	UNUSED(instance);

	//write_DEBUG("\r\nHere 1", strlen("\n\rHere 1"));
    // printf("into the interrupt");
	if (usart_hw->INTFLAG.reg & SERCOM_USART_INTFLAG_RXC)
	{
		/* Check if a data reception error occurred */
		uint8_t rx_error = usart_hw->STATUS.reg & (SERCOM_USART_STATUS_FERR | SERCOM_USART_STATUS_BUFOVF);

		/* If error occurred clear the error flags, otherwise queue new data */
		if (rx_error)
		usart_hw->STATUS.reg = rx_error;
		else
		{
			uint8_t data = (usart_hw->DATA.reg & SERCOM_USART_DATA_MASK);
			RcvResponseFromSTN(data);
			//write_DEBUG(&data);
			//printf(&data);
		}
	}
}

//-------------------------------------------RECEIVE RESPONSE FROM STN--------------------------------------------------------------------
void RcvResponseFromSTN(uint8_t RcvedData)
{
	switch(RcvedData)
	{
		case 0x0D:/* 0x0D */
		break;
		
		case '>':/* '>' */
		//write_DEBUG(STNResBuff,RcvCount);
			STNRcvCompleteFlag = true;
		break;

		case ' ':/* ' ' */
		break;

		default:
			STNResBuff[RcvCount++] = RcvedData;
		break;
	}
}

//---------------------------------------------SEND PID-------------------------------------------------------------------------

void sendPID(const char *cmd)
{
	/*  Clear STNResString */
	memset(STNResBuff,'\0',strlen((char *)STNResBuff));
	RcvCount = 0;
	
	usart_write_buffer_wait(&usart_instance_STN, (uint8_t *)cmd, strlen(cmd));
	usart_write_wait(&usart_instance_STN, '\r');
	usart_write_wait(&usart_instance_STN, '\n');
	
	delay_ms(50) ; // This delay is playing role in improving throughput with STN communication
	
	while( STNRcvCompleteFlag != true );
	STNRcvCompleteFlag = false;
}
//--------------------------------------------------------CAR SCAN---------------------------------------------------------------------
void start_scan()
	{
		
		
}




//-------------------------------------------------------LED ININT--------------------------------------------------------------------------

void LedInit(void)
{
	struct port_config ConfigPinLed;
	
	port_get_config_defaults(&ConfigPinLed);
	ConfigPinLed.direction = PORT_PIN_DIR_OUTPUT;
	ConfigPinLed.input_pull = PORT_PIN_PULL_UP;
	
	port_pin_set_config(PIN_PB01, &ConfigPinLed);
	port_pin_set_config(PIN_PB02, &ConfigPinLed);
	port_pin_set_config(PIN_PB03, &ConfigPinLed);
	
	port_pin_set_output_level(PIN_PB01, 0);
	port_pin_set_output_level(PIN_PB02, 0);
	port_pin_set_output_level(PIN_PB03, 0);
}

//---------------------------------------------------MAIN--------------------------------------------------------------------------
int main (void)
{
	char  received_data;
	system_init();
	delay_init();
	configure_usart_CONSOLE();
	system_interrupt_enable_global();
	configure_usart_STN();
	LedInit();
	int i;
	const char temp;
	printf("\r\n welcome to CARIQ!!!!!!!!!!!!!\r\n");
	printf("\n done with all initializations!!! \n ");
		
//----------------------------------------------------------------------------------------------------------------	
		
	
		
	
	for(i=0;i<4;i++)
		{
			port_pin_set_output_level(PIN_PB03,0);
			delay_ms(1000);
			port_pin_set_output_level(PIN_PB03,1);
			delay_ms(1000);
		}
	
//printf("\n press any key for basic STN Testing \n \r");
//scanf("%d",&temp);
delay_ms(4000);

	


printf("\n sending PID ATZ:");
sendPID("ATZ");
write_DEBUG(&STNResBuff);
delay_ms(2000);

printf("\n sending PID ATRV:");
sendPID("ATRV");
write_DEBUG(&STNResBuff);
delay_ms(2000);

printf("\n sending PID 010C:");
sendPID("010C");
write_DEBUG(&STNResBuff);
delay_ms(2000);

printf("\n sending PID ATDP:");
sendPID("ATDP");
write_DEBUG(&STNResBuff);
delay_ms(2000);


//printf("\n done with basic STN testing!!! press y to proceed!!!!\n \r");
//scanf("%c",&temp);
delay_ms(3000);
//if(temp=='y'|| temp=='Y')
//	{
		
		
		port_pin_set_output_level(PIN_PB03,0);
		delay_ms(1000);
		printf("\n Sameer!!!");
		//void start_scan();

        printf("\n Initializing CAR_SCAN...	");
        delay_ms(20);
        printf("...");
        delay_ms(30);
        printf(".... \n");
        printf("\n Default header\n");
        
        for(i=0;i<TOTAL_PIDS;i++)
        {
	        printf("\n sending PID %s: ",PIDS[i]);
			sendPID(PIDS[i]);
	        write_DEBUG(&STNResBuff);
	        delay_ms(1000);
        }
        //----set headers and fire----------------------------------------
        for(j=0;j<TOTAL_HEADERS;j++)
        {
	        printf("\n headers set is %s \n",Headers[j]);
	        sendPID(Headers[j]);
			for(i=0;i<TOTAL_PIDS;i++)
	        
	        {
		        printf("\n sending PID %s :",PIDS[i]);
		        sendPID(PIDS[i]);
				write_DEBUG(&STNResBuff);
		        delay_ms(500);
	        }
	        
        }
        



//	}









		//write_DEBUG("sendig this cmd: 010C");
		//sendPID("010C");
		
		//printf("\r\nRecieved from STN: %s\r\n", STNResBuff);
	  //  write_DEBUG("testing read  enter any character");
		
	//	write_DEBUG(&received_data);
		
		//delay_ms(2000); 
	
    	
}
