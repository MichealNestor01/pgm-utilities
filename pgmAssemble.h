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

// library for math
//#include <math.h>

// functions to round and sqrt
#include "roundNumber.h"
#include "sqrtNumber.h"

int main(int argc, char **argv);
int assembler(char **arguments, int arg_length);
//int check_int(char * string);
