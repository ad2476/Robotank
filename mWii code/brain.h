/* Brain.h 

   Author: Arun Drelich
   Date: 17.07.13
*/

#ifndef BRAIN_H
#define BRAIN_H

#include "../saast.h"

#define BLOBS 4
#define ELEMENTS 12
#define MAXP 1023
#define MAXS 15

#define X_LEFT 600
#define X_CENTER 500
#define X_RIGHT 400
#define UNDEF 1023

#define SERVO G

struct mWii {
	int x;
	int y;
	int size;
};

typedef struct mWii irCam;

#endif
