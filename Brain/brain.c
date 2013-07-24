/* Wireless-controlled driving Test

   author: Arun Drelich
   date: 23.07.13 */
  
#include "brain.h"

#define LMOT 0
#define RMOT 1

float prop(float x, float n) {
	if(x>n)
		return (x-n)/n;
	else
		return (n-x)/n;
}

float scaleSpeed(float speed, float midpoint) {
	if(speed>midpoint)
		return speed*0.5;
	else
		return prop(speed, midpoint)*100;
}

int main(void) {
	float speeds[2]={0,0}; // Array containing motor speeds: {LMOT, RMOT}
	int i;
	int dirs[2];

	mx_init();
	
	m_usb_init();
	m_rf_open(RF_CHANNEL, b_addr, RF_LENGTH);
	
	m_red(ON);
	while(true) {
		m_wait(UPDATE);
		/* Set new_packet flag */
		if(new_packet)
			new_packet=false;
		else
			clearbuf(recv_buf, RF_LENGTH, sizeof (uint8));
		
		if(recv_buf[MODE]==DRIVE) {
			for(i=0; i<2; i++) 
				speeds[i]=recv_buf[i+1]; // Load speeds from recv_buf
			
			/* NOTE:
  
   			Left motor turns CW for forward
   			Right motor turns CCW for forward

			*/
			int dir;	
			for(i=0; i<2; i++) {
				switch(i) {
					case LMOT:
						dir=CCW;
						if(speeds[i]>0x6C)
							dir=CW;
						
						speeds[i]=scaleSpeed(speeds[i], 0x6C);
						break;
					case RMOT:
						dir=CW;
						if(speeds[i]>0x5D)
							dir=CCW;
						
						speeds[i]=scaleSpeed(speeds[i], 0x5D);
				}
				
				dirs[i]=dir;	
				mx_motor(i+1, dir, speeds[i]);		
			}
		}
		
		debug(dirs, 2);
		
	}
	
	return 0;
}

