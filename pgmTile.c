/*
======PGM TILE MODULE======
Author: Micheal Nestor 
Date Created: 16/03/22
Email: sc21mpn@leeds.ac.uk
Purpose: This file will take 3 arguments, an input file, an integer factor n and an output file 
teplate name, divide the input file into nn smaller images corresponding to parts of the image
*/

// function prototypes and dependencies
#include "pgmTile.h"

// As time will be run directly it uses a main function
int main(int argc, char **argv) {
    // verify the arguments
    if (argc == 1) { // no arguments given
        // print the usage code and return without error
        printf("Usage: %s inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm\n", argv[0]);
        return EXIT_NO_ERRORS;
    } else if (argc != 4) { // incorrect argument count
        // adjust return code and go to the end of the function
        return exit_code_with_message(ERROR_ARG_COUNT, NULL);
    }

    // check that arg[2] is an int
    int return_code = check_int(argv[2]);
    if (return_code != EXIT_NO_ERRORS)
        return exit_code_with_message(return_code, NULL);


    // check the format of the output string (and also remove the _<row>_<column>.pgm from the end of it)
    return_code = check_output_format(argv[3]);
    if (return_code != EXIT_NO_ERRORS) // bad output template, so return error code and output a message
        return exit_code_with_message(return_code, argv[3]);

    // the struct will be what stores the input pgm file's data
    PGM * pgm = (PGM * ) malloc(sizeof(PGM));
    // verify that the memory was allocated correctly
    if (pgm == NULL) // malloc failed so return an error code and show a message
        return exit_code_with_message(ERROR_MISC_MALLOC, NULL);
    

    // get the data from the file into the pgm struct
    return_code = read_from_file(argv[1], pgm);
    if (return_code != EXIT_NO_ERRORS) {
        pgm_cleaner(pgm);
        return exit_code_with_message(return_code, argv[1]);
    }

    return_code = split(pgm, atoi(argv[2]), argv[3]);
    pgm_cleaner(pgm);
    return exit_code_with_message(return_code, argv[3]);
}

