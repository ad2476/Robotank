/*  Robotank centralised header

	author: Arun Drelich
	date: 22.07.13     */

#ifndef ROBOTANK_H
	#define ROBOTANK_H

#include "saast.h"

#define RF_CHANNEL 3
#define RF_LENGTH 4

/* Mode codes */
#define DRIVE 0x99
#define FIRE 0x66
#define ERR 0xEE

#define TRIGGER 0xE7 // byte 2 MUST be this magic number to fire

#define BITMASK 0x04 // 00000100 to mask third bit

/* Packet indices */
#define MODE 0
#define LEFT 1
#define RIGHT 2
#define CHKSUM 3

#define UPDATE 100 // Update interval in ms

typedef unsigned char uint8; // Since a char is an 8-bit, this clarifies
typedef char int8;
typedef unsigned int uint16;

/* Packet structure:

   {mode, byte 1, byte 2, checksum} 
   --------------------------------
   {DRIVE, left motor speed, right motor speed, checksum}
   {FIRE, tilt value, trigger (magic number E7 fires), checksum} */
   
uint8 recv_buf[RF_LENGTH];
uint8 send_buf[RF_LENGTH];

// c_addr: controller address
// b_addr: brain address
uint8 c_addr = 0x7A, b_addr=0x7C;

/* Creates checksum from a packet based on the third bit of its first three bytes */
/* Eg. {10011001, 01111111, 00000000, 00000000} would generate a checksum of 00000010 */
uint8 genChecksum(uint8* packet) {	
	return (packet[0] & BITMASK) | ((packet[1] & BITMASK)>>1) | ((packet[2] & BITMASK)>>2);
}

/* Conveniently packs data into a Robotank packet, includes checksum */
void packgen(uint8* packet, uint8 mode, uint8 byte1, uint8 byte2) {
	packet[0]=mode; packet[1]=byte1; packet[2]=byte2; packet[3]=genChecksum(packet);
}

/* Convert 8-bit char to 16-bit int */
uint16 ctoi(uint8 number) {
	uint8 str[2];
	str[0]=number;
	str[1]=0;
	
	return (str[1] << 8) | str[0];
}

/* Clears the recv buffer if no new packet has arrived */
/* Returns 0 on error, 1 on success */
int clearbuf(void* buffer, size_t bufsize, size_t typesize) {
	int i;
	if(buffer==NULL)
		return 0;
	
	if(typesize==sizeof(int)) {
		for(i=0; i<bufsize; i++)
			*(((int *)buffer)+i)=0; // Set each element to 0x00
	}
	else if(typesize==sizeof(uint8)) {
		for(i=0; i<bufsize; i++)
			*(((uint8 *)buffer)+i)=0; // Set each element to 0x00
	}
	else return 0;
	
	return 1;
}

#endif

