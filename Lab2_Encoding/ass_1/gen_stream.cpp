#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "my_functions.h"
#include <math.h>


int main(int argc, char const *argv[]) {

    /* generate 9 data streams */
    gen_stream(1000, 8, 1, 0, 2, 0.0, "./ass_1/traces/data/data_stream_8_2.txt");       // 20% random
    gen_stream(1000, 8, 1, 0, 4, 0.0, "./ass_1/traces/data/data_stream_8_4.txt");       // 50%
    gen_stream(1000, 8, 1, 0, 6, 0.0, "./ass_1/traces/data/data_stream_8_6.txt");       // 70%
    gen_stream(1000, 8, 1, 0, 8, 0.0, "./ass_1/traces/data/data_stream_8_8.txt");       // 100%
    gen_stream(1000, 16, 1, 0, 3, 0.0, "./ass_1/traces/data/data_stream_16_3.txt");     // 20%
    gen_stream(1000, 16, 1, 0, 8, 0.0, "./ass_1/traces/data/data_stream_16_8.txt");     // 50%
    gen_stream(1000, 16, 1, 0, 11, 0.0, "./ass_1/traces/data/data_stream_16_11.txt");   // 70%
    gen_stream(1000, 16, 1, 0, 16, 0.0, "./ass_1/traces/data/data_stream_16_16.txt");    // 100%
    gen_stream(1000, 13, 1, 0, 7, 0.0, "./ass_1/traces/data/data_stream_odd.txt");

    /*generate 10 address streams */
    gen_stream(1000, 8, 0, 1, 0, 0.0, "./ass_1/traces/addr/add_stream_8_q0.txt");
    gen_stream(1000, 8, 0, 1, 0, 1.0, "./ass_1/traces/addr/add_stream_8_q1.txt");
    gen_stream(1000, 8, 0, 1, 0, 0.05, "./ass_1/traces/addr/add_stream_8_q005.txt");
    gen_stream(1000, 8, 0, 1, 0, 0.2, "./ass_1/traces/addr/add_stream_8_q02.txt");
    gen_stream(1000, 8, 0, 1, 0, 0.6, "./ass_1/traces/addr/add_stream_8_q06.txt");
    gen_stream(1000, 16, 0, 1, 0, 0.0, "./ass_1/traces/addr/add_stream_16_q0.txt");
    gen_stream(1000, 16, 0, 1, 0, 1.0, "./ass_1/traces/addr/add_stream_16_q1.txt");
    gen_stream(1000, 16, 0, 1, 0, 0.05, "./ass_1/traces/addr/add_stream_16_q005.txt");
    gen_stream(1000, 16, 0, 1, 0, 0.2, "./ass_1/traces/addr/add_stream_16_q02.txt");
    gen_stream(1000, 16, 0, 1, 0, 0.6, "./ass_1/traces/addr/add_stream_16_q06.txt");

  return 0;
}
