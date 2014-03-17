#!/bin/bash

PROGRAM_NAME="FourierTransform"
OUTPUT_NAME="test.out"

echo "Compiling program..."
sleep 0.5
gcc "$PROGRAM_NAME".c -lsndfile -o $PROGRAM_NAME || { echo "Compiling error"; exit $?; }

if [ $# -eq 0 ]
then
echo "Usage: $0 in.wav [optional commit message]"
exit -1
fi

echo "Executing program..."
sleep 0.5
time $PROGRAM_NAME $1 > $OUTPUT_NAME || { echo "Executing error"; exit $?; }

if [ $# -eq 2 ]
then
   echo "Committing changes..."
   sleep 0.5
   git add "--all"
   git commit "-am $2"
fi

echo "Opening file..."
sleep 0.5
less test.out

