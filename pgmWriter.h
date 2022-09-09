// pgm struct deffinition 
#include "pgmStruct.h"

// exit codes
#include "exitCodes.h"

// global constants
#include "pgmConstants.h"

// library for I/O routines      
#include <stdio.h>

// library for memory routines     
#include <stdlib.h>


#ifndef writer
int writer(FILE * output_file, PGM * pgm);
int write_header(FILE * output_file, PGM * pgm);
int write_plain_image(FILE * output_file, PGM * pgm, long int total_pixel);
int write_raw_image(FILE * output_file, PGM * pgm, long int total_pixels);
#endif
