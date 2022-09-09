// pgm struct definition 
#include "pgmStruct.h"

// exit codes
#include "exitCodes.h"

// get the error printer function
#include "exitMessage.h"

// get the read from file function
#include "pgmReadFromFile.h"

// get the write to file function
#include "pgmWriteToFile.h"

// get the pgm cleaner function
#include "pgmCleaner.h"

// library for io routines 
#include <stdio.h>

// library for memory routines
#include <stdlib.h>

int converter(int argc, char **argv, char format);
int read_and_write(char * input_file, char * output_file, PGM * pgm, char format);
