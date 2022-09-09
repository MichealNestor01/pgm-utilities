/*
======PGM REDUCE MODULE======
Author: Micheal Nestor
Date Created: 15/03/22
Email: sc21mpn@leeds.ac.uk
Purpose: This file will take a pgm input file, an output file name and a reduction factor. It will then 
store the input file reduced by the factor in the output file
*/

// prototypes and dependencies for this file
#include "pgmReduce.h"

// as reduce will be run directly it uses a main function
int main(int argc, char **argv) {
    
    // verify the arguments:
    if (argc == 1) { // no arguments given
	    // print usage code and return without error
	    printf("Usage: %s inputImage.pgm reduction_factor outputImage.pgm\n", argv[0]);
	    return EXIT_NO_ERRORS;
    }  else if (argc != 4) { // incorrect argument count 
	    // adjust return code and go to the end of the function
	    return exit_code_with_message(ERROR_ARG_COUNT, NULL);
    }  

    // check that arg[2] is an int
    int return_code = check_int(argv[2]); 
    if (return_code != EXIT_NO_ERRORS)
        return exit_code_with_message(return_code, NULL);

    // the struct will be what stores the pgm file's data
    PGM * pgm = (PGM * ) malloc(sizeof(PGM));
    // verify that the memory was allocated correctly
    if (pgm == NULL) // malloc failed so return with correct errro code and a message
        return exit_code_with_message(ERROR_MISC_MALLOC, NULL);

    // get the data from the file into the pgm struct
    return_code = read_from_file(argv[1], pgm);
    if (return_code == EXIT_NO_ERRORS) {// there was no error reading data, so now try and copy the data to the output file
        return_code = reduce(pgm, atoi(argv[2]));
        if (return_code == EXIT_NO_ERRORS) 
            return_code = write_to_file(argv[3], pgm);    
    }
 
    // clean up memory
    pgm_cleaner(pgm);

    if (return_code == ERROR_OUT)
        return exit_code_with_message(return_code, argv[3]);
    else if (return_code != EXIT_NO_ERRORS)
        return exit_code_with_message(return_code, argv[1]);
    return exit_code_with_message(REDUCED, NULL);
}

//reduce actually reduces the pgm file by the given factor
int reduce(PGM * pgm, int factor) {
    if (factor <= 0 || factor > pgm->width || factor > pgm->height) { //factor must be > 0 and cant be more then a dimensions of the image
        return ERROR_MISC_SCALEFACTOR;    
    }
    
    // old hight is used when reading data.
    int old_height = pgm->height;
    // adjust the width and height by the given factor
    if (pgm->width % factor != 0) {
        pgm->width = (pgm->width + factor) / factor;
    } else {
        pgm->width = pgm->width / factor;
    }
    if (pgm->height % factor != 0) {
        pgm->height = (pgm->height + factor) / factor;
    } else {
        pgm->height = pgm->height / factor;    
    }
    
    // what I am going to do to reduce the image is to overwrite the existing 2d array:
    // Take this image data array and reduce it by factor 3 this would be the output:
    // (0)  1  2  (3)                    0  3  2  3                      0  3  2 3
    // 4    5  6  7      ===reduced===>  12 15 5  6   ===after free===>  12 15 5 6
    // 8    9  10 11                     8  9  10 11
    // (12) 13 14 (15)                   12 13 14 15
    // as the width and height have been adjusted the data on the end of row 1 and 2 will
    // be ignored and the data on rows 3 and 4 will be ignored however that means I will need
    // to free those rows here

    for (int r_index = 0; r_index < pgm->height; r_index++) { // loop over each point in the new reduced array
        // current row in the original is r_index * height
        for (int p_index = 0; p_index < pgm->width; p_index++) { // loop over each point in the new reduced row
            // overwrite the current value in the reduced raster with the corresponding value in the original.
            pgm->image_data[r_index][p_index] = pgm->image_data[r_index *  factor][p_index *  factor];
        }
    }

    // free the rows in image data that are no longer in use
    for (int r_index = old_height; r_index >= pgm->height; r_index--){
        free(pgm->image_data[r_index]);
    }
    // return without error
    return EXIT_NO_ERRORS;
}

