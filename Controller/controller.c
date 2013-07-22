/* Wireless Test

   author: Arun Drelich
   date: 19.07.13 */
  
#include "../Robotank.h"

uint8 modes[2]={DRIVE, FIRE};
uint8 byte1[2]={100, 50};
uint8 byte2[2]={10, TRIGGER};

int main(void) {
	m_init();
	m_rf_open(RF_CHANNEL, c_addr, RF_LENGTH);
	
	m_green(ON);
	
	while(true) {
		int i;
		for(i=0; i<2; i++) {
			m_wait(1000);
			packgen(send_buf, modes[i], byte1[i], byte2[i]);
			m_rf_send(b_addr, send_buf, RF_LENGTH);
		}
	}
	
	return 0;
}

