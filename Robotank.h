/* Wireless Test: 19.07.13

	Arun Drelich */

#ifndef ROBOTANK_H
#define ROBOTANK_H

#include "saast.h"

#define RF_CHANNEL 4
#define RF_LENGTH 2

/* Packet structure:

   {bool red, bool green} */
   
char recv_buf[RF_LENGTH];
char send_buf[RF_LENGTH];

// c_addr: controller address
// b_addr: brain address
char c_addr = 0xAB, b_addr=0xCD;

// Interrupt handler to process incoming mRF packets
ISR(INT2_vect) {
	m_rf_read(recv_buf, RF_LENGTH);
}

#endif
