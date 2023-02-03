/**
    @file state24.c
    @author Symone Lundy
    The state24 component stores the current state of the encoding or decoding, 
    represented up to 24 bits. Its job is to make it easy to convert between 
    bytes and encoding characters for up to 24 bits at a time.  
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "state24.h"


#define EMPTY_ARRAY 0
#define NUMBER_OF_BITS 6
#define BITS_IN_BYTE 8
#define ADD_FOUR_BITS 4
#define INDEX_TWO 2
#define BIT_COUNT_TWELVE 12
#define BIT_COUNT_EIGHTEEN 18
#define PADDING_MULTIPLE 3



/**
    Given a pointer to a State24 struct, this function will initialize it to an empty state,
    where no bits have been added yet. 
    @param state the poiinter to the State24 struct
*/
void initState( State24 *state )
{
    //state -> arrayBytes[BYTES_IN_ARRAY];
    for (int i = 0; i < BYTES_IN_ARRAY; i++){
         state -> arrayBytes[i] = EMPTY_ARRAY;
    }

    state -> byteCount = 0;
    state -> bitCount = 0;
}

/**
    This function returns true if the parameter is one of the 64 encoding characters 
    given in the table in the requirements seciton. 
    Client code can use this to check whether a character is valid before calling addChar().
    @param ch the character to check 
    @return true if character if valid, else returns false
*/
bool validChar( char ch )
{
   
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '+' || ch == '/') {
        return true;
    }

    return false;
}

/**
    This function adds 8 bits to the state, given by the bits of byte
    @param state the pointer to the State24 struct to be added to
    @param b the btye to be added
*/
void addByte( State24 *state, byte b )
{
    
        state -> arrayBytes[state -> byteCount] = b;
        state -> byteCount++;
        state -> bitCount += BITS_IN_BYTE;


}

/**
    This function adds 6 bits to the state, given an encoding character.
    For example, if ch is 'a', then this function will add the bits "011010"; 
    that's the bit sequence represented by character 'a'. 
    Behavior is undefined if the given character isn't an encoding character 
    or if there isn't enough room for six more bits.
    @param state the pointer to the State24 struct to be added to
    @param ch the character to be added
*/
void addChar( State24 *state, char ch )
{
    
    unsigned int value;
    
    char baseCharacter[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    for (int i = 0; i < sizeof(baseCharacter); i++) {
        if (baseCharacter[i] == ch) {
            
            //Add six bits to the beginning of the new bytes
            if ( state -> bitCount == 0)  {
                value = state -> arrayBytes[0]; 
                value = i << INDEX_TWO;
                
                state -> arrayBytes[0] = value;
                state -> bitCount += NUMBER_OF_BITS;
               
            //Add two bits to the end of the byte 
            } else if (state -> bitCount == NUMBER_OF_BITS ) {
                unsigned int maskToSetLastTwoBitsInValue = 00000011;
              
                value = value & maskToSetLastTwoBitsInValue;
                state -> bitCount += INDEX_TWO;
                state -> byteCount++;
                   
                //addfour bits to the next byte
                unsigned int maskToSetFirstFOURBitsInValue = 11110000;
                //printf("%d\n", i);
                state -> arrayBytes[1] = (i << ADD_FOUR_BITS) & maskToSetFirstFOURBitsInValue;
                
                state -> bitCount += 4;
                
            
            //add four bits to end

            } else if ( state -> bitCount == BIT_COUNT_TWELVE ) {
                
                //orginial value
                value =  state -> arrayBytes[1]; // correct
        
                unsigned int endValue = (i >> INDEX_TWO);
                
                state -> arrayBytes[1] = value | endValue;
                state -> bitCount += ADD_FOUR_BITS;
                state -> byteCount++;
               
                //Add remaining 2 bits to the next byte
                
                unsigned int maskToSetFirstTwoBitsInValue = 1100000;
                
                state -> arrayBytes[INDEX_TWO] = (i << NUMBER_OF_BITS) & maskToSetFirstTwoBitsInValue;
                
                //state -> arrayBytes[2] = newValue;
                 
                state -> bitCount += 2;

                //add 6 bits to end
            } else if ( state -> bitCount == BIT_COUNT_EIGHTEEN ){
                //original value
                value = state -> arrayBytes[INDEX_TWO];
                
                unsigned int endValue = i;
             
                value = value | endValue;
                
                state -> arrayBytes[INDEX_TWO] = value;
                
                state -> byteCount++;
               
               }
                
            }
    
        }
       
    }



/**
    This function fills in the given array with up to three bytes based on the sequence 
    of bits in the given State24 struct. It returns the number of bytes written to the 
    array and sets the given state to empty.
    @param state the pointer to the State24 struct
    @param buffer[] array to be filled in with up to three bytes 
    @return returns the number of bytes written to the array
*/
int getBytes( State24 *state, byte buffer[] )
{
    int bufferCount;
    memcpy( buffer, state -> arrayBytes, state -> byteCount );
    bufferCount = state -> byteCount;
    initState( state );
    return bufferCount;

}

/**
    This function fills in the given array with character codes for up to four encoding characters 
    based on the sequence of bits in the given State24 struct. It returns the number of characters 
    written to the array and sets the given state to empty. 
    @param state the pointer to the State24 struct
    @param buffer array to be filled in with character codes for up to four encoding charaters
    @return returns the number of characters written to the array
*/
int getChars( State24 *state, char buffer[] )
{
    char baseCharacter[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
    int charCount = 0;
    int temp = 0;
    int index = 0;
    int value;
    int padding; 

    for ( int i = 0; i < state -> byteCount; i++) {
        value = value << BITS_IN_BYTE;
        value = value | state -> arrayBytes[i]; 
    }
    
    padding = state -> bitCount % PADDING_MULTIPLE;
    

    while (state -> bitCount != 0) {
              
      if (state -> bitCount >= NUMBER_OF_BITS) {
          
            temp = state -> bitCount - NUMBER_OF_BITS;
            index = (value >> temp) & 0x3F;
            state -> bitCount -= NUMBER_OF_BITS;
        
       } else {
            temp = NUMBER_OF_BITS -  state -> bitCount;
            index = (value << temp) & 0x3F;
            state -> bitCount  = 0;
           
        }

    buffer[charCount++] = baseCharacter[index];
         
    }

    for (int i = 1; i <= padding; i++) {
        
        buffer[charCount++] = '=';
    }
 
    initState( state );

    return charCount;
    
}