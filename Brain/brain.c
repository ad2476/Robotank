/* USB Com Test

   author: Arun Drelich
   date: 15.07.13 */
  
#include "../saast.h"

int main(void)
{	
	m_init();
	m_usb_init();

	char wii_ok=m_wii_open();
	char check=0;
	
	unsigned int blobs[12];	
		
	while(1){
		m_red(ON); // Indicates board is operating
		
		
		/* Turn on test IR LEDs */
		m_gpio_out(B0, ON);
		m_gpio_out(B1, ON);
		m_gpio_out(B2, ON);
		m_gpio_out(B3, ON);
		
		
		/* Check that the Wii Blob is working */
		if(wii_ok)
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
			else
				m_usb_tx_uint(wii_ok);
			
			m_usb_tx_char('\n');
		
		}
		
		m_wait(2000);
		
	}
}
