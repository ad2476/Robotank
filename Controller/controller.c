/* Wireless Test

   author: Arun Drelich
   date: 19.07.13 */
  
#include "../Robotank.h"

int main(void)
{	
	m_init();
	
	m_red(ON); // Indicates board is operating
	while(1){
		/* Turn on test IR LEDs */
		m_gpio_out(B4, ON);
		m_gpio_out(B1, ON);
		m_gpio_out(B2, ON);
		m_gpio_out(B3, ON);	
	}
	
	return 0;
}
