#ifndef BUS_ENCODER_H
#define BUS_ENCODER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
// following include are needed to use HD function


#define MAX_BITS 256
//#define DEBUG_MODE

//local variables
/*
* bus_inv = Bus-invert
* t_part = two-partitoned Bus-invert
* gray = gray encoding
*/
enum encoding {bus_inv, t_part, gray};

//encode an input stream
void bus_encoder(char *filename, encoding enc);

//invert the stream of bits
char * invert(char *stream);

// function implement Bus-invert, all these functions print out the encoded sequence
void bus_inv_enc(char *filename);

// function implement two-partitoned
void part_enc(char *filename);

// function implement gray
void gray_enc(char *filename);

// hamming distance
int HD (char *stream1, char *stream2);


// compute the XOR between two strings
char * XOR(char *stream1, char *stream2);

// shift right by one inserting a zero as MSB
char * shifter(char *stream);


#endif /* end of include guard: BUS_ENCODER_H */
