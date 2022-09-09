// pgm struct deffinition 
#include "pgmStruct.h"

// exit codes
#include "exitCodes.h"

// get the error writer function
#include "exitMessage.h"

// get the read from file function
#include "pgmReadFromFile.h"

// get the write to file function
#include "pgmWriteToFile.h"

// get the pgm cleaner function
#include "pgmCleaner.h"

// get the check int function
#include "checkInt.h"

// library for I/O routines      
#include <stdio.h>

// library for memory routines     
#include <stdlib.h>

// library for string manipulation
#include <string.h>

// library for math functions
//#include <math.h>

// include function for rounding
#include "roundNumber.h"

int main(int argc, char **argv);
int split(PGM * pgm, int factor, char * output_file);
int check_output_format(char * string);
