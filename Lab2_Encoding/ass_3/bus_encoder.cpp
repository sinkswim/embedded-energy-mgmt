#include "bus_encoder.h"

// #define DEBUG_MODE

//bus encoder implementation
void bus_encoder(char *filename, encoding enc)
{


  switch (enc) {
    case bus_inv:
      bus_inv_enc(filename);
      break;

    case t_part:
      part_enc(filename);
      break;

    case gray:
      gray_enc(filename);
      break;

    default:
      printf("Error wrong enc value\n");
      return;
      break;
  }


}

// invert the content of a String
char * invert(char *stream)
{
  // local variables
  char *tmp;
  int size_stream;


  //function body
  size_stream = strlen(stream);
  tmp = (char *) calloc(size_stream, sizeof(char));

  for (int i = 0; i < size_stream; i++) {
    if (stream[i] == '1')
      tmp[i] = '0';
    else if(stream[i] == '0')
      tmp[i] = '1';
  }

  return tmp;

}

//bus invert encoder
void bus_inv_enc(char *filename)
{
  // local variables
  FILE *source;


  // function body
  source = fopen (filename,"r");
  if (source == NULL) {
    printf("Error, %s doesn't exitst \n", filename);
    return;
  }

  char *tmp1;
  char *tmp2;
  char *tmp3;       // copy of the last value that has been sent over the bus
  char flag = 0;    // flag to signal the first iteration



  //allocate the max lenght of the string, i.e. 256 bits
  tmp1 = (char *) calloc(MAX_BITS, sizeof(char));
  tmp2 = (char *) calloc(MAX_BITS, sizeof(char));
  tmp3 = (char *) calloc(MAX_BITS, sizeof(char));

  // try to open the file
  source = fopen (filename,"r");
  while(fscanf(source,"%s", tmp1) != EOF) {
    // if the flag = 0 -> first iteration over the file
    // pseudo algorithm
    /*
      1st iteration:
        sent tmp1 as it is
        then HD(tmp1, tmp2)
        decide whether to invert tmp2 and store in tmp3
        send tmp3 = tmp2 , note tmp3 do not contain the additional bus line -> do it through printf
      2nd iteraion:
        read tmp1
        compare with tmp3(past value sent)
        decide whether to invert tmp1 and store in tmp3
        send tmp3 = tmp1 , note tmp3 do not contain the additional bus line -> do it through printf
    */
    if (!flag) {
      // read anothre line, and raise error if it is EOF!
      if(fscanf(source,"%s", tmp2) == EOF) {
        printf("Error, at least two lines are needed!\n");
        return;
      }
      flag = 1;
      printf("%s%s\n", tmp1, "0");
      if((unsigned long) HD(tmp1, tmp2) <= (unsigned) (strlen(tmp1)/2)) {
        // not invert
        printf("%s%s\n", tmp2, "0");
        strcpy(tmp3, tmp2);
      } else {
        //invert
        //           printf("YES INVERT\n");
        printf("%s%s\n", invert(tmp2), "1");
        strcpy(tmp3, invert(tmp2));
      }
    } else {
      if((unsigned long) HD(tmp1, tmp3) <= (unsigned) (strlen(tmp1)/2)) {
        // not invert
        // printf("NO INV: %d\n", HD(tmp1, tmp3));
        printf("%s%s\n", tmp1, "0");
        strcpy(tmp3, tmp1);
      } else {
        //  invert
        // printf("YES INVERT\n");
        //printf("YES INV: %d\n", HD(tmp1, tmp3));
        printf("%s%s\n", invert(tmp1), "1");
        strcpy(tmp3, invert(tmp1));
      }
    }
  }
}

// gray encoder
void gray_enc(char *filename){

  // local variables
  FILE *source;


  // function body
  source = fopen (filename,"r");
  if (source == NULL) {
    printf("Error, %s doesn't exitst \n", filename);
    return;
  }

  char *tmp1;
  char *tmp2;
  char *tmp3;       // copy of the last value that has been sent over the bus



  //allocate the max lenght of the string, i.e. 256 bits
  tmp1 = (char *) calloc(MAX_BITS, sizeof(char));
  tmp2 = (char *) calloc(MAX_BITS, sizeof(char));
  tmp3 = (char *) calloc(MAX_BITS, sizeof(char));

  // try to open the file
  source = fopen (filename,"r");
  while(fscanf(source,"%s", tmp1) != EOF) {
    // gray encoding algorithm
    /*
      given word A, perform A EXOR (A>>1) to obtain gray(A)
    */
    /*
    tmp2 = tmp1 >> 1;
    tmp3 = tmp2 exor tmp2;
    printf("%s\n", tmp3);
    */
    printf("%s\n", XOR(tmp1, shifter(tmp1)));
    }
}

