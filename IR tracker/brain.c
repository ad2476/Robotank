/* IR Tracking: Two Phototransistor test

	Authors: William Kayat and Arun Drelich
	Date: 24.07.13
*/

#include "brain.h"

int main(void)
{
    mx_init();
    m_usb_init();
    mx_servo_init(PAN);
    int angle = 150;
	int left, right, center, bestAngle;
	int bestValue = -10;
	
	char lastdir=LEFT;
	
	mx_servo(PAN,-150); // Start left
	m_red(ON);
	m_wait(1000);
	
	/* Sweep for target */
	while (angle>-50)
	{
		center = m_adc(CENTER);
		angle-=5;
		m_wait(200);
		mx_servo(PAN,angle);
		if (bestValue<center)			
		{
			bestValue=center;
			bestAngle = angle;
		}
	}
	mx_servo(PAN,bestAngle);
	m_wait(500);
	
	angle=bestAngle;
	
	/* Follow target if it moves */
	while (42)
	{		
		left = m_adc(LEFT);
		right = m_adc(RIGHT);
		center = m_adc(CENTER);
		
		if((center>right)&&(center>left)) // Target lock!
			angle=angle;
		else if((right<2)&&(left<2)) { // There's no IR target
			if(lastdir==LEFT) {
				angle+=5;
			}
			else {
				angle-=5;
			}
		}
		else if ((center-right>10)||(left-center>10)) {
			angle+=5;
			lastdir=LEFT;
		}
		else if((center-left>10)||(right-center>10)) {
			angle-=5;
			lastdir=RIGHT;
		}
		
		mx_servo(PAN,angle);
		m_wait(200);
	}
            
}
