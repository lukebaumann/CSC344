#ifndef __FINAL_PROJECT_H
#define __FINAL_PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <sndfile.h>

#define BUFFER_SIZE 100000
#define HALF_BUFFER_SIZE 50000
#define WINDOW_SIZE 44100
//#define SAMPLE_RATE 44100
// MAX_FREQUENCY can be at most half of the total size of the window
#define MAX_FREQUENCY 2100
#define WINDOW_DELTA 22050
#define FREQUENCY_DELTA 1.0

// Opens a SNDFILE
void openWaveFile(char *inFileName, SNDFILE *in, SF_INFO *info);
// Computes the DFT for a single frequency using a given window of samples.
// Returns the amplitude of the given frequency.
double DFT(double *buffer, int bufferSize, double frequency);
// Runs DFT for all the frequencies and does something useful with the
// amplitudes
void DFTAll(double *buffer, int bufferSize);

#endif
