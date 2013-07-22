/* Brain.h 

   Author: William Kayat
   Date: 18.07.13
*/

#ifndef BRAIN_H
#define BRAIN_H

#include "../Robotank.h"

volatile uint8 new_packet=false; // Flag signifying new packet

/* Convert 8-bit char to 16-bit int */
uint16 ctoi(uint8 number) {
	uint8 str[2];
	str[0]=number;
	str[1]=0;
	
	return (str[1] << 8) | str[0];
}

#endif
