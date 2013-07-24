/* Brain.h 

   Author: Arun Drelich
   Date: 22.07.13
*/

#ifndef BRAIN_H
#define BRAIN_H

#include "../Robotank.h"

volatile uint8 new_packet=false; // Flag signifying new packet

/* Checks if a buffer is empty */
/* Returns 1 if empty, 0 if not */
int bufEmpty(uint8* buffer, size_t size) {
	int i;
	for(i=0; i<size; i++) {
		if(buffer[i]!=0)
			return 0;
	}
	
	return 1;
}

void debug(int* array, size_t size) {
	int i;
	if(m_usb_isconnected()) {			
		/* Process what's in the buffer */
		if(bufEmpty(recv_buf, RF_LENGTH))
			m_usb_tx_string("EMPTY BUFFER");
		else if(recv_buf[MODE]==ERR) {
			switch(recv_buf[1]) {
				case CHKSUM:
					m_usb_tx_string("BAD CHECKSUM");
					break;
				default:
					m_usb_tx_string("ERROR");
			}
		}
		else {
			for(i=0; i<size; i++) {
				//m_usb_tx_int(ctoi((uint8)recv_buf[i]));
				m_usb_tx_int(array[i]);
				m_usb_tx_char(' ');
			}
		}
			
		m_usb_tx_char('\n');
	}
}

/* Interrupt service routine to handle incoming packets */
ISR(INT2_vect) {
	m_rf_read((char *)recv_buf, RF_LENGTH);
	if(recv_buf[CHKSUM]!=genChecksum(recv_buf)) {
		// NOTE: For now, just ignore bad checksum packets
		recv_buf[MODE]=ERR;
		recv_buf[1]=CHKSUM; // It's a bad checksum
		
		new_packet=false;
	}
	else
		new_packet=true;
}


#endif

