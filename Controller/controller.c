/* mWii IR Tracking Test

   author: Arun Drelich
   date: 17.07.13 */
  
#include "../saast.h"

int main(void)
{	
	m_init();
	
	m_red(ON); // Indicates board is operating
	while(1){
		/* Turn on test IR LEDs */
		m_gpio_out(B0, ON);
		m_gpio_out(B1, ON);
		m_gpio_out(B2, ON);
		m_gpio_out(B3, ON);	
	}
	
	return 0;
}
