/*
Converter is used by both pgmA2B and pgmB2A as they are basically the exact same function with 2 lines different
*/

// prototypes and dependencies for the file
#include "converter.h"

// as pgmA2B wll be run directly it uses a main function
int converter(int argc, char **argv, char format) {
    // verify the argument count:
    if (argc == 1) { // no arguments were given
        // print out the usage code and return without error
        printf("Usage: %s inputImage.pgm outputImage.pgm\n", argv[0]);
        return EXIT_NO_ERRORS;
    } else if (argc != 3) { // incorrect argument cunt
        // adjust return code and go to the end of the function 
        return exit_code_with_message(ERROR_ARG_COUNT, NULL);
    }

    // The struct will be what stores the pgm file's data
    PGM * pgm = (PGM *) malloc(sizeof(PGM));
    // verify that the memory was allocated correctly 
    if (pgm == NULL) // malloc failed so return with correct code and a message
        return exit_code_with_message(ERROR_MISC_MALLOC, NULL);
    
    // read the data from the input file and attempt to write that data out to the given format of pgm
    int return_code = read_and_write(argv[1], argv[2], pgm, format);

    // clean the memory allocated to pgm
    pgm_cleaner(pgm);

    if (return_code == ERROR_OUT) 
        return exit_code_with_message(return_code, argv[2]);
    else if (return_code != EXIT_NO_ERRORS)
        return exit_code_with_message(return_code, argv[1]);
    return exit_code_with_message(CONVERTED, NULL);
}

// used to make the main function smaller
int read_and_write(char * input_file, char * output_file, PGM * pgm, char format) {
    // get the data from the file into the pgm struct
    int return_code = read_from_file(input_file, pgm);
    if (return_code == EXIT_NO_ERRORS) { // there was no error reading the data
        // check that the file format does not match the format we are converting it to
        if (pgm->format == format) {
            if(format == 'R') {
                return ERROR_MAGICNUMBER;
            } else { 
                return ERROR_MAGICNUMBER;
            }  
        }
        // set the format of the pgm equal to the format we are converting to
        pgm->format = format;
        // now write this file
        return write_to_file(output_file, pgm);
    } 
    return return_code;
}
