/* Wireless-controlled driving Test

   author: Arun Drelich
   date: 23.07.13 */
  
#include "brain.h"

#define LMOT 0
#define RMOT 1

#define MIDPOINT 70

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
	// Initialise servos
	mx_servo_init(PAN);
	mx_servo_init(LOADER);
	
	m_usb_init();
	m_rf_open(RF_CHANNEL, b_addr, RF_LENGTH);
	
	m_red(ON);
	mx_servo(PAN, MIDPOINT);
	m_gpio_out(BEACON, ON);
	while(true) {
		m_wait(UPDATE);
		mx_servo(LOADER, 50);
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
						if(speeds[i]>0x67)
							dir=CW;
						
						speeds[i]=scaleSpeed(speeds[i], 0x67);
						break;
					case RMOT:
						dir=CW;
						if(speeds[i]>0x57)
							dir=CCW;
						
						speeds[i]=scaleSpeed(speeds[i], 0x57);
				}
					
				mx_motor(i+1, dir, speeds[i]);		
			}
			
			mx_servo(PAN, MIDPOINT);
		}
		else if(recv_buf[MODE]==FIRE) {
			mx_motor(LEFT, CW, 0);
			mx_motor(RIGHT, CCW, 0);
			
			//targetAcquisition();
			
			// Wait for the TRIGGER to be pressed or to be switched back to drive mode
			while(recv_buf[2]!=TRIGGER) {
				float tiltdir=MIDPOINT;
				//m_usb_tx_string("Waiting for trigger...\n");
				debug(recv_buf, RF_LENGTH);
				
				if(recv_buf[1]>90)
					tiltdir=100;
				else if(recv_buf[1]<10)
					tiltdir=-50;
				else
					tiltdir=(float)recv_buf[1];
				
				mx_servo(PAN, tiltdir);
				
				if(recv_buf[MODE]==DRIVE) {
					m_usb_tx_string("DRIVE\n");
					break;
				}
				
				m_wait(200);
			}
			
			if(recv_buf[MODE]==DRIVE)
				continue;
			
			m_gpio_out(SHOOTER1, ON);
			m_gpio_out(SHOOTER2, ON);
			
			m_wait(3000);
			
			mx_servo(LOADER, 0);
			m_wait(500);
			mx_servo(LOADER, 50);
			
			m_gpio_out(SHOOTER1, OFF);
			m_gpio_out(SHOOTER2, OFF);
			
			m_wait(1000);
		
		}
		
		debug(recv_buf, RF_LENGTH);
		
	}
	
	return 0;
}

