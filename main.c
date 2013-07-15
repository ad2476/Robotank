/* USB Com Test

   author: Arun Drelich
   date: 15.07.13 */
  
#include "saast.h"

#define LEFT_IR_OUT A // input channel
#define LEFT_IR_IN B // output channel

#define RIGHT_IR_IN G
#define RIGHT_IR_OUT H

int main(void)
{	
	mx_init();
	m_usb_init();
		
	while(1){
		m_red(ON);
		if(m_usb_isconnected()) {		
			m_usb_tx_int(1234);
			m_usb_tx_char('\n');
						
			m_wait(2000);
		}	
	
	}
}
