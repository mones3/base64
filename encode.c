/**
    @file encode.c
    @author Symone Lundy
    This is the starting point of the encode program. 
    It takes two required command-line arguments, an input file and an output file.
    The input file will consist of any sequence of bytes, but the output file will
    consist only of printable ASCII characters.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filebuffer.h"
#include "state24.h"

#define ARRAY_MAX 100
#define TOTAL_CHAR 76
#define MAX_CHAR 4
#define MAX_BTYE 3


int main ( int argc, char *argv[] ) 
{
    FILE *inputFile;
    FILE *outputFile;
    

if (argc > 3) {
    if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "-b") == 0) {
        if (strcmp(argv[2], "-b") != 0 || strcmp(argv[2], "-p") != 0) {
        fprintf(stderr, "usage: encode [-b] [-p] <input-file> <output-file>\n");
        exit(1);
        }
    }
} 

    inputFile  = fopen( argv[ 1 ], "r" );
        if ( inputFile == NULL ) {
            perror(argv[ 1 ]);
            exit(1);
        }

    


    outputFile = fopen( argv[ 2 ], "w" );
        if ( outputFile == NULL ) {
            perror( argv[ 2 ] );
            exit( 1 );
        }


    //Creating buffer to hold sequence of byte
    FileBuffer *buffer = makeFileBuffer();
    

    //intiliazing state struct which holds an array of bytes
    State24 state;
    initState ( &state );

    //store char values after 
    char charBuffer[ARRAY_MAX];
    int charCount = 0;
    int charTotal = 0;
    

    //reads a binary input file, stores its contents  the resizable array inside a new FileBuffer
    buffer = loadFileBuffer( argv[ 1 ] );

    for (int j = 0; j < buffer -> count; j += MAX_BTYE) {
        //printf("%d\n",j);
         
    //while ( buffer != NULL ) {           
    //loop through buffer array 
    for ( int i = j; i < buffer -> count && state.byteCount != MAX_BTYE; i++ ) { 

        //function adds 8 bits to the state, given by the bits of byte
        addByte( &state, buffer -> data[i] );
    
    }

    //fills in the given array with character codes for up to four encoding characters based on the sequence of bits
    //while (charCount < 4 ) {
    charCount = getChars( &state, charBuffer);
    fwrite(charBuffer, sizeof(char), charCount, outputFile);
    charTotal = charTotal + MAX_CHAR;

    if (charTotal == TOTAL_CHAR ){
        fprintf(outputFile, "\n");
        charTotal = 0;
    }
    
    }
    fprintf(outputFile, "\n");
    
    
    fclose( outputFile );
   
    freeFileBuffer( buffer );

    return 0;
    
}