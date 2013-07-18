/* mWii IR Tracking Test

   author: Arun Drelich
   date: 17.07.13 */
  
#include "brain.h"

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
	int dir, rot=0; // Direction of LED, Rotation of servo
	
	/* Initialise m2 board, USB subsystem and mWii camera */
	mx_init();
	m_usb_init();
	mx_servo_init(SERVO);
	
	while(!m_wii_open())
		flashLEDs();
	
	m_red(ON);
	m_green(OFF);
	mx_servo(SERVO, rot); // Set servo to 0
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
				else if(dir>=X_LEFT) {
					rot+=10;
					m_usb_tx_string("Left");
					mx_servo(SERVO, rot);
				}
				else if((dir<X_LEFT)&&(dir>X_RIGHT))
					m_usb_tx_string("Centered");
				else if(dir==X_CENTER)
					m_usb_tx_string("On target");
				else if(dir<X_RIGHT) {
					rot-=10;
					m_usb_tx_string("Right");
					mx_servo(SERVO, rot);
				}
				else
					m_usb_tx_string("Error");
				
				m_usb_tx_string(" | Servo at ");
				m_usb_tx_int(rot);
				m_usb_tx_string(" percent");
			}
			else {
				m_usb_tx_string("Error");
			}
			
			m_usb_tx_char('\n');
		
		}
		
		if(rot>100) rot=100;
		if(rot<0) rot=0;
		
		m_wait(2000);
	}
	
	return 0;
}
