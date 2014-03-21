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
const static int WINDOW_SIZE = 2048;
const static double FREQUENCY_SPACING = SAMPLE_RATE / (double) WINDOW_SIZE;
const static int NUMBER_OF_FREQUENCIES = 14000 / FREQUENCY_SPACING;
const static int WINDOW_DELTA = 4410;
const static int SCALING_FACTOR = 1;
const static int WIDTH = 800;
const static int HEIGHT = 600;
const static int BOTTOM_OF_FREQUENCY_BARS = 580;
const static int TOP_OF_FREQUENCY_BARS = 100;
const static int LEFT_MOST_FREQUENCY_BAR = 10;
const static int RIGHT_MOST_FREQUENCY_BAR = 790;
const static int FREQUENCY_BAR_WIDTH = LEFT_MOST_FREQUENCY_BAR + (RIGHT_MOST_FREQUENCY_BAR - LEFT_MOST_FREQUENCY_BAR) / NUMBER_OF_FREQUENCIES - WIDTH + RIGHT_MOST_FREQUENCY_BAR;
const static int CHARACTER_BUFFER_SIZE = 1000;
const static std::complex<double> I = std::complex<double>(0.0,1.0);

typedef enum {
   INIT = 0,
   FIRST_THIRD,
   SECOND_THIRD,
   THIRD_THIRD,
   CLEAN_UP,
   FINISH
} fourierState;

void initFourierTransform(char *fileName);
double *doNextFrameOfFourierTransform();
void FFT(double *window, int windowSize, int stride, std::complex<double> *frequencyBuffer);
void FFTVisualize(double *window, int windowSize, double *frequencyVisualizeBuffer);
//void makeBar(int frequencyIndex, double normalizedAmplitude, sf::VertexArray *quad);
sf::VertexArray makeBar(int frequencyIndex, double normalizedAmplitude);
void getFrequencyAmplitudes(double *frequencyAmplitudes, int frequencyAmplitudesSize);
int getMaxAmplitudeIndex(double *frequencyAmplitudes, int frequencyAmplitudesSize);

#endif
