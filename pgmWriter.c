/*
======PGM WRITER MODULE======
Author: Micheal Nestor
Date Created: 05/03/22
Email: sc21mpn@leeds.ac.uk
Purpose: This file will take a pgm struct and convert this data to a pgm file
*/

// prototypes and dependencies for this file
#include "pgmWriter.h"

// writer will take a pointer to a pgm struct, and write this data to the given file
int writer(FILE * output_file, PGM * pgm) {
    // return code is used to return any error codes at the end
    int return_code = EXIT_NO_ERRORS;

    // write the header data to the output file:
    return_code =  write_header(output_file, pgm);
    if (return_code != EXIT_NO_ERRORS) 
	    return return_code;
    
    // total_pixels will be used when writing the data to the file
    long int total_pixels = pgm->width * pgm->height;
    if (pgm->format == 'P') { // write data according to the PLAIN specification
     	return_code = write_plain_image(output_file, pgm, total_pixels);    
    } else { //write data according to the RAW pgm specification
        return_code = write_raw_image(output_file, pgm,total_pixels);
    }

    //return the return code
    return return_code;
}

// write header writes the header part of the file
int write_header(FILE * output_file, PGM * pgm) {
    // bytes_written is used to track wehterh the fprints work
    int bytes_written = -1;
    if (pgm->format == 'P') { // write the magic number as P2 as this is a plain file
        bytes_written = fprintf(output_file, "P2\n%d %d\n%d\n", pgm->width, pgm->height, pgm->max_gray);
    } else { // write the magic number as P5 as this is a raw file we can ignore the comment line, as we dont care about writing that 
        bytes_written = fprintf(output_file, "P5\n%d %d\n%d\n", pgm->width, pgm->height, pgm->max_gray); 
    }
         
    // verify that the header was written correctly
    if (bytes_written < 0) {
        // Return output file error code 
        return ERROR_OUT;
    } 
 
    // return without error 
    return EXIT_NO_ERRORS;
}

// write plain image, writes the image data to the file in the form of ascii characters
int write_plain_image(FILE * output_file, PGM * pgm, long int total_pixels) {
    // bytes_written is used to track whether the fprints work 
    size_t bytes_written = -1;
    // write to the file
    for (int r_index = 0; r_index < pgm->height; r_index++) { // loop over each row in image data
        for (int p_index = 0; p_index < pgm->width; p_index++) { // loop over each point in the row
            // put the current pixel into the file, if we are at the end of the row add a line break instead of a space
            bytes_written =  fprintf(output_file, "%d%c", pgm->image_data[r_index][p_index], (pgm->width - p_index > 1 ? ' ' : '\n'));
            // sanity check bytes written 
            if (bytes_written < 0) {
                // return there is an issue with the output
                return ERROR_OUT;
            }
        }
    }
    //return without error
    return EXIT_NO_ERRORS;   
}

// write raw image, writes the image data to the file in the form of binary bytes
int write_raw_image(FILE * output_file, PGM * pgm, long int total_pixels) {
    // bytes_written is used to track whether the fwrite works
    size_t bytes_written = -1;
    
    // write to file
    for (int r_index = 0; r_index < pgm->height; r_index++) { // loop over each row in image data
        // write the current row to the file
        bytes_written = fwrite(pgm->image_data[r_index], 1, pgm->width, output_file);
        // sanity check the data being written
        if (bytes_written < 0) { //data write failed
            // return error with output
            return ERROR_OUT;                   
        }
    }
    //return wihout error 
    return EXIT_NO_ERRORS;
}
