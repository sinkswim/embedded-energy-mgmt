/* Call: ./workload_generator distribution_type length */
#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
using namespace std;

enum distrib_type {uni_high, uni_low, normal, expon, trimod};

int main(int argc, char *argv[]){

  if(argc != 3){
    cout << "> ERROR: wrong format. \n Usage: " << argv[0] << " distribution_type length \n where distribution_type: uni_high, uni_low, normal, expon, trimod \n" << endl;
    return 0;
  }

  char filename[20];
  strcpy(filename, "wl_");
  strcat(filename, argv[1]);
  strcat(filename, ".txt");
  FILE *fp;
  fp = fopen (filename,"w");
  if (fp==NULL)
  {
    cout << "> ERROR: Couldn't open file \n" << endl;
    return 0;
  }



  char *ptr;
  long ret;
  int length = strtol(argv[2], &ptr, 10);

  distrib_type dist;
  if(!strcmp(argv[1], "uni_high"))
    dist = uni_high;
  else if(!strcmp(argv[1], "uni_low"))
    dist = uni_low;
  else if(!strcmp(argv[1], "normal"))
    dist = normal;
  else if(!strcmp(argv[1], "expon"))
    dist = expon;
  else if(!strcmp(argv[1], "trimod"))
    dist = trimod;
  else{
    cout << "Wrong distribution requested, accepted values are: uni_high, uni_low, normal, expon, trimod \n" << endl;
    return 0;
  }

  int active, idle, second;
  int first = 0;

  default_random_engine generator;
  uniform_int_distribution<int> uni_active_distribution(1,500);
  uniform_int_distribution<int> uni_high_distribution(1,100);
  uniform_int_distribution<int> uni_low_distribution(1,400);
  normal_distribution<double> norm_distribution(100.0, 20.0);
  exponential_distribution<double> exp_distribution(1/50.0);
  normal_distribution<double> norm50_distribution(50.0, 10.0);
  normal_distribution<double> norm100_distribution(100.0, 10.0);
  normal_distribution<double> norm150_distribution(150.0, 10.0);


  for(int i = 0;  i < length; i++){
    active = uni_active_distribution(generator);      // compute the active interval
    // based on the requested distribution type computer the idle interval
    switch(dist){
      case uni_high:{
        idle = uni_high_distribution(generator);
        break;
      }
      case uni_low:{
        idle = uni_low_distribution(generator);
        break;
      }
      case normal:{
      idle = (int)norm_distribution(generator) + 1;
        break;
      }
      case expon:{
        idle = (int)exp_distribution(generator) + 1;
        break;
      }
      case trimod:{
        switch(rand()%3+1){
          case 1:
            idle = (int)norm50_distribution(generator) + 1;
            break;
          case 2:
            idle = (int)norm100_distribution(generator) + 1;
            break;
          case 3:
            idle = (int)norm150_distribution(generator) + 1;
            break;
        }
        break;
      }
      // default:
      //   break;
    }

    first += active + 1;
    second = first + idle;
    fprintf(fp, "\t \t %d\t%d \n", first, second);
    first = second + 1;
  }

  fclose (fp);
  return 1;
}
