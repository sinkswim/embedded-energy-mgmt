#include "bus_encoder.h"


int main(int argc, char const *argv[]) {
    
    encoding selected_enc; // enum encoding {bus_inv, t_part, gray};
    if(!strcmp(argv[2], "bus_inv"))
        selected_enc = bus_inv;
    else if(!strcmp(argv[2], "t_part"))
        selected_enc = t_part;
    else if(!strcmp(argv[2], "gray"))
        selected_enc = gray;
    else{
        printf("Wrong encoding requested, select one of: bus_inv, t_part, gray \n");
        return 1;
    }

    // printf("Generating all encoded streams\n");
    bus_encoder((char*)argv[1], selected_enc);

  return 0;
}
