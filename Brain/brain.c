/* mWii IR Tracking Test

   author: Arun Drelich
   date: 17.07.13 */
  
#include "../saast.h"

#define BLOBS 4
#define ELEMENTS 12
#define MAXP 1023
#define MAXS 15

#define X_OFFSET 200
#define X_CENTER 500

struct mWii {
	int x;
	int y;
	int size;
};

typedef struct mWii irCam;

void flashLEDs() {
	m_red(TOGGLE);
	m_green(TOGGLE);
	m_wait(1000);
}

void blobPack(unsigned int* wiinput, irCam* blobs) {
	int i, thisBlob;
	for(i=0; i<ELEMENTS; i++) {
		thisBlob=i/3;
		
		switch(i%3) {
			case 0:
				blobs[thisBlob].x=wiinput[i]; break;
			case 1:
				blobs[thisBlob].y=wiinput[i]; break;
			case 2:
				blobs[thisBlob].size=wiinput[i]; break;
		}
	}
}

int main(void)
{	
	unsigned int raw_in[12];
	char check=0;
	irCam Blobs[4];
	
	/* Initialise m2 board, USB subsystem and mWii camera */
	m_init();
	m_usb_init();
	
	while(!m_wii_open())
		flashLEDs();
	
	m_red(ON);
	m_green(OFF);	
	while(1){
		m_red(TOGGLE);
		m_green(TOGGLE);
		check=m_wii_read(raw_in);
		
		/* Pack blobs data into struct */
		blobPack(raw_in, Blobs);
		
		
		
		/* Start relaying the data over USB */
		if(m_usb_isconnected()) {
			if(check) {
				int i;
				for(i=0; i<12; i++) {
					if(i==0)
						m_usb_tx_char('(');
					else if(i%3==0) {
						m_usb_tx_string(")(");
					}
					
					m_usb_tx_uint(raw_in[i]);
					if(i==11)
						m_usb_tx_char(')');
					else
						m_usb_tx_char(' ');
				}
			
			}
			else {
				m_usb_tx_char('E');
			}
			
			m_usb_tx_char('\n');
		
		}
		
		m_wait(2000);
		
	}
	
	return 0;
}
