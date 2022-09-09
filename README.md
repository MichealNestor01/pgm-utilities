# PGM UTILITIES

## PROJECT DESCRIPTION

This was a project in my first year at leeds, it is a set of utilities for pgm files, entirely written in c.

## USAGE INSTRUCTIONS

# RUN 'make' TO CREATE ALL EXECUTABLES DISPLAYED FOLLOWING

# ELSE RUN 'make <executable>' TO CREATE THE EXECUTABLE YOU WANT

./pgmEcho:
This program coppies a given pgm file to another given pgm file
Usage: Usage: ./pgmEcho inputImage.pgm outputImage.pgm

./pgmComp:
This program compares two pgm files and will output wheter or not they are logically identical
Usage: ./pgmComp inputImage.pgm inputImage.pgm

./pgma2b:
This program converts a given ascii pgm file and will output an equivalent binary version of this file
Usage: ./pgma2b inputImage.pgm outputImage.pgm

./pgmb2a:
This program converts a given binary pgm file and will output an equivalent ascii version of this file
Usage: ./pgmb2a inputImage.pgm outputImage.pgm

./pgmReduce:
This program takes an input pgm file and an integer factor and will reduce the image by that factor in each directopn
Usage: ./pgmReduce inputImage.pgm reduction_factor outputImage.pgm

./pgmTile:
This program takes 3 arguments, an input pgm file, an integer factor n, and an output file name template. It
divides the input file into nxn smaller images corresponding to parts of the image)
Usage: ./pgmTile inputImage.pgm tiling*factor outputImage*<row>\_<column>.pgm

./pgmAssemble:
This program takes 3n + 3 arguments, an output image with its width and height, and then a set of tiles and their
row and column where they should be inserted into the output image.
Usage: ./pgmAssemble outputImage.pgm width height (row column inputImage.pgm)+

RUNNING THE TEST SCRIPT
I have implemented a test script which ensures that the files work correctly, to run this script and see if the
tests pass, open the main directory in a terminal, and type "./testscript.sh" and click enter this will run the
script.
