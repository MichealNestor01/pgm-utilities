#/!bin/bash
#======TEST SCRIPT======
#Author:   Micheal Nestor
#Date created: 16/04/2022
#Purpose: To test my pgm modules 

# FUNCTIONS
function print_heading() {
    printf "\n======$1======\n\n" 
}

function silent_clean() {
    make clean &> /dev/null
    exit 0
}

# global variables
run_message="MICHEAL'S TEST SCRIPT"
make_message="MAKING PROGRAMS AND OBJECT FILES"
testing_message="TESTING MODULES"
make_clean_message="REMOVING OBJECT FILES AND PROGRAMS"
# modules list
modules=(./pgmEcho ./pgmComp ./pgma2b ./pgmb2a ./pgmReduce)
# variables used to track the expected output from tests

# error codes
code_no_error=0
code_arg_count=1
code_filename=2
code_magicnum=3
code_comment=4
code_dimensions=5
code_max_gray=6
code_malloc_image=7
code_image_data=8
code_output_file=9
code_layout=10
code_misc=100
# usage messages
usage_echo="Usage: ./pgmEcho inputImage.pgm outputImage.pgm"
usage_comp="Usage: ./pgmComp inputImage.pgm inputImage.pgm"
usage_a2b="Usage: ./pgma2b inputImage.pgm outputImage.pgm"
usage_b2a="Usage: ./pgmb2a inputImage.pgm outputImage.pgm"
usage_reduce="Usage: ./pgmReduce inputImage.pgm reduction_factor outputImage.pgm"
usage_messages=("$usage_echo" "$usage_comp" "$usage_a2b" "$usage_b2a" "$usage_reduce")
error_arg_count="ERROR: Bad Argument Count"
error_bad_filename="ERROR: Bad File Name"
error_bad_magicnumber="ERROR: Bad Magic Number"
error_bad_scalefactor="ERROR: Miscellaneous Bad Scale Factor, must be an int > 0, and should not exceed the width or height of the input image"
error_bad_dimensions="ERROR: Bad Dimensions"
error_bad_image_data="ERROR: Bad Data"
expected_arg_count=(2 2 2 2 3)

print_heading "$run_message"

# make the files 
print_heading "$make_message"
printf "running make...\n"
make > /dev/null 
# check for make error
if [ $? != 0 ]
then
    # if there is a error with the make file delete any files created and close the script 
    printf "MAKE ERROR ENCOUNTERED: CLOSING TEST SCRIPT\n"
    silent_clean
fi

# now run all the unit tests 
print_heading "$testing_message"


# STANDARDISED TESTS

