/**
  The filebuffer component will provide a struct
  that contains a resizable array of bytes representing the contents of a file. 
  The starter just has one field in this struct, a pointer to a dynamically allocated 
  array of bytes representing a file's contents.
 */

#ifndef _FILEBUFFER_H_
#define _FILEBUFFER_H_

/** A shorthand for talking about a byte. */
typedef unsigned char byte;

/** Representation of a resizable array of bytes, with functions to
    make it easy to read and write the contents of this array to a
    file. */
typedef struct {
  /** Resizable array of bytes stored in this filebuffer. */
  byte *data;

  // More fields added by you.
  // ..

  int capacity;
  int count;
  
} FileBuffer;

FileBuffer *makeFileBuffer();
void freeFileBuffer( FileBuffer *buffer );
void appendFileBuffer( FileBuffer *buffer, byte val );
FileBuffer *loadFileBuffer( char const *filename );
void saveFileBuffer( FileBuffer *buffer, char const *filename );

#endif
