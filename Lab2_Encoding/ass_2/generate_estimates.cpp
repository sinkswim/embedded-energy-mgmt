#include "estimate_energy.h"

int main(int argc, char const *argv[]) {

  // printf("Estimating energy for all 19 streams (in femtoJoules (fJ))\n");
  estimate_energy((char*)argv[1], 0, 50.0, 1.0);  // Capacitance = 50.0 fF; Voltage = 1.0V
  return 0;
}