printf "=STANDARDISED TESTS=\n\n"
module_index=0
for module in "${modules[@]}"; do
    printf "TESTING $module:\n"
    # check 0 arguments:
    output=$($module)
    if [ $? == $code_no_error ] && [ "$output" == "${usage_messages[$module_index]}" ] 
    then
        printf "  0 ARGUMENTS:\t\tPASS\n"
    else
        printf "  0 ARGUMENTS:\t\tFAILED\n"
    fi

    # check 1 argument error message
    output=$($module arg1)
    #echo $?
    #echo $? == $code_arg_count
    #echo ""
    if [ $? == $code_arg_count ] && [ "$output" == "$error_arg_count" ]
    then
        printf "  1 ARGUMENT:\t\tPASS\n"
    else
        printf "  1 ARGUMENT:\t\tFAILED\n"
    fi

    # Tests for files that expect two arguments
    if [ $[expected_arg_count[$module_index]] == 2 ]
    then
        # test if there is the correct error message when presented with more than two arguments
        output=$($module arg1 arg2 arg3)
        if [ $? == $code_arg_count ] && [ "$output" == "$error_arg_count" ] 
        then
            printf "  MULTI ARGUMENT:\tPASS\n"
        else
            printf "  MULTI ARGUMENT:\tFAILED\n"
        fi

        # test for an input file that doesnt exist 
        output=$($module arg1 ./pgmTestFiles/GA.pgm)
        code=$?
        message=""$error_bad_filename" (arg1)"
        if [ $code == $code_filename ] && [ "$output" == "$message" ]
        then
            printf "  BAD INPUT FILENAME:\tPASS\n"
        else
            printf "  BAD INPUT FILENAME:\tFAILED\n"
        fi

        # test with various bad pgm's given

        # test pgm with dimensions too large
        output=$($module ./pgmTestFiles/BAHighWidth.pgm ./pgmTestFiles/tmp.pgm)
        code=$?
        output_2=$($module ./pgmTestFiles/BAHighHeight.pgm ./pgmTestFiles/tmp.pgm)
        code_2=$?
        message=""$error_bad_dimensions" (./pgmTestFiles/BAHighWidth.pgm)"
        message_2=""$error_bad_dimensions" (./pgmTestFiles/BAHighHeight.pgm)"
        if [ $code == $code_dimensions ] && [ $code_2 == $code_dimensions ] && [ "$output" == "$message" ] && [ "$output_2" == "$message_2" ]
        then
            printf "  BAD DIMENSION (65536):PASS\n"
        else
            printf "  BAD DIMENSION (65536):FAILED\n"
        fi

        # test pgm with dimensions too small
        output=$($module ./pgmTestFiles/BALowWidth.pgm ./pgmTestFiles/tmp.pgm)
        code=$?
        output_2=$($module ./pgmTestFiles/BALowHeight.pgm ./pgmTestFiles/tmp.pgm)
        code_2=$?
        message=""$error_bad_dimensions" (./pgmTestFiles/BALowWidth.pgm)"
        message_2=""$error_bad_dimensions" (./pgmTestFiles/BALowHeight.pgm)"
        if [ $code == $code_dimensions ] && [ $code_2 == $code_dimensions ] && [ "$output" == "$message" ] && [ "$output_2" == "$message_2" ]
        then
            printf "  BAD DIMENSION (-1):\tPASS\n"
        else
            printf "  BAD DIMENSION (-1):\tFAILED\n"
        fi

        # test bad magic numbers ascii
        output=$($module ./pgmTestFiles/BAMagicNumber.pgm ./pgmTestFiles/tmp.pgm)
        code=$?
        message=""$error_bad_magicnumber" (./pgmTestFiles/BAMagicNumber.pgm)"
        if [ $code == $code_magicnum ] && [ "$output" == "$message" ]
        then
            printf "  MAGIC NUMBER ASCII:\tPASS\n"
        else
            printf "  MAGIC NUMBER ASCII:\tFAILED\n"
        fi

        # test bad magic numbers binary
        output=$($module ./pgmTestFiles/BBMagicNumber.pgm ./pgmTestFiles/tmp.pgm)
        code=$?
        message=""$error_bad_magicnumber" (./pgmTestFiles/BBMagicNumber.pgm)"
        if [ $code == $code_magicnum ] && [ "$output" == "$message" ]
        then
            printf "  MAGIC NUMBER BINARY:\tPASS\n"
        else
            printf "  MAGIC NUMBER BINARY:\tFAILED\n"
        fi
        
        # test bad image raster
        output=$($module ./pgmTestFiles/BARasterLength.pgm ./pgmTestFiles/tmp.pgm)
        code=$?
        message=""$error_bad_image_data" (./pgmTestFiles/BARasterLength.pgm)"
        if [ $code == $code_image_data ] && [ "$output" == "$message" ]
        then
            printf "  IMAGE RASTER EXTRA:\tPASS\n"
        else
            printf "  IMAGE RASTER EXTRA:\tFAILED\n"
        fi

        # test bad image raster 2
        output=$($module ./pgmTestFiles/BARasterLength_short.pgm ./pgmTestFiles/tmp.pgm)
        code=$?
        message=""$error_bad_image_data" (./pgmTestFiles/BARasterLength_short.pgm)"
        if [ $code == $code_image_data ] && [ "$output" == "$message" ]
        then
            printf "  IMAGE RASTER SHORT:\tPASS\n"
        else
            printf "  IMAGE RASTER SHORT:\tFAILED\n"
        fi

        
    # Tests for funcs that expect three arguments
    elif [ $[expected_arg_count[$module_index]] == 3 ]
    then
        # test if there is the correct error message when presented with two arguments
        output=$($module arg1 arg2)
        if [ $? == $code_arg_count ] && [ "$output" == "$error_arg_count" ] 
        then
            printf "  2 ARGUMENTS:\t\tPASS\n"
        else
            printf "  2 ARGUMENTS:\t\tFAILED\n"
        fi
        # test if there is the correct error message when presented with more than three arguments
        output=$($module arg1 arg2 arg3 arg4)
        if [ $? == $code_arg_count ] && [ "$output" == "$error_arg_count" ] 
        then
            printf "  MULTI ARGUMENT:\tPASS\n"
        else
            printf "  MULTI ARGUMENT:\tFAILED\n"
        fi
        
        # test for an input file that doesnt exist 
        output=$($module arg1 2 ./pgmTestFiles/GA.pgm)
        code=$?
        message=""$error_bad_filename" (arg1)"
        if [ $code == $code_filename ] && [ "$output" == "$message" ]
        then
            printf "  BAD INPUT FILENAME:\tPASS\n"
        else
            printf "  BAD INPUT FILENAME:\tFAILED\n"
        fi

        # test pgm with dimensions too large
        output=$($module ./pgmTestFiles/BAHighWidth.pgm 2 ./pgmTestFiles/tmp.pgm)
        code=$?
        output_2=$($module ./pgmTestFiles/BAHighHeight.pgm 2 ./pgmTestFiles/tmp.pgm)
        code_2=$?
        message=""$error_bad_dimensions" (./pgmTestFiles/BAHighWidth.pgm)"
        message_2=""$error_bad_dimensions" (./pgmTestFiles/BAHighHeight.pgm)"
        if [ $code == $code_dimensions ] && [ $code_2 == $code_dimensions ] && [ "$output" == "$message" ] && [ "$output_2" == "$message_2" ]
        then
            printf "  BAD DIMENSION (65536):PASS\n"
        else
            printf "  BAD DIMENSION (65536):FAILED\n"
        fi
        
        # test bad magic numbers ascii
        output=$($module ./pgmTestFiles/BAMagicNumber.pgm 2 ./pgmTestFiles/tmp.pgm)
        code=$?
        message=""$error_bad_magicnumber" (./pgmTestFiles/BAMagicNumber.pgm)"
        if [ $code == $code_magicnum ] && [ "$output" == "$message" ]
        then
            printf "  MAGIC NUMBER ASCII:\tPASS\n"
        else
            printf "  MAGIC NUMBER ASCII:\tFAILED\n"
        fi

        # test bad magic numbers binary
        output=$($module ./pgmTestFiles/BBMagicNumber.pgm 2 ./pgmTestFiles/tmp.pgm)
        code=$?
        message=""$error_bad_magicnumber" (./pgmTestFiles/BBMagicNumber.pgm)"
        if [ $code == $code_magicnum ] && [ "$output" == "$message" ]
        then
            printf "  MAGIC NUMBER BINARY:\tPASS\n"
        else
            printf "  MAGIC NUMBER BINARY:\tFAILED\n"
        fi
        
        # test bad image raster
        output=$($module ./pgmTestFiles/BARasterLength.pgm 2 ./pgmTestFiles/tmp.pgm)
        code=$?
        message=""$error_bad_image_data" (./pgmTestFiles/BARasterLength.pgm)"
        if [ $code == $code_image_data ] && [ "$output" == "$message" ]
        then
            printf "  IMAGE RASTER EXTRA:\tPASS\n"
        else
            printf "  IMAGE RASTER EXTRA:\tFAILED\n"
        fi

        # test bad image raster 2
        output=$($module ./pgmTestFiles/BARasterLength_short.pgm 2 ./pgmTestFiles/tmp.pgm)
        code=$?
        message=""$error_bad_image_data" (./pgmTestFiles/BARasterLength_short.pgm)"
        if [ $code == $code_image_data ] && [ "$output" == "$message" ]
        then
            printf "  IMAGE RASTER SHORT:\tPASS\n"
        else
            printf "  IMAGE RASTER SHORT:\tFAILED\n"
        fi

        # reduce specific tests:
        if [ "$module" == "./pgmReduce" ]
        then
            #Check for two small reduce factor
            output=$($module ./pgmTestFiles/GA.pgm 0 ./pgmTestFiles/tmp.pgm)
            if [ $? == $code_misc ] && [ "$output" = "$error_bad_scalefactor" ]
            then
                printf "  SMALL SCALEFACTOR:\tPASS\n"
            else    
                printf "  SMALL SCALEFACTOR:\tFAILED\n"
            fi
            #Check too large reduce factor - uses GA.pgm which is a 2x2 image
            output=$($module ./pgmTestFiles/GA.pgm 3 ./pgmTestFiles/tmp.pgm)
            if [ $? == $code_misc ] && [ "$output" = "$error_bad_scalefactor" ]
            then
                printf "  LARGE SCALEFACTOR:\tPASS\n"
            else    
                printf "  LARGE SCALEFACTOR:\tFAILED\n"
            fi

        fi
    fi
    # increment module index
    module_index=$((module_index + 1))
    echo ""
