/* mWii IR Tracking Test

   author: Arun Drelich
   date: 17.07.13 */
  
#include "../saast.h"

#define BLOBS 4
#define ELEMENTS 12
#define MAXP 1023
#define MAXS 15

#define X_LEFT 700
#define X_CENTER 500
#define X_RIGHT 300
#define UNDEF 1023

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
	int dir;
	
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
		
		dir=Blobs[0].x; // X coordinate of first blob
		
		/* Start relaying the data over USB */
		if(m_usb_isconnected()) {
			if(check) {					
				m_usb_tx_string("Blob: ");
				m_usb_tx_int(Blobs[0].x);
				m_usb_tx_string(" | ");
				
				if(dir==UNDEF)
					m_usb_tx_string("No target");
				else if(dir>=X_LEFT)
					m_usb_tx_string("Left");
				else if((dir<X_LEFT)&&(dir>X_RIGHT))
					m_usb_tx_string("Centered");
				else if(dir==X_CENTER)
					m_usb_tx_string("On target");
				else if(dir<X_RIGHT)
					m_usb_tx_string("Right");
				else
					m_usb_tx_string("Error");
			}
			else {
				m_usb_tx_string("Error");
			}
			
			m_usb_tx_char('\n');
		
		}
		
		m_wait(2000);
		
	}
	
	return 0;
}
