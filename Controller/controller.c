/* Wireless Test

   author: Arun Drelich
   date: 19.07.13 */
  
#include "../Robotank.h"

int main(void) {
	m_init();
	m_rf_open(RF_CHANNEL, c_addr, RF_LENGTH);
	
	m_green(ON);
	
	packgen(send_buf, DRIVE, 100, 0); // Send a "drive right" command
	while(true) {
		m_wait(1000);
		
		m_rf_send(b_addr, send_buf, RF_LENGTH);
	}
	
	return 0;
}

