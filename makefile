#
# Author: Micheal Nestor
# Date created: 08/04/22
#
# Make file for assignment one (fancy version)
#

# Flags:
#  -E 			preproces only (use .i for output)
#  -c			compile only
#  -g			debug support for gdb
#  -std=c99		use the C99 C standard
#  -fmax-errors=1	stop the compiler after one error
#  -o <FILENAME>	output file name
#  -Wall		turn all warnings on
#  -Werror		treat all warnings as errors
#  -lm 			link the math library


# Macros to add details:
# first a macro for the compiler name
CC = gcc
# now a macro for the compile flags
CCFLAGS = -g -std=c99 -fmax-errors=1 -Wall -Werror # -lm
# The set of subordinate routines and then grouped by which are dependent on which
MODULES_LIST = pgmReader.o pgmWriter.o exitMessage.o pgmCleaner.o pgmReadFromFile.o pgmWriteToFile.o converter.o checkInt.o roundNumber.o sqrtNumber.o
MODULES_ECHO = pgmWriter.o exitMessage.o pgmCleaner.o pgmReadFromFile.o pgmReader.o pgmWriteToFile.o
MODULES_COMP = pgmReader.o exitMessage.o pgmCleaner.o pgmReadFromFile.o
MODULES_CONVERTERS = ${MODULES_ECHO} converter.o
MODULES_REDUCE = pgmWriter.o exitMessage.o pgmCleaner.o pgmReadFromFile.o pgmReader.o pgmWriteToFile.o checkInt.o
MODULES_TILE = ${MODULES_REDUCE} roundNumber.o
MODULES_ASSEMBLE = ${MODULES_TILE} sqrtNumber.o
# the set of main routines 
MAINS = pgmEcho.o pgmComp.o pgma2b.o pgmb2a.o pgmReduce.o pgmTile.o pgmAssemble.o
# the set of executables
TARGETS = pgmEcho pgmComp pgma2b pgmb2a pgmReduce pgmTile pgmAssemble


# First target is the default
# we will use a dummy target to fource compilation of multiple executables
all: ${TARGETS}
# note the empty command - we don't do "anything" except check dependencies

# next, a dummy target to clean things up
# This is in case the cmpilation gets confused 
# This has no dependencies at all, and always executes
clean:
	rm ${TARGETS} ${MAINS} ${MODULES_LIST} 

# The pgmEcho executable depends on it's own file and the standard set of modules
# target	implementation
pgmEcho:	pgmEcho.o 		${MODULES_ECHO}
#	tool	compile flags	output name			files to compile
	${CC} 	${CCFLAGS}		-o pgmEcho			pgmEcho.o ${MODULES_ECHO}

# target	implementation
pgmComp:	pgmComp.o 		${MODULES_COMP}
#	tool	compile flags	output name			files to compile
	${CC} 	${CCFLAGS}		-o pgmComp			pgmComp.o ${MODULES_COMP}

# target	implementation
pgma2b:		pgma2b.o 		${MODULES_CONVERTERS}
#	tool 	compile flags	outut message		files to compile
	${CC}   ${CCFLAGS}		-o pgma2b			pgma2b.o ${MODULES_CONVERTERS}

# target	implementation
pgmb2a:		pgmb2a.o 		${MODULES_CONVERTERS}
#	tool	compile flags	output name			files to compile
	${CC}	${CCFLAGS}		-o pgmb2a	 	   	pgmb2a.o ${MODULES_CONVERTERS}	

# target	implementation
pgmReduce:	pgmReduce.o 	${MODULES_REDUCE}
#	tool	compile flags	output name			files to compile
	${CC}	${CCFLAGS}		-o pgmReduce		pgmReduce.o ${MODULES_REDUCE}	

# target	implementation
pgmTile:	pgmTile.o 		${MODULES_TILE}
#	tool	compile flags	output name			files to compile
	${CC}	${CCFLAGS}		-o pgmTile			pgmTile.o ${MODULES_TILE}	

