#ifndef __FINAL_PROJECT_H
#define __FINAL_PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sndfile.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>

const static int BUFFER_SIZE = 99999;
const static int THIRD_BUFFER_SIZE = 33333;
const static int SAMPLE_RATE = 44100;
const static int WINDOW_SIZE = 4096;
const static int WINDOW_DELTA = 2450;
const static int SCALING_FACTOR = 1000;
const static int WIDTH = 800;
const static int HEIGHT = 600;
const static int BOTTOM_OF_FREQUENCY_BARS = 590;
const static int TOP_OF_FREQUENCY_BARS = 10;
const static int LEFT_MOST_FREQUENCY_BAR = 10;
const static int RIGHT_MOST_FREQUENCY_BAR = 790;
const static int FREQUENCY_BAR_WIDTH = LEFT_MOST_FREQUENCY_BAR + (RIGHT_MOST_FREQUENCY_BAR - LEFT_MOST_FREQUENCY_BAR) / WINDOW_SIZE - WIDTH + RIGHT_MOST_FREQUENCY_BAR;
const static std::complex<double> I = std::complex<double>(0.0,1.0);

typedef enum {
   INIT = 0,
   FIRST_THIRD,
   SECOND_THIRD,
   THIRD_THIRD,
   FINISH
} fourierState;

void initFourierTransform(char *fileName);
double *doNextFrameOfFourierTransform();
void FFT(double *window, int windowSize, int stride, std::complex<double> *frequencyBuffer);
void FFTVisualize(double *window, int windowSize, double *frequencyVisualizeBuffer);
sf::VertexArray makeBar(int frequencyIndex, double normalizedAmplitude);
void getFrequencyAmplitudes(double *frequencyAmplitudes, int frequencyAmplitudesSize);
int getMaxAmplitudeIndex(double *frequencyAmplitudes, int frequencyAmplitudesSize);

#endif
