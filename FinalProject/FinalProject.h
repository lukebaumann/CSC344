#ifndef __FINAL_PROJECT_H
#define __FINAL_PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>
#include <sndfile.h>

#define BUFFER_SIZE 99999
#define THIRD_BUFFER_SIZE 33333
#define SAMPLE_RATE 44100
#define WINDOW_SIZE 32768
#define WINDOW_DELTA 2450
#define FREQUENCY_DELTA 1.0

typedef enum {
   INIT = 0,
   FIRST_THIRD,
   SECOND_THIRD,
   THIRD_THIRD,
   FINISH
} fourierState;

// Opens a SNDFILE
void initFourierTransform(char *fileName) {
void doNextFrameOfFourierTransform() {
void FFT(double *window, int windowSize, int stride, complex double *frequencyBuffer);
void FFTVisualize(double *window, int windowSize);

#endif
