#include <avr/io.h>
#include <avr/interrupt.h>
#include "bt_driver.h"

void init_uart(void)
{
    UBRRH = (unsigned char)(BAUD_RATE>>8);
    UBRRL = (unsigned char)BAUD_RATE;

    UCSRB = (1<<RXEN)|(1<<TXEN) |(1<<RXCIE); // enable tx, rx, int
    UCSRC = (1<<URSEL) | (1<<UCSZ0)|(1<<UCSZ1); // async, 8-bit
}

void uart_tx(char ch)
{
    while (! (UCSRA & (1<<UDRE)));  /* Wait for empty transmit buffer */
    UDR = ch;
}

unsigned char uart_rx(void)
{
    while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */
    return UDR;
}

UART uart = {
    .init = &init_uart,
    .tx = &uart_tx,
    .rx = &uart_rx
};

ISR(USART_RXC_vect)
{
    char c = uart.rx();
    uart.tx(c);
}
