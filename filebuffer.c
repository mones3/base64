/**
    @file filebuffer.c
    @author Symone Lundy
    The FileBuffer will make it easy to access the input and build the output file.
    The filebuffer component will provide a struct (typedeffed to FileBuffer) that 
    contains a resizable array of bytes representing the contents of a file.
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filebuffer.h"
#include "state24.h"

#define ARRAY_CAPACITY 1

/**
    This function dynamically allocates a FileBuffer, initializes so it contains an 
    empty sequence of bytes and returns a pointer to the new FileBuffer.
    @return fileBuffer the filebuffer pointer to be returned
*/
FileBuffer *makeFileBuffer()
{
    FileBuffer *fileBuffer = (FileBuffer * ) malloc ( sizeof ( FileBuffer ) );
    //fileBuffer -> data = calloc(ARRAY_CAPACITY, sizeof(unsigned int*) * ARRAY_CAPACITY);
    fileBuffer -> data = 0;
    fileBuffer -> capacity = ARRAY_CAPACITY;
    fileBuffer -> count = 0;

    return fileBuffer;
}

/**
    This function frees all the memory used to represent the given FileBuffer.
    @param buffer the buffer to be freed
*/
void freeFileBuffer( FileBuffer *buffer )
{
   
    free(buffer -> data); 
    buffer -> count = 0;
    buffer -> capacity = 0;
    //free( buffer -> data );
    free( buffer );
}

/**
    This function adds a byte with the given value to the end of the byte sequence 
    stored inside the given FileBuffer. It grows the internal data array in FileBuffer 
    if necessary to add the given byte to the end.
    @param buffer the buffer to append to
    @param val the value to be added to filebuffer   
*/
void appendFileBuffer( FileBuffer *buffer, byte val )
{
    if ( buffer -> count >= buffer -> capacity - 1 ) {
        buffer -> capacity = 2 * buffer -> capacity;
        buffer -> data = (byte *)realloc(buffer -> data, buffer -> capacity * (sizeof ( byte )));
    }
    
        buffer -> data[buffer -> count] = val;
        buffer -> count++;
}


/**
    This function reads a binary input file, stores its contents it in the resizable array inside a 
    new FileBuffer and returns it to the caller. After using this function to read a file, 
    client code can access the file's contents via the FileBuffer's data array.
    @param filename the filename to load buffer from
*/
FileBuffer *loadFileBuffer( char const *filename )
{

    FILE *loadFile = fopen( filename, "r" );
    if ( loadFile == NULL) {
   
       perror( filename ); 
       exit(1);
    }
    FileBuffer *loadBuffer = makeFileBuffer();

   int ch;
    while ((ch = fgetc( loadFile ) ) != EOF){
        ungetc(ch, loadFile );
        ch = fgetc(loadFile);

        appendFileBuffer( loadBuffer, ch );
        
    }

 
fclose( loadFile );
return loadBuffer;
//free(loadBuffer);
}

/**
    This function saves the contents of the given FileBuffer to a 
    (binary) file with the given name.
    @param buffer the buffer to be saved
    @param filename the pointer to the file to be saved
*/
void saveFileBuffer( FileBuffer *buffer, char const *filename )
{
    FILE *saveFile = fopen( filename, "wb" );
    if ( !saveFile) {
       perror( filename ); 
    }
        fwrite(buffer -> data, sizeof(byte), buffer -> count, saveFile);
        fclose( saveFile );
        freeFileBuffer(buffer);

}