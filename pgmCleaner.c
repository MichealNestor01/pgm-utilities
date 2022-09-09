// include the header file
#include "pgmCleaner.h"

// This function cleans up the memory of the pgm struct it is passed  
void pgm_cleaner(PGM * pgm) {
    // Cleanup memory
    if (pgm->comment_line != NULL) {
        free(pgm->comment_line);
    }
    if (pgm->image_data != NULL) {
        // loop over and free rows workng backwards
        for (int r_index = pgm->height - 1; r_index >= 0; r_index--) {
            if (pgm->image_data[r_index] != NULL) {
                free(pgm->image_data[r_index]);
            }
        }
        free(pgm->image_data);
    } 
    if (pgm != NULL) {
        free(pgm);
    }
}
