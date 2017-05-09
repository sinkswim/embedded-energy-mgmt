#ifndef ESTIMATE_ENERGY_H
#define ESTIMATE_ENERGY_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_BITS 256
// #define DEBUG_MODE

/*
* Function:   estimate_energy
* Parameters: pointer to input file (filename); stream length (length);
*             capacitance (C); voltage (v)
* Behavior:   compute energy consumption and print it out by mean of printf
* Return:     true if computation is correct, false otherwise
* NOTE:       since voltage is 1 V, the magnitude of energy is the same of
*             capacitance, that is fempto! Thus energy will be in fJ
*/
bool estimate_energy(char *filename, int length, double C, double v);


/*
* Function:   HD
* Parameters: strings containing the two stream of bits
* Behavior:   compute the Hamming distance between two stream
* Return:     the Hamming distance of the two stream, -1 in case of Error
              that is stream of different lengths!
*/
int HD (char *stream1, char *stream2);






#endif /* end of include guard: ESTIMATE_ENERGY_H  */
