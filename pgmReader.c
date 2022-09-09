/*
======PGM READER MODULE======
Author: Micheal Nestor
Date Created: 02/03/22
Email: sc21mpn@leeds.ac.uk
Purpose: This file will verify and read the data from a pgm file and return a pointer to a struct containing this data
*/

// prototypes and dependencies for this file
#include "pgmReader.h"

// reader will take the given file, and pgmStruct and will populate the struct with the files data 
int reader(FILE * input_file, PGM * pgm) {
    // sanity check input_file
    if (input_file == NULL) {
        // return with bad filename error code
        return ERROR_FILENAME;
    }

    // default return code is exit_no_errors
    int return_code = EXIT_NO_ERRORS;

    // Read the pgm's header data
    return_code = read_header_data(input_file, pgm);
    if (return_code != EXIT_NO_ERRORS) {
        goto function_end;
    }

    // Read the pgm's image data
    return_code = read_image_data(input_file, pgm);
    
    //debug output
    //printf("Debug output:\nFormat")

    // Function End (where clean up occurs)
    function_end:
        return return_code;
}

//===Read header data functions===//

// read header data reads the header data of a pgm image file
int read_header_data(FILE * input_file, PGM * pgm) {
    // default return code is exit_no_errors
    int return_code = EXIT_NO_ERRORS;
        
    // check for a comment
    return_code = read_comment_line(input_file, pgm);
    if (return_code != EXIT_NO_ERRORS) 
        goto function_end;

    // Validate and Read the magic number
    return_code = read_magic_number(input_file, pgm);
    if (return_code != EXIT_NO_ERRORS) 
        goto function_end;
	
    // scan white space if present
    fscanf(input_file, " ");

    // check for a comment
    return_code = read_comment_line(input_file, pgm);
    if (return_code != EXIT_NO_ERRORS)
        goto function_end;

    // validate and read the pgm dimensions and max gray
    return_code = read_dimensions_max_gray(input_file, pgm);
    if (return_code != EXIT_NO_ERRORS) 
        goto function_end;
	
    // check for a comment
    return_code = read_comment_line(input_file, pgm);
    
    // function end where return code is dealt with
    function_end:
        return return_code;
}

// read magic number expects a file pointer the start of the pgm file and a pgm struct with adiquate memory added to it
int read_magic_number(FILE * input_file, PGM * pgm) {
    // Raw (binary): P5 (2 chars) 
    // Plain (ASCII): P2 (2 chars) 
    unsigned char magic_number[2] = {'0', '0'}; 
    
    // Read in magic numbers 
    magic_number[0] = getc(input_file);
    magic_number[1] = getc(input_file);
    
    // Check that it is followed by a white space.
    unsigned char buffer = getc(input_file);
    // validate buffer is white space
    if (
        buffer != ' '  &&
        buffer != '\n' &&
        buffer != '\t' &&
        buffer != '\v' &&
        buffer != '\f' &&
        buffer != '\r' 
    ) {
        // no space was found after the magic number, therefore return bad magic number error
        return ERROR_MAGICNUMBER;
    }

    // Verify magic number is one of the two allowed values
    if (magic_number[0] == 'P' && magic_number[1] == MAGIC_PLAIN_IDENTIFIER) { // magic number is P2
        pgm->format = 'P';
    } else if (magic_number[0] == 'P' && magic_number[1] == MAGIC_RAW_IDENTIFIER) { // magic number is P5
        pgm->format = 'R';
    } else { // magic number is not P5 or P2
        // Return with the relevant error code
        return ERROR_MAGICNUMBER;
    }

    return EXIT_NO_ERRORS;
}

// read comment line expects a file pointer pointing to the second line in a pgm file and a pgm struct with adiquate memory added to it
int read_comment_line(FILE * input_file, PGM * pgm) {
    // set the pgm's comment_line to null
    pgm->comment_line = NULL;

    // if the file is raw check for a comment line
    char next_char = fgetc(input_file);
    if (next_char ==  '#') {
        // check that there isnt already a comment line, if so we will overwrite it
        if (pgm->comment_line != NULL) 
            free(pgm->comment_line);
        // allocate memory for the comment line
        pgm->comment_line = (char *) malloc(MAX_COMMENT_LINE_LENGTH);
        if (pgm->comment_line == NULL) { //malloc failed
            // free memory
            free(pgm->comment_line);
            // close the file
            fclose(input_file);
            // Return with appropriate exit code
            return ERROR_MISC;
        }

        fgets(pgm->comment_line, MAX_COMMENT_LINE_LENGTH, input_file);
        // valid line is set to 1 if there is a comment of valid length
        int valid_line = 0;
        for (int char_index = 0; char_index < MAX_COMMENT_LINE_LENGTH; char_index++) {
            if (pgm->comment_line[char_index] == '\n') {
                valid_line = 1;
            }
        }
        if (valid_line == 0) {
            return ERROR_COMMENT;
        }

    } else { // no comment line found
        // put the charracter back
        ungetc(next_char, input_file);
    }
    return EXIT_NO_ERRORS;
}

