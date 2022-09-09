/*
======PGM A2B MODULE======
Author: Micheal Nestor
Date Created:  14/03/22
Email:  sc21mpn@leeds.ac.uk
Purpose: This file will take a plain pgm input file, an output filename, and then it will 
copy a raw version of the file to the output file
*/

// prototypes and dependencies for this file
#include "pgma2b.h"

// as pgma2b will be run directly it uses a main function
int main(int argc, char **argv) {
    // convert the given file
    return converter(argc, argv, 'R');
}
