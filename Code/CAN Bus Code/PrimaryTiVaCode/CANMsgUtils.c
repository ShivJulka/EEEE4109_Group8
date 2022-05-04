/*
 * CANMsgUtils.c
 *
 *  Functions for encoding/decoding CAN msg payload.
 *  Created on: 10 Sep 2021
 *      Author: ezzpe
 */


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



uint8_t WriteToCANMsg( uint8_t *msgData, uint8_t byteOffset, void *value, size_t size ) {
    /* if starting offset > 8- sizeof(value) then value wont fit in message */
    if( byteOffset > 8-size ) return 0;

    memcpy( msgData+byteOffset, value, size );

    return 1;
}



uint8_t ReadFromCANMsg( uint8_t *msgData, uint8_t byteOffset, void *value, size_t size ) {
    /* if starting offset > 8- sizeof(value) then value wont fit in message */
    if( byteOffset > 8-size ) return 0;

    memcpy( value, msgData+byteOffset, size );

    return 1;
}



void WriteByteBitField( uint8_t *byte, uint8_t *b ) {
    *byte =     ( (b[0] >= 0x01) )
             |  ( (b[1] >= 0x01) << 1)
             |  ( (b[2] >= 0x01) << 2)
             |  ( (b[3] >= 0x01) << 3)
             |  ( (b[4] >= 0x01) << 4)
             |  ( (b[5] >= 0x01) << 5)
             |  ( (b[6] >= 0x01) << 6)
             |  ( (b[7] >= 0x01) << 7);
}



void ReadByteBitField( uint8_t *byte, uint8_t *b ) {
    b[0] = ( *byte      & 0x01 )? 0x01: 0x00;
    b[1] = ( *byte >> 1 & 0x01 )? 0x01: 0x00;
    b[2] = ( *byte >> 2 & 0x01 )? 0x01: 0x00;
    b[3] = ( *byte >> 3 & 0x01 )? 0x01: 0x00;
    b[4] = ( *byte >> 4 & 0x01 )? 0x01: 0x00;
    b[5] = ( *byte >> 5 & 0x01 )? 0x01: 0x00;
    b[6] = ( *byte >> 6 & 0x01 )? 0x01: 0x00;
    b[7] = ( *byte >> 7 & 0x01 )? 0x01: 0x00;
}


