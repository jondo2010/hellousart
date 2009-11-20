/*
 * 	main.c
 * 	Test application for the USART library.
 *
 * 	Michael Jean <michael.jean@shaw.ca>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <usart.h>

#define BAUD_RATE 9600

void
rx_full_callback (void)
{
	char *msg = "Buffer is full.\r\n";
	usart0_write_to_tx_buf (msg, strlen(msg));
}

void
rx_overrun_callback (void)
{
	char *msg = "Buffer overrun!\r\n";
	usart0_write_to_tx_buf (msg, strlen(msg));
}


void
rx_byte_callback (uint8_t byte)
{
	char msg[64];
	char buf[9];

	snprintf(msg, 64, "Byte %02x received\r\n", byte);
	usart0_write_to_tx_buf (msg, strlen(msg));

	switch (byte)
	{
		case '@':

			usart0_flush_rx_buf ();
			sprintf(msg, "Flushing buffer.\r\n");
			usart0_write_to_tx_buf (msg, strlen(msg));

			break;

		case '#':

			usart0_read_from_rx_buf (buf, 8, 1);
			snprintf(msg, 64, "Buffer: `%s'\r\n", buf);
			usart0_write_to_tx_buf (msg, strlen(msg));

			break;
	}

	PORTB ^= _BV (PB0);
}

void
rx_newline_callback (void)
{
	char *msg = "Newline received.\r\n";
	usart0_write_to_tx_buf (msg, strlen(msg));
}

int
main (void)
{
	sei ();

	DDRB |= _BV (PB0);

	usart0_init (BAUD_RATE);

	usart0_set_rx_byte_callback (&rx_byte_callback);
	usart0_set_rx_newline_callback (&rx_newline_callback);
	usart0_set_rx_full_callback (&rx_full_callback);
	usart0_set_rx_overrun_callback (&rx_overrun_callback);

	usart0_enable_rx ();
	usart0_enable_tx ();

	PORTB |= _BV (PB0);

	for (;;)
		;

	return 0;
}
