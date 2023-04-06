/* Inspired by BASIC program by Michael A. Covington N4TMI as
   published in 73 Magazine, September 1990

   For input 0.58 1.77, output should be

     Inductance (uH): 448.09
     Distrib Capacitance (pF): 18.04
     Resonant Frequencies with this coil:
      C (pF)   F (MHz)
         2     5.3164836
         4     3.7593216
         8     2.6582418
        16     1.8796608
        32     1.3291209
        64     0.9398304
       128     0.6645605
       256     0.4699152
       512     0.3322802

   This assumes CT (C1 in schematic) has value 150 pF.  Change
   value of CT below to match your known capacitor value.

   Compile on Linux with "gcc -o compute_L compute_L.c -lm"
   Run via "compute_L F_with_cap [F_without_cap]" with
   frequencies in MHz and F_without_cap optional.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

void usage(void)
{
  fprintf(stderr, "usage: calc [options] freq_w_cap [freq_wo_cap]\n");
  fprintf(stderr, "  Freqeuncies in MHz\n");
  fprintf(stderr, "Options\n");
  fprintf(stderr, "  -c cap_value (pF)\n");
  fprintf(stderr, "  -f compute F rather than L\n");
  fprintf(stderr, "  -r Print table of resonant frequencies\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
  double PI = 3.14159;
  double LS = 0.0; /* Stray inductance in H if known */
  double CS = 0.0; /* Stray capacitance in F if known */
  double CT = 100e-12; /* capacitor value, Covington used 150 pF */
  double F2 = 0.58e+6; /* Frequency with capacitor in Hz */
  double F1 = 1.77e+6; /* Frequency without capacitor in Hz */
  double CD, L, C, F;
  int i, opt, print_res = 0;
  int compute_f = 0;

  while ((opt = getopt(argc, argv, "c:frx:")) != -1) {
    switch (opt) {
    case 'c':
      CT = atof(optarg) * 1.0e-12;
      break;
    case 'r':
      print_res = 1;
      break;
    case 'f':
      compute_f = 1;
      break;
    default:
      usage();
      break;
    }
  }

  if (compute_f) {
    /* Next argument is L in uH, all we do is compute F */
    if (argc - optind != 1) usage();
    L = atof(argv[optind]) * 1.0e+6;
    printf("F = %0.3lf MHz\n", 1.0/(2*PI*sqrt(L*CT)));
    return EXIT_SUCCESS;
  }

  if (argc - optind == 1) {
    F2 = atof(argv[optind]) * 1.0e+6;
    F1 = 0.0;
  } else if (argc - optind == 2) {
    F2 = atof(argv[optind]) * 1.0e+6;
    F1 = atof(argv[optind + 1]) * 1.0e+6;
  } else {
    usage();
  }
  
  if (F1 == 0) {
    CD = 0.0;
  } else {
    CD = CT/(pow(F1/F2, 2.0) - 1.0);
  }

  L = 1.0/(pow(2.0*PI*F2, 2.0) * (CD+CT)) - LS;

  printf("Inductance (uH): %0.2lf\n", L*1.0e+6);
  if (CD != 0.0)
    printf("Distrib Capacitance (pF): %0.2lf\n", (CD - CS) * 1.0e+12);

  if (print_res) {
    printf("Resonant Frequencies with this coil:\n");
    printf(" C (pF)   F (MHz)\n");
    for (i = 1; i < 10; i++) {
      C = 1.0e-12 * pow(2, i);
      F = 1.0/(2.0*PI*sqrt(L*C));
      printf("%5.0lf     %0.7lf\n", C*1.0e+12, F*1.0e-6);
    }
  }
  
  return EXIT_SUCCESS;
}
  
