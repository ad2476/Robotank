/* USB Com Test

   author: Arun Drelich
   date: 15.07.13 */
  
#include "../saast.h"

int main(void)
{	
	m_init();
	m_usb_init();
		
	while(1){
		m_red(ON); // Indicates board is operating
		
		/* Stuff goes here */
		
	}
}
