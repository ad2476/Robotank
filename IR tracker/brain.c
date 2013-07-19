#include "brain.h"

int main(void)
{
    mx_init();
    mx_servo_init(PAN);
    int angle = 150;
	int center, bestAngle;
	int bestValue = -10;
	
	mx_servo(PAN,150);
	m_wait(10000);
	while (42)
	{
		while (angle>-50)
		{
			center = m_adc(CENTER);
			angle--;
			m_wait(300);
			mx_servo(PAN,angle);
			if (bestValue<center)			
			{
				bestValue=center;
				bestAngle = angle;
			}
		}
		mx_servo(PAN,bestAngle);
	}
            
}
