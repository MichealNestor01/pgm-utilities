#ifndef PGM_STRUCT_DEFINED
typedef struct _pgm {
  //format is either R for raw or P for plain  
  unsigned char format;
  //comment line 
  char * comment_line;
  //width and height
  unsigned int width, height;  
  //max gray value
  unsigned int max_gray;
  //image data
  unsigned char ** image_data;
} PGM;
#define PGM_STRUCT_DEFINED 1
#endif
