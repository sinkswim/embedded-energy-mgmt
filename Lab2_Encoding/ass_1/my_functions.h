/* Functions prototypes */
#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include "my_functions.h"
#include <math.h>

/*************************************************************/
/* Given an integer as input, it returns the equivalent in   */
/* binary                                                    */
/*************************************************************/
unsigned long long int int_to_int(unsigned long long int value);

/*************************************************************/
/* Given an integer as input, and the number of bits, it     */
/* returns the equivalent representation in binary.          */
/* NOTE The representation is unsigned!! not 2's complement  */
/*************************************************************/
char * int_to_binary(unsigned long long int value, unsigned n);

/***************************************************************************/
/* Sequence generator                                                      */
/* l = lenght of the sequence. n = number of bits. d = 1 data. a = 1       */
/* address. q = degree of sequentiality. m = number of random LSBs         */
/* filename = where to save the generated sequence                         */
/***************************************************************************/
void gen_stream(int l, int n, int d, int a, int m, double q, char *filename);


#endif /* end of include guard: MY_FUNCTIONS_H  */
