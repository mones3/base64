/**
  The state24 component stores the current state of the encoding or decoding, 
  represented up to 24 bits. Its job is to make it easy to convert between bytes 
  and encoding characters for up to 24 bits at a time.
 */

#ifndef _STATE64_H_
#define _STATE64_H_

#include <stdbool.h>
#include <string.h>

// Include filebuffer to get the byte type.
#include "filebuffer.h"

#define BYTES_IN_ARRAY 3
//#define CHARS_IN_ARRAY 4



typedef struct {
  // Defined by you.
  // ...
  int byteCount;
  int bitCount;
  byte arrayBytes[BYTES_IN_ARRAY];
  
} State24;

void initState( State24 *state );
bool validChar( char ch );
void addByte( State24 *state, byte b );
void addChar( State24 *state, char ch );
int getBytes( State24 *state, byte buffer[] );
int getChars( State24 *state, char buffer[] );

#endif
