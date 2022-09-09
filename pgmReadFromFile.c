// this is code that was being coppied and pasted in multple files
// so I have put it in a separate function so that it can be reused

// functon prototypes and dependencies
#include "pgmReadFromFile.h"

// this function attempts to read the data from a given pgm file and paste this into a given pgm struct.
int read_from_file(char * filename, PGM * pgm) {
    // verify the given file name exists and create file pointer to that file
    FILE * input_file = fopen(filename, "r");                 
    if (input_file == NULL) { // fopen failed
    	// adjust the return code to reflect the bad file name
    	return ERROR_FILENAME;
    }

    int return_code = EXIT_NO_ERRORS;
    // get the data from the file into the the pgm struct
    return_code = reader(input_file, pgm);
    // close the file
    fclose(input_file);
    // return the reader's return code
    return return_code;
}
