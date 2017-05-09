/* Functions implementations */

#include "my_functions.h"

/*************************************************************/
/* Convert int in binary.The result is again an interger     */
/*************************************************************/
unsigned long long int int_to_int(unsigned long long int value) {
    if (value == 0) return 0;
    if (value == 1) return 1;                       /* optional */
    return (value % 2) + 10 * int_to_int(value / 2);
}

/*************************************************************/
/* Convert the binary integer in a string                    */
/*************************************************************/
char * int_to_binary(unsigned long long int value, unsigned n) {

  unsigned long long int tmp;
  int original_size;
  char *bit_stream;
  char *dummy_stream;

  bit_stream = (char *) calloc(n, sizeof(char));
  tmp = int_to_int(value);
  sprintf(bit_stream, "%llu", tmp);

  // correcting the leding 0s
  if(strlen(bit_stream) < n) {
    original_size = strlen(bit_stream);
    dummy_stream = (char *) calloc(n-original_size, sizeof(char));
    for (int i = 0; i < n-original_size; i++) {
      dummy_stream[i] = '0';
    }
      strcat(dummy_stream, bit_stream);
      return dummy_stream;
    }
    else
      return bit_stream;
}

/*****************************************************************************/
/* Generates the stream of data or addresses according to the specifications */
/*****************************************************************************/
void gen_stream(int l, int n, int d, int a, int m, double q, char *filename) {

  char *bit_stream;
  srand(time(NULL));  // init seed
  FILE *fp;
  fp = fopen (filename,"w");


  if (d == 0 && a == 1) {
    // Address stream generator
    bit_stream = (char *) calloc(n, sizeof(char));
    if (q == 0.0) {
      // random numbers
      unsigned long long int tmp;
      for (unsigned long int i = 0; i < l; i++) {
          tmp = rand()%((int) pow(2, n)-1); // max number with n bits is 2^n -1
          bit_stream = int_to_binary(tmp, n);

          //print on the target file
          for (int i = 0; i < n; i++) {
            fprintf(fp,"%c", bit_stream[i]);
          }
          fprintf(fp,"\n");
      }

    } else if (q < 1.0 && q > 0.0) {
        /* generate a random number once each q*100 samples */
        unsigned int randomizer = 0;
        unsigned long long int tmp = 0;
        for (unsigned long int i = 1; i <= l; i++) {
            if (randomizer ==  q*100) {
              // generate a random number
              tmp = rand()%((int) pow(2, n)-1); // max number with n bits is 2^n -1
              bit_stream = int_to_binary(tmp, n);
              randomizer = 1;
            } else {
              randomizer++;
              if (i != 1)
                tmp++;
              bit_stream = int_to_binary(tmp, n);
            }
            //print on the target file
            for (int i = 0; i < n; i++) {
              fprintf(fp,"%c", bit_stream[i]);
            }
            fprintf(fp,"\n");
        }

    } else if (q == 1.0) {
      /* perfect sequence */
      for (unsigned long int i = 0; i < l; i++) {
          bit_stream = int_to_binary(i, n);

          //print on the target file
          for (int i = 0; i < n; i++) {
            fprintf(fp,"%c", bit_stream[i]);
          }
          fprintf(fp,"\n");
      }

    } else {
        printf("ERROR!!\n");
        return;
    }

  } else if(d == 1 && a == 0) {
    // Data stream generator

    bit_stream = (char *) calloc(n, sizeof(char));
    for (int j = 0; j < l; j++) {
      // set the MSBs at 0
      for (int i = 0; i < n-m; i++) {
        bit_stream[i] = '0';
      }

      int tmp = 0;
      // set LSBs random
      for (int i = n-m; i < n; i++) {
        tmp = rand()%2;
        if (tmp == 0)
          bit_stream[i] = '0';
        else
          bit_stream[i] = '1';

      }

      // print on the target file
      for (int i = 0; i < n; i++) {
        fprintf(fp,"%c", bit_stream[i]);
      }
      fprintf(fp,"\n");
    }
  }

  // close the file
  fclose (fp);

}
