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


#ifndef reader
int reader(FILE * input_file, PGM * pgm);
int read_header_data(FILE * input_file, PGM * pgm);
int read_magic_number(FILE * input_file, PGM * pgm);
int read_comment_line(FILE * input_file, PGM * pgm);
int read_dimensions_max_gray(FILE * input_file, PGM * pgm);
int read_image_data(FILE * input_file, PGM * pgm);
int read_binary_data(FILE * input_file, PGM * pgm);
int read_ASCII_data(FILE * input_file, PGM * pgm);
#endif
