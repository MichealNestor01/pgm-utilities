/*
======PGM COMP MODULE======
Author: Micheal Nestor
Date Created: 06/03/22
Email: sc21mpn@leeds.ac.uk
Purpose: This file will take two pgm files as attributes, compare them, and decide if they are logically equivalent
*/

// prototypes and dependencies for this file
#include "pgmComp.h"

int main(int argc, char **argv) {
    // verify the argumen count
    if (argc == 1) { // no arguments given
	    // print usage code and return without error
	    printf("Usage: %s inputImage.pgm inputImage.pgm\n", argv[0]);
        // return without error
        return EXIT_NO_ERRORS;
    } else if (argc != 3) { // incorrect argument count
        // return the correct error code, and print the correct error message
        return exit_code_with_message(ERROR_ARG_COUNT, NULL);
    }
    
    // allocate memory to the structs that will store data about both files
    PGM * pgm_a = (PGM *) malloc(sizeof(PGM));
    PGM * pgm_b = (PGM *) malloc(sizeof(PGM));
    // verify memory was allocated correctly
    if (pgm_a == NULL || pgm_b == NULL) // malloc failed so exit program
        return exit_code_with_message(ERROR_MISC_MALLOC, NULL);

    // now try and read data to these pgm's
    int return_code = read_from_file(argv[1], pgm_a);
    if (return_code != EXIT_NO_ERRORS) { // There was an error reading data from the first file
        pgm_cleaner(pgm_a);
        return exit_code_with_message(return_code, argv[1]);
    }
    return_code = read_from_file(argv[2], pgm_b);
    if (return_code != EXIT_NO_ERRORS) { // There was an error reading data from the second file
        pgm_cleaner(pgm_a);
        pgm_cleaner(pgm_b);
        return exit_code_with_message(return_code, argv[2]);
    }

    // now compare the two files to determine if they are logically equivalent   
    return_code = compare_pgms(pgm_a, pgm_b) == 1 ? IDENTICAL : DIFFERENT;    
    
    // clean the pgms
    pgm_cleaner(pgm_a);
    pgm_cleaner(pgm_b);
    
    // return the correct code and print the correct message   
    return exit_code_with_message(return_code, NULL);
}

// this function will return 1 if the two pgms are logically equivalent 0 else
int compare_pgms(PGM * pgm_a, PGM * pgm_b) {
    // compare the width and height
    if (pgm_a->width != pgm_b->width || pgm_a->height != pgm_b->width)
        return 0;        
    // compare the pixel values in each pgm    
    for (int r_index = 0; r_index < pgm_a->height; r_index ++) { // loop over the rows in the pgms 
        for (int p_index = 0; p_index < pgm_a->width; p_index ++) { // loop over the pixels in each row
            if (pgm_a->image_data[r_index][p_index] != pgm_b->image_data[r_index][p_index]) { // corresponding pixels are not identical
                return 0;
            }
        }
    }
    // return identical
    return 1;
}
