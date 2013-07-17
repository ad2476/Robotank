/* mWii IR Tracking Test

   author: Arun Drelich
   date: 17.07.13 */
  
#include "../saast.h"

void flashLEDs() {
	m_red(TOGGLE);
	m_green(TOGGLE);
	m_wait(1000);
}

int main(void)
{	
	unsigned int blobs[12];
	char check=0;
	
	/* Initialise m2 board, USB subsystem and mWii camera */
	m_init();
	m_usb_init();
	
	while(!m_wii_open())
		flashLEDs();
	
	
	m_red(ON); // Indicates proper functioning	
	while(1){
		check=m_wii_read(blobs);
		
		/* Start relaying the data over USB */
		if(m_usb_isconnected()) {
			if(check) {
				int i;
				for(i=0; i<12; i++) {	
					m_usb_tx_uint(blobs[i]);
					m_usb_tx_char(' ');
				}
			
			}
			else {
				m_usb_tx_char('E');
				m_usb_tx_char('r');
				m_usb_tx_char('r');
			}
			
			m_usb_tx_char('\n');
		
		}
		
		m_wait(2000);
		
	}
	
	return 0;
}
