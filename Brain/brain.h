/* Brain.h 

   Author: Arun Drelich
   Date: 22.07.13
*/

#ifndef BRAIN_H
#define BRAIN_H

#include "../Robotank.h"

volatile uint8 new_packet=false; // Flag signifying new packet

/* Convert 8-bit char to 16-bit int */
uint16 ctoi(uint8 number) {
	uint8 str[2];
	str[0]=number;
	str[1]=0;
	
	return (str[1] << 8) | str[0];
}

/* Interrupt service routine to handle incoming packets */
ISR(INT2_vect) {
	m_rf_read((char *)recv_buf, RF_LENGTH);
	if(recv_buf[CHKSUM]!=genChecksum(recv_buf)) {
		recv_buf[MODE]=ERR;
		recv_buf[1]=CHKSUM; // It's a bad checksum
	}
		
	new_packet=true;
}

#endif
