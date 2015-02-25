/**
 * \file
 *
 * \brief User board configuration template
 *
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define PINS_UART_PIO  PIOA
#define PINS_UART        (PIO_PA8A_URXD | PIO_PA9A_UTXD)
#define PINS_UART_FLAGS  (PIO_PERIPH_A | PIO_DEFAULT)

/* ------------------------------------------------------------------------ */
/* USART0                                                                   */
/* ------------------------------------------------------------------------ */
/*! USART0 pin RX */
#define PIN_USART0_RXD_IDX        (PIO_PA10_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/*! USART0 pin TX */
#define PIN_USART0_TXD_IDX        (PIO_PA11_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)

/* ------------------------------------------------------------------------ */
/* USART1                                                                   */
/* ------------------------------------------------------------------------ */
/*! USART1 pin RX */
#define PIN_USART1_RXD_IDX        (PIO_PA12_IDX)
#define PIN_USART1_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/*! USART1 pin TX */
#define PIN_USART1_TXD_IDX        (PIO_PA13_IDX)
#define PIN_USART1_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)

#endif // CONF_BOARD_H