// partitioned Bus-invert
void part_enc(char *filename)
{
  // local variables
  FILE *source;


  // function body
  source = fopen (filename,"r");
  if (source == NULL) {
    printf("Error, %s doesn't exitst \n", filename);
    return;
  }

  char *tmp1;
  char *tmp2;
  char *tmp3;       // copy of the last value that has been sent over the bus
  char *tmpA1;  // left side of tmp1
  char *tmpB1;  // right side of tmp1
  char *tmpA2;  // left side of tmp2
  char *tmpB2;  // right side of tmp2
  char *tmpA3;
  char *tmpB3;
  char flagA, flagB;  // tell whether one or both parts have been inverted
  int len, len1, len2;
  char flag = 0;    // flag to signal the first iteration



  //allocate the max lenght of the string, i.e. 256 bits
  tmp1 = (char *) calloc(MAX_BITS, sizeof(char));
  tmp2 = (char *) calloc(MAX_BITS, sizeof(char));
  tmp3 = (char *) calloc(MAX_BITS, sizeof(char));

  // try to open the file
  source = fopen (filename,"r");
  while(fscanf(source,"%s", tmp1) != EOF) {
    // if the flag = 0 -> first iteration over the file
    // pseudo algorithm
    /*
      1st iteration:
        sent tmp1 as it is
        then HD(tmp1, tmp2)
        decide whether to invert tmp2 and store in tmp3
        send tmp3 = tmp2 , note tmp3 do not contain the additional bus line -> do it through printf
      2nd iteraion:
        read tmp1
        compare with tmp3(past value sent)
        decide whether to invert tmp1 and store in tmp3
        send tmp3 = tmp1 , note tmp3 do not contain the additional bus line -> do it through printf
    */
    if (!flag) {
      // read anothre line, and raise error if it is EOF!
      if(fscanf(source,"%s", tmp2) == EOF) {
        printf("Error, at least two lines are needed!\n");
        return;
      }
      flag = 1;
      printf("%s%s%s\n", tmp1, "0", "0");
      // split the string just read
      //split tmp1
      len = strlen(tmp1);
      len1 = len/2;
      len2 = len - len1; // Compensate for possible odd length
      tmpA1 = (char*)malloc(len1+1); // one for the null terminator
      memcpy(tmpA1, tmp1, len1);
      tmpA1[len1] = '\0';
      tmpB1 = (char*)malloc(len2+1); // one for the null terminator
      memcpy(tmpB1, tmp1+len1, len2);
      tmpB1[len2] = '\0';
      //split tmp2
      len = strlen(tmp2);
      len1 = len/2;
      len2 = len - len1; // Compensate for possible odd length
      tmpA2 = (char*)malloc(len1+1); // one for the null terminator
      memcpy(tmpA2, tmp2, len1);
      tmpA2[len1] = '\0';
      tmpB2 = (char*)malloc(len2+1); // one for the null terminator
      memcpy(tmpB2, tmp2+len1, len2);
      tmpB2[len2] = '\0';

      //allocate tmpA3 and tmpB3
      tmpA3 = (char *) calloc(strlen(tmpA1), sizeof(char));
      tmpB3 = (char *) calloc(strlen(tmpB1), sizeof(char));

      // check left side
      if((unsigned long) HD(tmpA1, tmpA2) <= (unsigned) (strlen(tmpA1)/2)) {
        // not invert
        flagA = '0';
        tmpA3 = tmpA2;
      } else {
        //invert
        flagA = '1';
        tmpA3 = invert(tmpA2);
      }

      // check right side

      if((unsigned long) HD(tmpB1, tmpB2) <= (unsigned) (strlen(tmpB1)/2)) {
        // not invert
        flagB = '0';
        tmpB3 = tmpB2;
      } else {
        //invert
        flagB = '1';
        tmpB3 = invert(tmpB2);
      }

      printf("%s%s%c%c\n", tmpA3, tmpB3, flagA, flagB);


    } else {

      // split the string just read
      //split tmp1
      len = strlen(tmp1);
      len1 = len/2;
      len2 = len - len1; // Compensate for possible odd length
      tmpA1 = (char*)malloc(len1+1); // one for the null terminator
      memcpy(tmpA1, tmp1, len1);
      tmpA1[len1] = '\0';
      tmpB1 = (char*)malloc(len2+1); // one for the null terminator
      memcpy(tmpB1, tmp1+len1, len2);
      tmpB1[len2] = '\0';

      // left side
      if((unsigned long) HD(tmpA1, tmpA3) <= (unsigned) (strlen(tmpA1)/2)) {
        // not invert
        flagA = '0';
        tmpA3 = tmpA1;
      } else {
        //  invert
        flagA = '1';
        tmpA3 = invert(tmpA1);
      }

      //right side
      if((unsigned long) HD(tmpB1, tmpB3) <= (unsigned) (strlen(tmpB1)/2)) {
        // not invert
        flagB = '0';
        tmpB3 = tmpB1;
      } else {
        //  invert
        flagB = '1';
        tmpB3 = invert(tmpB1);
      }

      printf("%s%s%c%c\n", tmpA3, tmpB3, flagA, flagB);

    }
  }
}


// XOR between two strings
char * XOR(char *stream1, char *stream2)
{

  char *tmp;

  if ((unsigned)strlen(stream1) != (unsigned)strlen(stream2))
    return NULL;

  tmp = (char *) calloc((unsigned)strlen(stream1), sizeof(char));

  for (unsigned long i = 0; i < (unsigned)strlen(stream1); i++) {
    if (stream1[i] != stream2[i])
      tmp[i] = '1';
    else
      tmp[i] = '0';
  }

  return tmp;
}

// perform the shift right by one inserting a zero as MSB
char * shifter(char *stream)
{
  char *tmp;

  tmp = (char *) calloc((unsigned)strlen(stream), sizeof(char));

  strcpy(tmp, stream);

  for (unsigned long i = 0; i < (unsigned)strlen(stream) -1 ; i++) {
    tmp[i+1] = stream[i];
  }

  tmp[0] = '0';

  return tmp;


}

// computes hamming distance
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
