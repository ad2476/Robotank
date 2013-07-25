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

void targetAcquisition() {
	int i, bestAngle, input;
	int angle = 150, bestValue = -10;
	char IR_channels[3]={F, E, D};
	
	mx_servo(PAN,-150); // Start left
	
	bestAngle=angle; // Just in case we find nothing, return to start
	
	/* Sweep for target */
	while (angle>-50)
	{
		/* Read each phototransistor input
		   and evaluate */
		for(i=0; i<3; i++) {
			input = m_adc(IR_channels[i]);
			
			m_usb_tx_int((int)IR_channels[i]);
			m_usb_tx_string(", ");
			m_usb_tx_int(input);
			m_usb_tx_char('\n');
		
			/* Is this the highest we've seen so far? (with a MOE of 10) */
			if (input-bestValue>IR_THRESH) { 
				bestValue=input;
				bestAngle = angle;
			}
		}
		
		angle-=5;
		m_wait(200);
		mx_servo(PAN,angle);
		
		if(recv_buf[MODE]==DRIVE)
			return;
	}
	mx_servo(PAN,bestAngle);
	
	m_wait(1000);
	
}

int main(void) {
	float speeds[2]={0,0}; // Array containing motor speeds: {LMOT, RMOT}
	int i;

	mx_init();
	mx_servo_init(PAN);
	
	m_usb_init();
	m_rf_open(RF_CHANNEL, b_addr, RF_LENGTH);
	
	m_red(ON);
	mx_servo(PAN, 0);
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
					
				mx_motor(i+1, dir, speeds[i]);		
			}
			
			mx_servo(PAN, 50);
		}
		else if(recv_buf[MODE]==FIRE) {
			mx_motor(LEFT, CW, 0);
			mx_motor(RIGHT, CCW, 0);
			
			targetAcquisition();
			
			while(recv_buf[2]!=TRIGGER) ; // Just wait
		
		}
		
		debug(recv_buf, RF_LENGTH);
		
	}
	
	return 0;
}