// split will split the image into tiles
// my algorithm visualised: (spliting a 4*5 into 9)
// 0  1  2  3                   0  | 1  | 2  3      
// 4  5  6  7                   4  | 5  | 6  7
// 8  9  10 11 === tiled ===>   ===============
// 12 13 14 15                  8  | 9  | 10 11
// 16 17 18 19                  12 | 13 | 14 15
//                              ===============
//                              16 | 17 | 18 19
// It does this by looping creating pgm structs for each 
// of the 9 pixels, then working out their dimensions which
// can be calculated by seeing if they are on the far right 
// column or bottom row. 
// Then I have a variable that keeps track of whether a row
// of tiles has been allocated (end_of_row);
// A variable that keeps track of how many rows of the original
// image have been allocated (rows_allocated); 
// And an array that keeps track of how many pixles in each 
// row of the original image have been allocated.
// With these three variables I can calculate which pixels need
// to be allocated to each row. 
int split(PGM * pgm, int factor, char * output_file) {
    // width and height are used to neaten the code up a bit
    int width = pgm->width;
    int height = pgm->height;
    // check that the tile factor is valid
    if (factor > width || factor > height) {
        // return factor error 
        return ERROR_MISC_SCALEFACTOR;
    }
    // output count is the number of output files
    int output_tiles_count = factor*factor;
   
    // pgm_array contains structs for all of the pgm arrays that we are going to create
    // allocate memory to it and the arrays it points to
    PGM ** pgm_array = malloc(sizeof(PGM *) * output_tiles_count);
    if (pgm_array == NULL) // malloc failed
        return ERROR_MISC_MALLOC;
    for (int i = 0; i < output_tiles_count; i++) {
        pgm_array[i] = (PGM *) malloc(sizeof(PGM));    
    }
    
    // current tile height and width will is used to track the width of the current tile
    int current_tile_width;
    int current_tile_height;
    // extra_x is x - (tile_x * (factor - 1))
    // extra_x is the sizing of the tiles on the right most and bottom tiles.
    // tile_x is x/factor rounded to the nearest whole number
    // tile_x is also the standard x for the tiles.
    int tile_width = round_number((double) width/factor);
    int extra_width = width - (tile_width * (factor - 1));
    int tile_height = round_number((double) height/factor);
    int extra_height = height - (tile_height * (factor - 1));
    
    // visualisation
    // 4 x 5 image split into 9 tiles:
    // tile_width = 1
    // tile_height = 2 
    // extra_width = 2
    // extra_heigt = 1

    // rows assigned keeps track of how many rows have been fully placed into tiles
    int rows_assigned = 0;
    // assigned pixels is an array that tracks how many pixels have been moved from each row into tiles
    unsigned int * assigned_pixels = (unsigned int *) malloc(height * sizeof(unsigned int));
    for (int i = 0; i < height; i++) {
        assigned_pixels[i] = 0;
    }

    // initialise the data for the pgm's in the pgm_array, these are our tiles, and we will create each tile fully sequentially
    for (int tile_index = 0; tile_index < output_tiles_count; tile_index++) { // here tile index is the current tile 
        // end of row checks to see whether the end of a row has been reached
        int end_of_row = 0;
        // every tile will be match the format of the input pgm and have the same max_gray
        pgm_array[tile_index]->format = pgm->format;
        pgm_array[tile_index]->max_gray = pgm->max_gray;
        // now decide the width and height of the tiles
        // the tiles on the edges of the reduced width and height will have potentially different widths and heights
        if ((tile_index+1)%factor == 0 && tile_index >= (output_tiles_count - factor )) { // you are in the bottom right corner so dimensions are e_wid * e_hei
            current_tile_width  = extra_width;
            current_tile_height = extra_height;
        } else if (tile_index >= (output_tiles_count - factor)) { // check if you are in in the last row, if so dimensions are t_wid * e_hei
            current_tile_width = tile_width;
            current_tile_height = extra_height;
        } else if ((tile_index+1)%factor == 0)  {// check if you are in the last column, if so dimensions are e_wid * t_hei
            // you are on right hand side, so you have reached the end of a row
            end_of_row = 1;
            current_tile_width = extra_width;
            current_tile_height = tile_height;
        } else { // current tile is a standard tile
            current_tile_width = tile_width;
            current_tile_height = tile_height;
        }
        // assign the width and height of the current tile to the current tile width and height found
        pgm_array[tile_index]->width = current_tile_width;
        pgm_array[tile_index]->height = current_tile_height;
        pgm_array[tile_index]->comment_line = NULL;
    
        

        // allocate memory to the current tile
        pgm_array[tile_index]->image_data = (unsigned char **) malloc(current_tile_height * sizeof(unsigned char *));
        // sanity check the memory allocation
        if (pgm_array[tile_index]->image_data == NULL) { // malloc failed
            // Return with the relevant error code 
            return ERROR_MALLOC_IMAGE;
        }
        // assign memory to each row in the current tile
        for (int i = 0; i < current_tile_height; i++) {
            pgm_array[tile_index]->image_data[i] = (unsigned char *) malloc(pgm->width * sizeof(unsigned char *));
           // sanity check memory allocation
           if (pgm_array[tile_index]->image_data[i] == NULL) { // malloc failed
                // return with malloc failerror code
                return ERROR_MISC_MALLOC;
           }
        }

        // populate the current tile's data with values
        for (int row = 0; row < current_tile_height; row++) { // loop over the rows in the current tile
            // row index of the row we would like to write pixels from
            int row_index = rows_assigned + row;
            for (int col = 0; col < current_tile_width; col++) { // loop over the columns in each row of the tile
                // now assign the tile's current pixel to the next unwritten pixel in the current row
                // row_index * width + assigned_pixels[row_index] gives you the index of the pixel in the original image that maps to the current tile's pixel
                pgm_array[tile_index]->image_data[row][col] = pgm->image_data[row_index][assigned_pixels[row_index]];
                // increase the count of pixels that have been written in the current row
                assigned_pixels[row_index]++;
            }
        }
            
        // if end of row then increment rows_assigned
        if (end_of_row == 1) 
            rows_assigned += tile_height;
        
        // WRITE THE TILE TO A FILE
        
        // row and col give you the coordinates of the tile you are currently writing
        int row = (tile_index)/factor;
        int col = tile_index - (row * factor);
        // work out the length of the output filename in characters
        int output_length = strlen(output_file) + (row/10 + 1) + (height/10 + 1) + 7; //the 7 chars are the two _'s .pgm and \0
        // buffer stores the output flename
        char * buffer = (char *) malloc(sizeof(char) * output_length);
        // join the _<row>_<column>.pgm to the end of the file name        
        snprintf(buffer, output_length, "%s_%d_%d.pgm", output_file, row, col);
        // write the current tile to a file
        int return_code = write_to_file(buffer, pgm_array[tile_index]);  
        if (return_code != EXIT_NO_ERRORS) {
            strcpy(output_file, buffer);    
            return return_code;
        }
        // clean up memory
        pgm_cleaner(pgm_array[tile_index]);
        free(buffer);
    }
    // free memory
    free(assigned_pixels);
    free(pgm_array);
    // return without error   
    return TILED;
}

// function to check the output string format and remove _<row>_<column>.pgm from the end of the string this is because it is no longer needed
int check_output_format(char * string) {//Checks that the output is in the form outputImage_<row>_<column>.pgm
    for (int c_index = 0; c_index <= ((int) strlen(string)) - 19; c_index++) { // loop over each char in the string up to end - 19 because the template is 19 long
        int current_char = (int) string[c_index]; // cast the current char to an int to get the ascii index   
        if (current_char == 95) {
            //printf("STRING: %s\n", &string[c]);
            if(strcmp(&string[c_index], "_<row>_<column>.pgm") == 0) {//Check if the end of the string matches
                // end the string here by inserting a null pointer
                string[c_index] = '\0';
                return EXIT_NO_ERRORS;
            }
        }
    }
    // return filename error filename if an _ is never found
    return ERROR_MISC_TILE_TEMPLATE;
}
