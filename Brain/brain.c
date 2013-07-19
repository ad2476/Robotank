/* Wireless Test

   author: Arun Drelich
   date: 19.07.13 */
  
#include "../Robotank.h"

int main(void) {
	m_init();
	m_usb_init();
	m_rf_open(RF_CHANNEL, b_addr, RF_LENGTH);
	
	m_red(ON);
	
	while(true) {
		m_wait(1000);
		
		
	}
	
	return 0;
}

// Interrupt handler to process incoming mRF packets
ISR(INT2_vect) {
	m_rf_read(recv_buf, RF_LENGTH);
}
