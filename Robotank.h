/*  Robotank centralised header

	author: Arun Drelich
	date: 19.07.13     */

#ifndef ROBOTANK_H
	#define ROBOTANK_H

#include "saast.h"

#define RF_CHANNEL 4
#define RF_LENGTH 4

/* Mode codes */
#define DRIVE 0x99
#define FIRE 0x66
#define TRIGGER E7 // byte 2 MUST be this magic number to fire

#define BITMASK 0x04 // 00000100 to mask third bit

typedef unsigned char uint8; // Since a char is an 8-bit, this clarifies
typedef char int8;

/* Packet structure:

   {mode, byte 1, byte 2, checksum} 
   --------------------------------
   {DRIVE, left motor speed, right motor speed, checksum}
   {FIRE, tilt value, trigger (magic number E7 fires), checksum} */
   
int8 recv_buf[RF_LENGTH];
int8 send_buf[RF_LENGTH];

// c_addr: controller address
// b_addr: brain address
uint8 c_addr = 0xAB, b_addr=0xCD;

/* Creates checksum from a packet based on the third bit of its first three bytes */
/* Eg. {10011001, 01111111, 00000000, 00000000} would generate a checksum of 00000010 */
/* Returns 0xFF on error */
int8 genChecksum(int8* packet) {	
	return (packet[0] & BITMASK) | ((packet[1] & BITMASK)>>1) | ((packet[2] & BITMASK)>>2);
}

/* Conveniently packs data into a Robotank packet, includes checksum */
/* Returns 0 on error, 1 on success */
void packgen(int8* packet, int8 mode, int8 byte1, int8 byte2) {
	packet[0]=mode; packet[1]=byte1; packet[2]=byte2; packet[3]=genChecksum(packet);
}

#endif

