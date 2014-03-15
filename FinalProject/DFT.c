#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

#define BUFFER_SIZE 1000
#define WINDOW_SIZE 100
// MAX_FREQUENCY can be at most half of the total size of the window
#define MAX_FREQUENCY 50
#define MAX_AMPLITUDE 1000
#define FREQUENCY_DELTA 1.0

double findOneFrequencyAmplitude(int *window, int windowSize, double frequency);

int main() {
   int *buffer = malloc(BUFFER_SIZE * sizeof(int));
   
   int i = 0;
   for (i = 0; i < BUFFER_SIZE; i++) {
      buffer[i] = MAX_AMPLITUDE * (sin(2 * M_PI * 4 * i / BUFFER_SIZE) + sin(2 * M_PI * 23 * i / BUFFER_SIZE));
      //printf("buffer[%d]: %d\n", i, buffer[i]);
   }

   double frequency = 0.0;
   double frequencyAmplitude = 0.0;

   for (frequency = 0.0; frequency < MAX_FREQUENCY * 2; frequency = frequency + FREQUENCY_DELTA) {
      frequencyAmplitude = findOneFrequencyAmplitude(buffer, BUFFER_SIZE, frequency);
      if (frequencyAmplitude > 100) {
         printf("Frequency: %lf Amplitude: %lf\n", frequency, frequencyAmplitude);
      }
   }

   return 0;
}

// Fills the window with data from the buffer until the window is full or the
// buffer is empty.
// Returns the number of data points filled in the window
int fillWindow(int *window, int windowSize, int *buffer, int bufferOffset) {
   int i = 0;
   for (i = 0; i < windowSize && buffer[bufferOffset + i]; i++) {
      window[i] = buffer[bufferOffset + i];
   }

   return i;
}

// Computes the DFT for a single frequency using a given window of samples.
// Returns the amplitude of the given frequency.
double findOneFrequencyAmplitude(int *window, int windowSize, double frequency) {
   int i = 0;
   complex double frequencyAmplitude = 0.0;
   complex double temp = 0.0;
   complex double tempExp = 0.0;

   for (i = 0; i < windowSize; i++) {
      tempExp = -I * 2 * M_PI * frequency * i / windowSize;
      temp = window[i] * cexp(tempExp);
      frequencyAmplitude += temp;

//      printf("temp %d: %lf + i%lf\n", i, creal(temp), cimag(temp));
   }

//   printf("frequencyAmplitude: %lf + i%lf. Amplitude: %lf\n", creal(frequencyAmplitude), cimag(frequencyAmplitude), cabs(frequencyAmplitude));
   return cabs(frequencyAmplitude);
}