# target	implementation
pgmAssemble:	pgmAssemble.o ${MODULES_ASSEMBLE}
#	tool	compile flags	output name			files to compile
	${CC}	${CCFLAGS}		-o pgmAssemble		pgmAssemble.o ${MODULES_ASSEMBLE}	

# Target for each object file 
# Each object file depends on its own implementation (.c)
# and any interfaces (.h) that it uses

# first the modules with the main routines
# Echo program:
# target	implementation				interfaces
pgmEcho.o: 	pgmEcho.c					pgmEcho.h 
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgmEcho.o		pgmEcho.c

# Compare program
# target	implementation				interfaces
pgmComp.o: 	pgmComp.c					pgmComp.h 
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgmComp.o		pgmComp.c

# Convert binary to ascii program
# target	implementation				interfaces
pgmb2a.o: 	pgmb2a.c					pgmb2a.h 
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgmb2a.o			pgmb2a.c

# Convert ascii to binary program
# target	implementation				interfaces
pgma2b.o: 	pgma2b.c					pgma2b.h 
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgma2b.o			pgma2b.c

# Reduce image program
# target		implementation			interfaces
pgmReduce.o: 	pgmReduce.c				pgmReduce.h 
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgmReduce.o		pgmReduce.c

# Reduce image program
# target		implementation			interfaces
pgmTile.o: 		pgmTile.c				pgmTile.h 
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgmTile.o		pgmTile.c

# Assemble image program 
# target		implementation			interfaces
pgmAssemble.o: 	pgmAssemble.c			pgmAssemble.h 
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgmAssemble.o	pgmAssemble.c

# now the module with the reader code
# target		implementation			interfaces
pgmReader.o: 	pgmReader.c				pgmReader.h 
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgmReader.o		pgmReader.c

# now with the writer code
# target		implementation			interfaces
pgmWriter.o: 	pgmWriter.c				pgmWriter.h 
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgmWriter.o		pgmWriter.c

# now with the error printer code
# target		implementation			interfaces
exitMessage.o:	exitMessage.c			exitMessage.h	
# 	tool	compile flags	output name	  		files to compile
	gcc		-c ${CCFLAGS}	-o exitMessage.o 	exitMessage.c  

# now with the pgm cleaner code
# taget			implementation			interfaces
pgmCleaner.o:	pgmCleaner.c			pgmCleaner.h
# 	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o pgmCleaner.o		pgmCleaner.c

# now with the module that writes to files
# target			implementation		interfaces
pgmWriteToFile.o: 	pgmWriteToFile.c	pgmWriteToFile.h
#	tool	compile flags	output name			files to compile
	gcc		-c ${CCFLAGS}	-o pgmWriteToFile.o	pgmWriteToFile.c

# now the module that programs use to read from files
# target			implementation		interfaces
pgmReadFromFile.o:	pgmReadFromFile.c	pgmReadFromFile.h	
# 	tool	compile flags	output name	  			files to compile
	gcc		-c ${CCFLAGS}	-o pgmReadFromFile.o 	pgmReadFromFile.c 

# now the module that converts files from a2b and b2a
# target			implementation		interfaces
converter.o:		converter.c			converter.h
#	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o converter.o		converter.c

# now the module that checks ints
# target			implementation		interfaces
checkInt.o:			checkInt.c			checkInt.h
#	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}	-o checkInt.o		checkInt.c

# now the module that rounds numbers
# target			implementation 		interfaces
roundNumber.o:		roundNumber.c		roundNumber.h
#	tool	compile flags	output name			files to compile
	gcc 	-c ${CCFLAGS}   -o roundNumber.o	roundNumber.c

# now the module that sqrts numbers
# target			implementation 		interfaces
sqrtNumber.o:		sqrtNumber.c		sqrtNumber.h
#	tool 	compile flags 	output name 		files to compile	
	gcc		-c ${CCFLAGS}	-o sqrtNumber.o		sqrtNumber.c

