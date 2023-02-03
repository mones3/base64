/**
    @file decode.c
    @author Symone Lundy
    This is the starting point of the decode program. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filebuffer.h"
#include "state24.h"

#define ARRAY_MAX 100
#define MAX_CHAR 4
#define MAX_BTYE 3



int main ( int argc, char *argv[] ) 
{

  FILE *inputFile = fopen( argv[ 1 ], "r" );
    if ( inputFile == NULL ) {
       perror(argv[ 1 ]);
       exit(1);
    }
    fclose ( inputFile );


    FILE *outputFile = fopen( argv[ 2 ], "wb" );
        if ( outputFile == NULL ) {
            perror( argv[ 2 ] );
            fclose(inputFile);
            exit( 1 );
        }

    //Creating buffer to hold sequence of byte
    FileBuffer *buffer = makeFileBuffer();

    //intiliazing state struct which holds an array of bytes
    State24 state;
    initState ( &state );

    byte byteBuffer[ARRAY_MAX];
    int byteCount = 0;
    //int charTotal = 0;

    //reads a binary input file, stores its contents in the resizable array inside a new FileBuffer
    buffer = loadFileBuffer( argv[ 1 ] );

    //increment to the next char in buffer
    for (int j = 0; j < buffer -> count; j += MAX_CHAR) {
             
    //loop through buffer array until state24 is filled
    for (  int i = j; i < buffer -> count && state.byteCount < MAX_BTYE; i++ ) { 
        
        //function adds 6 bits to the state, given an encoding character
        addChar( &state, buffer -> data[i] );
        //printf("%c", buffer -> data[j] );
    
    }

    //This function fills in the given array with up to three bytes based on the sequence
    byteCount = getBytes( &state, byteBuffer);
    
    fwrite(byteBuffer, sizeof(byte), byteCount, outputFile);

    }

    freeFileBuffer( buffer );
    
    fclose(outputFile);

}