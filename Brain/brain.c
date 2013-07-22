/* Wireless Test

   author: Arun Drelich
   date: 19.07.13 */
  
#include "brain.h"

int main(void) {
	int i;
	m_init();
	m_usb_init();
	m_rf_open(RF_CHANNEL, b_addr, RF_LENGTH);
	
	m_red(ON);
	
	while(true) {
		m_wait(1000);
		
		if(m_usb_isconnected()) {
			if(new_packet)
				new_packet=false;
			else
				clearbuf(recv_buf, RF_LENGTH, sizeof (uint8));
				
			for(i=0; i<RF_LENGTH; i++) {
				m_usb_tx_hex(ctoi((uint8)recv_buf[i]));
				m_usb_tx_char(' ');
			}
			m_usb_tx_char('\n');
		}
	}
	
	return 0;
}

ISR(INT2_vect) {
	m_rf_read(recv_buf, RF_LENGTH);
	new_packet=true;
}
