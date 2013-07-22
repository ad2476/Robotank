/* Wireless Test

   author: Arun Drelich
   date: 22.07.13 */
  
#include "brain.h"

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

int main(void) {
	int i;
	mx_init();
	m_usb_init();
	m_rf_open(RF_CHANNEL, b_addr, RF_LENGTH);
	
	m_red(ON);
	while(true) {
		m_wait(1000);
		/* Set new_packet flag */
		if(new_packet)
			new_packet=false;
		else
			clearbuf(recv_buf, RF_LENGTH, sizeof (uint8));
		
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
				for(i=0; i<RF_LENGTH; i++) {
					m_usb_tx_hex(ctoi((uint8)recv_buf[i]));
					m_usb_tx_char(' ');
				}
			}
				
			
			m_usb_tx_char('\n');
		}
	}
	
	return 0;
}

