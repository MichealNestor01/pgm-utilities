// pgm struct deffinition
#include "pgmStruct.h"

// exit codes
#include "exitCodes.h"

// get the reader function
#include "pgmWriter.h"

// library for I/O routines
#include <stdio.h>

// library for memory routines
#include <stdlib.h>

// prototypes
int write_to_file(char * filename, PGM * pgm);
