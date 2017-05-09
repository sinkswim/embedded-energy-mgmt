#include "estimate_energy.h"

// esitimate the energy of a given stream of data or addresses
bool estimate_energy(char *filename, int length, double C, double v)
{

  // variables declarations
  unsigned long sum_HD; // sum of Hamming Distances
  FILE *fp;
  char *tmp1;
  char *tmp2;
  char flag = 0;    // flag to signal the first iteration



  //allocate the max lenght of the string, i.e. 256 bits
  tmp1 = (char *) calloc(MAX_BITS, sizeof(char));
  tmp2 = (char *) calloc(MAX_BITS, sizeof(char));

  // try to open the file
  fp = fopen (filename,"r");
  if (fp!=NULL)
  {
    while(fscanf(fp,"%s", tmp1) != EOF) {
      // if the flag = 0 -> first iteration over the file
      if (!flag) {
        // read anothre line, and raise error if it is EOF!
        if(fscanf(fp,"%s", tmp2) == EOF) {
          printf("Error, at least two lines are needed!\n");
          return false;
        }
        flag = 1;
        sum_HD = (unsigned long) HD(tmp1, tmp2);
      } else {
        sum_HD += (unsigned long) HD(tmp1, tmp2);
        // before leaving, copy the content of tmp1 in tmp2
        strcpy(tmp2, tmp1);
      }
    }

    // The entire file has been read
    // Compute the energy consumption
    // printf the result
    printf("Energy consumption is: %f fJ \n", (double)sum_HD*C*v);

    #ifdef DEBUG_MODE
    printf("Tot hamming distance %lu \n", sum_HD);
    #endif // DEBUG_MODE

    fclose (fp);
  }
  else {
    printf("Error, cannot open the file %s\n", filename);
    return false;
  }

  return true;
}


// Hamming Distance
int HD (char *stream1, char *stream2)
{
  // local variables declarations
  int distance = 0;

  // body of the Function
  if ((unsigned)strlen(stream1) != (unsigned)strlen(stream2))
    return -1;

  for (unsigned long i = 0; i < (unsigned)strlen(stream1); i++) {
    if (stream1[i] != stream2[i])
      distance++;
  }

  #ifdef DEBUG_MODE
  printf("distance of %s %s is  %d \n",  stream1, stream2, distance);
  #endif // DEBUG_MODE

  return distance;

}