// read magic number expects a pointer pointing either the second line in a 
// pgm or the 3rd line in the case where there is a comment 
// and a pgm struct with adiquate memory added to it
int read_dimensions_max_gray(FILE * input_file, PGM * pgm) {
    // Verify in width, height
    int scan_count = fscanf(input_file, " %u %u", &(pgm->width), &(pgm->height));
    if ( 
        (scan_count != 2                  ) ||
        (pgm->width      <  MIN_IMAGE_DIMENSION) ||
        (pgm->width      >  MAX_IMAGE_DIMENSION) ||
        (pgm->height     <  MIN_IMAGE_DIMENSION) ||
        (pgm->height     >  MAX_IMAGE_DIMENSION) 
    ) { // Failed verification
        // Return with relevant error code 
        return ERROR_DIMENSIONS;
    }
    
    // the comment line needs to checked for as comment line can apear anywhere in the header
    int return_code = read_comment_line(input_file, pgm);
    if (return_code != EXIT_NO_ERRORS) 
            return return_code;
        
    // verify max gray value
    scan_count = fscanf(input_file, " %u", &(pgm->max_gray));
    if ( 
        (scan_count != 1                  ) ||
        (pgm->max_gray  !=  PGM_GRAY_VALUE) 
    ) { // Failed verification

        //CHECK MAX GRAY
        //printf("bad gray\n");

        // Return with relevant error code 
        return ERROR_MAX_GRAY;
    } 
    //printf("good gray\n");

    //printf("gray value: %u\n", pgm->max_gray);

    return EXIT_NO_ERRORS;
}

//===IMAGE DATA READING FUNCTIONS===//

// this attempts to read the image data from a pgm file
int read_image_data(FILE * input_file, PGM * pgm) {
    int return_code = EXIT_NO_ERRORS;
    // allocate memory to the pgm struct's image data
    pgm->image_data = NULL;
    // allocate unsigned char * size of height to pgm->image_data
    // allocate to each of them unsigned char size of width to each in pgm->image_data
    pgm->image_data = (unsigned char **) malloc(pgm->height * sizeof(unsigned char *));
    // sanity check the memory allocation
    if (pgm->image_data == NULL) { // malloc failed
        // Return with relevant error code
        return ERROR_MALLOC_IMAGE;
    }
    for (int i = 0; i < pgm->height; i++) {
        pgm->image_data[i] = (unsigned char *) malloc(pgm->width * sizeof(unsigned char *));
        // sanity check memory allocation
        if (pgm->image_data[i] == NULL) {//malloc failed
            // return with malloc fail error code
            return ERROR_MALLOC_IMAGE;
        }
    }
    
    // read the image data   
    if (pgm->format == 'P') { // the file contains ascii image data
        return_code = read_ASCII_data(input_file, pgm);
    } else { // the file contains binary image data
        return_code = read_binary_data(input_file, pgm);
    }
    if (return_code != EXIT_NO_ERRORS) { // there was an error reading the image data 
        return return_code;
    } 
     
    // now check to see whether all of the data has been read from the file (it should have been)
    unsigned char * excess_data = (unsigned char *) malloc(1);
    if (excess_data == NULL) { // malloc failed
	    // return miscilaneous error code
	    return ERROR_MISC_MALLOC;	
    } 
    // attempt to read data from the file 
    // read any white space 
    int scan_count = fscanf(input_file, " ");
    scan_count = fread(excess_data, 1, 1, input_file);
    // clean memory
    free(excess_data);
    if (scan_count > 0) { // something has been scanned, there fore there is too much data in the file
	    // return with teh bad image data error code
	    return ERROR_DATA;
    }
  
    // return the no errors exit code	
    return EXIT_NO_ERRORS;
}

// reads image data encoded as binary in a pgm file
int read_binary_data(FILE * input_file, PGM * pgm) {
    // read the line break
    getc(input_file);
    // read the binary data into each row of pgm->image_data
    for (int r_index = 0; r_index < pgm->height; r_index++) {
        // read the binary data
        int scan_count = fread(pgm->image_data[r_index], 1, pgm->width, input_file);
        //printf("Bytes read: %d\n", scan_count);
        // verify the image data
        if (scan_count != pgm->width) { // check the correct  number of bits was read
            // return with the error data code
            return ERROR_DATA;            
        } else {
            for (int p_index = 0; p_index < pgm->width; p_index++) { // check that each pixel has a valid value
                if (
                    pgm->image_data[r_index][p_index] > pgm->max_gray  || 
                    pgm->image_data[r_index][p_index] < 0
                ) { 
                    // return with data error code
                    return ERROR_DATA;
                }
            }
        }
    }
    return EXIT_NO_ERRORS;
}

// reads image data encoded as ascii in a pgm file
int read_ASCII_data(FILE * input_file, PGM * pgm) {
    // itterate over the rows in pgm->image_data
    for (int r_index = 0; r_index < pgm->height; r_index++) {
        // itterate over the pixels in each row
        for (int p_index = 0; p_index < pgm->width; p_index++) {
            // read the current pixel into the address pointed to by pgm->image_data[r_index][p_index]
            unsigned int data_value;
            int scan_count = fscanf(input_file, " %d", &data_value);
            pgm->image_data[r_index][p_index] = (unsigned char) data_value;
            // Verify the gray value
            if (
                scan_count != 1 || 
                pgm->image_data[r_index][p_index] > pgm->max_gray ||
                pgm->image_data[r_index][p_index] < 0
            ) {
                // return with the image data error code
                return ERROR_DATA;
            }
        }
    }
    return EXIT_NO_ERRORS;
}
