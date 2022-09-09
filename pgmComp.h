// pgm struct deffinition 
#include "pgmStruct.h"

// exit codes
#include "exitCodes.h"

// global constants
#include "pgmConstants.h"

// get the read from file function
#include "pgmReadFromFile.h"

// get the pgm memory cleaner function
#include "pgmCleaner.h"

// get the error printer function
#include "exitMessage.h"

// library for I/O routines      
#include <stdio.h>

// library for memory routines     
#include <stdlib.h>

int main(int argc, char **argv);
int compare_pgms(PGM * pgm_a, PGM * pgm_b);
int end_of_function(char * bad_file, PGM * pgm_a, PGM * pgm_b, int return_code);
//int read_pgm_file(char * filename, PGM * pointer);
//void pgm_cleaner(PGM * pgm);

