#!/bin/bash

# Load Arguments
num1=$1  # Number 1 to add
num2=$2  # Number 2 to add
file=$3  # File to use for calculation

number_re='^[0-9]+$'

# Check if Numbers are numbers
if ! [[ $num1 =~ $re ]] ; then
	echo "$num1 is not a number"
	exit 1
fi
if ! [[ $num2 =~ $re ]] ; then
	echo "$num2 is not a number"
	exit 1
fi

# Generate the input file
python3 inputter.py $num1 $num2

# Do the thing and pipe to out file
../computer $file < num.in > out 

# Read the outfile nicely
python3 outputReader.py out
	