done

#NOW RUN INDIVIDUAL FUNCTION TESTS FIRST TEST PGM COMP AS WE WILL USE PGM COMP TO TEST THE FILE OUTPUT 
printf "=SPECIFIED TESTS= (These rely on pgm comp working, so ensure that has passed all tests)\n\n"
printf "Testing ./pgmEcho:\n"
output=$(./pgmEcho ./pgmTestFiles/GA.pgm ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GA.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  ECHO ASCII:\t\tPASS\n"
else
    printf "  ECHO ASCII:\t\tFAILED\n"
fi

output=$(./pgmEcho ./pgmTestFiles/GB.pgm ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GB.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  ECHO BINARY:\t\tPASS\n"
else
    printf "  ECHO BINARY:\t\tFAILED\n"
fi

echo""

printf "Testing ./pgma2b:\n"
output=$(./pgma2b ./pgmTestFiles/GA.pgm ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GA.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  CONVERT ASCII:\tPASS\n"
else
    printf "  CONVERT ASCII:\tFAILED\n"
fi

echo ""

printf "Testing ./pgmb2a:\n"
output=$(./pgmb2a ./pgmTestFiles/GB.pgm ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GB.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  CONVERT BINARY:\tPASS\n"
else
    printf "  CONVERT BINARY:\tFAILED\n"
fi

echo ""

printf "Testing ./pgmb2a and ./pgma2b:\n";
output=$(./pgmb2a ./pgmTestFiles/GB.pgm ./pgmTestFiles/tmp.pgm)
output=$(./pgma2b ./pgmTestFiles/tmp.pgm ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GA.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  B TO A REVERSE:\tPASS\n"
else
    printf "  B TO A REVERSE:\tFAILED\n"
fi

output=$(./pgma2b ./pgmTestFiles/GA.pgm ./pgmTestFiles/tmp.pgm)
output=$(./pgmb2a ./pgmTestFiles/tmp.pgm ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GA.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  A TO B REVERSE:\tPASS\n"
else
    printf "  A TO B REVERSE:\tFAILED\n"
fi

echo ""

printf "Testing ./pgmReduce:\n"
output=$(./pgmReduce ./pgmTestFiles/GA4X4.pgm 1 ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GA4X4reduced1.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  REDUCE BY 1:\t\tPASS\n"
else
    printf "  REDUCE BY 1:\t\tFAILED\n"
fi

output=$(./pgmReduce ./pgmTestFiles/GA4X4.pgm 2 ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GA4X4reduced2.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  REDUCE BY 2:\t\tPASS\n"
else
    printf "  REDUCE BY 2:\t\tFAILED\n"
fi

output=$(./pgmReduce ./pgmTestFiles/GA4X4.pgm 3 ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GA4X4reduced3.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  REDUCE BY 3:\t\tPASS\n"
else
    printf "  REDUCE BY 3:\t\tFAILED\n"
fi

output=$(./pgmReduce ./pgmTestFiles/GA4X4.pgm 4 ./pgmTestFiles/tmp.pgm)
output=$(./pgmComp ./pgmTestFiles/GA4X4reduced4.pgm ./pgmTestFiles/tmp.pgm)
if [ $? == 0 ] && [ "$output" == "IDENTICAL" ]
then
    printf "  REDUCE BY 4:\t\tPASS\n"
else
    printf "  REDUCE BY 4:\t\tFAILED\n"
fi

echo ""

printf "Testing ./pgmTile:\n"

output=$(./pgmTile ./pgmTestFiles/GA4X4.pgm 2 "./pgmTestFiles/testoutput_<row>_<column>.pgm")
output=$(./pgmComp ./pgmTestFiles/testoutput_0_0.pgm ./pgmTestFiles/GA4X4SPLIT_0_0.pgm)
code_1=$?
output=$(./pgmComp ./pgmTestFiles/testoutput_0_1.pgm ./pgmTestFiles/GA4X4SPLIT_0_1.pgm)
code_2=$?
output=$(./pgmComp ./pgmTestFiles/testoutput_1_0.pgm ./pgmTestFiles/GA4X4SPLIT_1_0.pgm)
code_3=$?
output=$(./pgmComp ./pgmTestFiles/testoutput_1_1.pgm ./pgmTestFiles/GA4X4SPLIT_1_1.pgm)

if [ $? == 0 ] && [ $code_1 == 0 ] && [ $code_2 == 0 ] && [ $code_3 == 0 ]
then
    printf "  TILE EVEN OUTPUT:\tPASS\n"
else
    printf "  TILE EVEN OUTPUT:\tFAILED\n"
fi

output=$(./pgmTile ./pgmTestFiles/GA4X4.pgm 3 "./pgmTestFiles/testoutputodd_<row>_<column>.pgm")
output=$(./pgmComp ./pgmTestFiles/testoutputodd_0_0.pgm ./pgmTestFiles/GA4X4SPLIT3_0_0.pgm)
code_1=$?
output=$(./pgmComp ./pgmTestFiles/testoutputodd_0_1.pgm ./pgmTestFiles/GA4X4SPLIT3_0_1.pgm)
code_2=$?
output=$(./pgmComp ./pgmTestFiles/testoutputodd_0_2.pgm ./pgmTestFiles/GA4X4SPLIT3_0_2.pgm)
code_3=$?
output=$(./pgmComp ./pgmTestFiles/testoutputodd_1_0.pgm ./pgmTestFiles/GA4X4SPLIT3_1_0.pgm)
code_4=$?
output=$(./pgmComp ./pgmTestFiles/testoutputodd_1_1.pgm ./pgmTestFiles/GA4X4SPLIT3_1_1.pgm)
code_5=$?
output=$(./pgmComp ./pgmTestFiles/testoutputodd_1_2.pgm ./pgmTestFiles/GA4X4SPLIT3_1_2.pgm)
code_6=$?
output=$(./pgmComp ./pgmTestFiles/testoutputodd_2_0.pgm ./pgmTestFiles/GA4X4SPLIT3_2_0.pgm)
code_7=$?
output=$(./pgmComp ./pgmTestFiles/testoutputodd_2_1.pgm ./pgmTestFiles/GA4X4SPLIT3_2_1.pgm)
code_8=$?
output=$(./pgmComp ./pgmTestFiles/testoutputodd_2_2.pgm ./pgmTestFiles/GA4X4SPLIT3_2_2.pgm)

if [ $? == 0 ] && [ $code_1 == 0 ] && [ $code_2 == 0 ] && [ $code_3 == 0 ] && [ $code_4 == 0 ] && [ $code_5 == 0 ] && [ $code_6 == 0 ] && [ $code_7 == 0 ] && [ $code_8 == 0 ]
then
    printf "  TILE ODD OUTPUT:\tPASS\n"
else
    printf "  TILE ODD OUTPUT:\tFAILED\n"
fi

echo ""

printf "Testing ./pgmAssemble:\n"

output=$(./pgmAssemble ./pgmTestFiles/testoutputassemble.pgm 4 4 0 0 ./pgmTestFiles/GA4X4SPLIT_0_0.pgm 0 2 ./pgmTestFiles/GA4X4SPLIT_0_1.pgm 2 0 ./pgmTestFiles/GA4X4SPLIT_1_1.pgm 2 2 ./pgmTestFiles/GA4X4SPLIT_1_1.pgm)
output=$(./pgmComp ./pgmTestFiles/GA4X4.pgm ./pgmTestFiles/testoutputassemble.pgm)
code=$?

if [ $? == 0 ]
then 
    printf "  ASSEMBLE EVEN:\tPASS\n"
else
    printf "  ASSEMBLE EVEN:\tFAILED\n" 
fi

print_heading "$make_clean_message";
printf "removing files...\n"
make clean > /dev/null
rm ./pgmTestFiles/testoutput* > /dev/null

printf "\n"
