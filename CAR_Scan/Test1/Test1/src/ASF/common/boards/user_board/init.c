/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	
	gpio_configure_pin(PIN_USART1_RXD_IDX, PIN_USART1_RXD_FLAGS);
	gpio_configure_pin(PIN_USART1_TXD_IDX, PIN_USART1_TXD_FLAGS);
}
