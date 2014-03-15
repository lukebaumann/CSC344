#!/bin/bash

PROGRAM_NAME="DFT"
OUTPUT_NAME="test.out"

echo "Compiling program..."
sleep 0.5
gcc "$PROGRAM_NAME".c -o $PROGRAM_NAME || { echo "Compiling error"; exit $?; }

echo "Executing program..."
sleep 0.5
time $PROGRAM_NAME > $OUTPUT_NAME || { echo "Executing error"; exit $?; }

if [ $# -ne 0 ]
then
   echo "Committing changes..."
   sleep 0.5
   git add "--all"
   git commit "-am $1"
fi

echo "Opening file..."
sleep 0.5
less test.out

