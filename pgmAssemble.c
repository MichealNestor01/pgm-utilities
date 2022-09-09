/*
======PGM ASSEMBLE MODULE======
Author: Micheal Nestor
Date Created: 18/03/22 (5am lol)
Email: sc21mpn@leeds.ac.uk
Purpose: This file will take pgm tiles and assemble them back into a single image
*/

// prototypes and dependencies for this file
#include "pgmAssemble.h"

// as assemble will be ran directly it uses a main function
int main(int argc, char **argv) {
    // return code will be the int returned at the end by main, it is used to track errors
    int return_code = EXIT_NO_ERRORS;

    // verify the arguments
    if (argc == 1) { // no arguments given
        // print the usage message and return without error
        printf("Usage: %s outputImage.pgm width height (row column inputImage.pgm)+\n", argv[0]);
        return EXIT_NO_ERRORS;
    } else if ((argc-1) % 3 != 0) { // check there is the correct number of arguments: 1 + 3n + 3
        // adjust return code and go to end of function 
        return exit_code_with_message(ERROR_ARG_COUNT, NULL);
    }

    // check that argv[2] and argv[3] are ints
    return_code = check_int(argv[2]);
    if (return_code != EXIT_NO_ERRORS)
        return exit_code_with_message(return_code, NULL);
    return_code = check_int(argv[3]);
    if (return_code != EXIT_NO_ERRORS)
        return exit_code_with_message(return_code, NULL);
    
    // run assembler function to assemble the image
    return_code = assembler(argv, argc);

    // return with correct code and message
    return exit_code_with_message(return_code, NULL);       
}

// this function actually assembles the given tiles
int assembler(char **arguments, int arg_length) {
    int return_code = EXIT_NO_ERRORS;
    
    // get the number of args that are describing input files
    int total_args = arg_length-4;

    // total tiles is the total number of tiles that have been given
    int total_tiles = total_args/3;
    
    // get master pgm widht and height from the aguments
    int width = atoi(arguments[2]);
    int height = atoi(arguments[3]);

    // Create a pgm for struct for the assembled image hensforth the master_image
    PGM * master_image = (PGM *) malloc(sizeof(PGM));
    // check malloc worked
    if (master_image == NULL) // if it didn't return an error
        return ERROR_MISC_MALLOC;
    // initialise the master pgm's values
    master_image->format = 'P';
    master_image->height = height;
    master_image->width = width;
    master_image->max_gray = 255;
    // allocate memory for the image data 
    master_image->image_data = (unsigned char **) malloc(height * sizeof(unsigned char *));
    // check malloc worked
    if (master_image->image_data == NULL) // if it didn't return an error
        return ERROR_MALLOC_IMAGE;
    // allocate memory to the master images image data
    for (int row_index = 0; row_index < height; row_index++) {
        // allocate memory to each row in the master pgm image
        master_image->image_data[row_index] =  (unsigned char *) malloc(width * sizeof(unsigned char));
        // check if malloc failed
        if (master_image->image_data == NULL) // if it did return an error
            return ERROR_MALLOC_IMAGE; 
        // populate the pgm with black pixels
        for (int col_index = 0; col_index < width; col_index++) {
            master_image->image_data[row_index][col_index] = (unsigned char) 0;
        }
    }

    // loop over all of the arguments given and add the corresponding tile to the master image 
    for (int argument_index = 0; argument_index < total_tiles; argument_index ++) {
        // get data about the current tile fron the given arguments
        char * current_filename = arguments[3 * (argument_index + 1) + 3];
        int row = atoi(arguments[3 * (argument_index + 1) + 1]);
        int col = atoi(arguments[3 * (argument_index + 1) + 2]);
        // create a temp pgm for the current tile
        PGM * temp_pgm = (PGM *) malloc(sizeof(PGM));
        // if malloc fails return an error
        if (temp_pgm == NULL) 
            return ERROR_MISC_MALLOC;
        // now read the data from the given file
        return_code = read_from_file(current_filename, temp_pgm);
        if (return_code != EXIT_NO_ERRORS)
            return return_code;
        // check if this pgm will fit
        if (temp_pgm->width + col > width || temp_pgm->height + row > height) {
            return ERROR_MISC_ASSEMBLE_BOUNDS; // Gonna need to make a new error for this
        }
        // dump this images pixels into the master pgm by copying them to the correct place
        for (int row_index = 0; row_index < temp_pgm->height; row_index++) {
            for (int col_index = 0; col_index < temp_pgm->width; col_index++) {
                master_image->image_data[row_index + row][col_index + col] = temp_pgm->image_data[row_index][col_index];
            }
        }
        // clean up the temp pgm's data
        pgm_cleaner(temp_pgm);
    }
    // write the master_image to a file
    return_code = write_to_file(arguments[1], master_image);
    // check for a write error
    if (return_code != EXIT_NO_ERRORS) 
        return return_code;
    
        
    // clean up the master pgm's memory 
    pgm_cleaner(master_image);  
    
    // return with the assembled return code 
    return ASSEMBLED;
}
