/*
======PGM ECHO MODULE======
Author: Micheal Nestor
Date Created: 05/03/22
Email: sc21mpn@leeds.ac.uk
Purpose: This file will take a pgm input file, and an output file name, and then it will copy the data from the input to the output
*/

// prototypes and dependencies for this file
#include "pgmEcho.h"

// as echo will be run directly it uses a main function
int main(int argc, char **argv) {
    // verify the argument count:
    if (argc == 1) { // no arguments given
	    // print usage code and return without error
	    printf("Usage: %s inputImage.pgm outputImage.pgm\n", argv[0]);
	    return EXIT_NO_ERRORS;
    }  else if (argc != 3) { // incorrect argument count 
	    // adjust return code and go to the end of the function
	    return exit_code_with_message(ERROR_ARG_COUNT, NULL);
    }

    // the struct will be what stores the pgm file's data
    PGM * pgm = (PGM * ) malloc(sizeof(PGM));
    // verify that the memory was allocated correctly
    if (pgm == NULL) // malloc failed so return malloc error code and message
	    return exit_code_with_message(ERROR_MISC_MALLOC, NULL);

    // get the data from the file into the the pgm struct
    int return_code = read_from_file(argv[1], pgm);
    if (return_code == EXIT_NO_ERRORS) // there was no error reading data, so now try and copy the data to the output file
    	return_code = write_to_file(argv[2], pgm);
               
    // clean memory allocated to pgm:
    pgm_cleaner(pgm);
    
    //return correct code with a message:
    if (return_code == ERROR_OUT) 
        return exit_code_with_message(return_code, argv[2]);
    // if the return code is exit no erros then it should be changed to echoed
    return_code = return_code == EXIT_NO_ERRORS ? ECHOED : return_code; 
    return exit_code_with_message(return_code, argv[1]);
}
