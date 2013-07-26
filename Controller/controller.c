/* Wireless Test

   author: Arun Drelich
   date: 22.07.13 */
  
#include "../Robotank.h"

#define LPOT B4
#define RPOT B5
#define TILTPOT F4
#define MSWITCH F1 // Mode switch
#define TSWITCH F5 // Trigger switch

#define INPUTSIZE 3 // Size of "raw_input" and "scaled" arrays
#define TILT 0 // Index of tilt value in arrays, but NOT packet

/* Scales a RAW analog input into a DEST buffer of size "size" */
/* "destmax" signifies the max value of the destination scale */
void scale(int* raw, uint8* dest, size_t size, int destmax) {
	int i;
	for(i=0; i<size; i++)
		dest[i]=(((float)raw[i])/1024.0)*destmax;
}

int main(void) {
	int raw_input[INPUTSIZE]; // Holds the raw input from pots
	uint8 scaled[INPUTSIZE]; // Scaled buffer of values to be sent
	
	/* Mode switch, trigger switch, fire code */
	int8 mode_switch;
	int8 trigger_switch=0;
	uint8 fire=0; 
	
	m_init();
	m_rf_open(RF_CHANNEL, c_addr, RF_LENGTH);
	
	while(true) {	
		clearbuf(raw_input, INPUTSIZE, sizeof(int)); // Clear the input buffer
		clearbuf(scaled, INPUTSIZE, sizeof(uint8)); // Clear motor speed buffer
		
		/* Read mode switch (ON means drive, OFF means fire) */
		mode_switch=m_gpio_in(MSWITCH);
		if(mode_switch==ON) {
			m_green(ON);
			m_red(OFF);
			/* Get in pot values and scale*/
			raw_input[LEFT]=m_adc(LPOT);
			raw_input[RIGHT]=m_adc(RPOT);
				
			scale(raw_input, scaled, INPUTSIZE, 200);
			
			/* Generate packet and send! */
			packgen(send_buf, DRIVE, scaled[LEFT], scaled[RIGHT]);			
			m_rf_send(b_addr, (char *)send_buf, RF_LENGTH);
		}
		else {
			m_green(OFF);
			m_red(ON);
			
			/* Get in joystick values, scale: Use joystick to turn turret */
			raw_input[TILT]=m_adc(RPOT);
			scale(raw_input, scaled, INPUTSIZE, 100);
			
			trigger_switch=m_gpio_in(TSWITCH);
			if(trigger_switch==ON)
				fire=TRIGGER;
			else fire=0;
			
			/* Generate packet and send! */
			packgen(send_buf, FIRE, scaled[TILT], fire);
			m_rf_send(b_addr, (char *)send_buf, RF_LENGTH);
		}
		
		m_wait(UPDATE);
	}
	
	return 0;
}

