#include "FinalProject.h"

void doFourierTransform(char *fileName) {
   double buffer[BUFFER_SIZE];
   int bufferOffset = 0;
   int bufferRead = 0;

   void (*fourierTransform)(double *, int) = &FFTVisualize;

   SNDFILE *in;
   SF_INFO info;

   if (!(in = sf_open(fileName, SFM_READ, &info))) {
      fprintf(stderr, "Error opening sound file: %s.\n", fileName);
      puts(sf_strerror(NULL));
      return; 
   }

   bufferRead = sf_read_double(in, buffer, THIRD_BUFFER_SIZE); 
   if (bufferRead == 0) {
      printf("Read no data.\n");
   }
   else if (bufferRead < 0) {
      fprintf(stderr, "Error on read\n");
      puts(sf_strerror(NULL));
      exit(-1);
   }

   int i = 0;
   while(++i) {
      for (bufferOffset %= 2 * THIRD_BUFFER_SIZE; bufferOffset < bufferRead - WINDOW_SIZE; bufferOffset += WINDOW_DELTA) {
         (*fourierTransform)(buffer + bufferOffset, WINDOW_SIZE);
      }

      if ((bufferRead = sf_read_double(in, buffer + THIRD_BUFFER_SIZE, THIRD_BUFFER_SIZE)) < WINDOW_DELTA) {
         //printf("Broke at 1\n");
         break;
      }

      for (; bufferOffset < THIRD_BUFFER_SIZE + bufferRead - WINDOW_SIZE; bufferOffset += WINDOW_DELTA) {
         (*fourierTransform)(buffer + bufferOffset, WINDOW_SIZE);
      }

      if ((bufferRead = sf_read_double(in, buffer + 2 * THIRD_BUFFER_SIZE, THIRD_BUFFER_SIZE)) < WINDOW_DELTA) {
         //printf("Broke at 2\n");
         break;
      }
      memcpy(buffer, buffer + 2 * THIRD_BUFFER_SIZE, THIRD_BUFFER_SIZE);

      for (; bufferOffset < 2 * THIRD_BUFFER_SIZE; bufferOffset += WINDOW_DELTA) {
         (*fourierTransform)(buffer + bufferOffset, WINDOW_SIZE);
      }

   }

   sf_close(in);
   free(buffer);

   return 0;
}

void DFTAll(double *window, int windowSize) {
   double frequencyAmplitude = 0.0;
   double maxFrequency = 0.0;
   double maxFrequencyAmplitude = 0.0;

   int i = 0;
   for (i = 0; i < windowSize / 2; i++) {
      frequencyAmplitude = DFT(window, windowSize, i);

      if (frequencyAmplitude > maxFrequencyAmplitude) {
         maxFrequency = i;
         maxFrequencyAmplitude = frequencyAmplitude;
      }
   }

   printf("Max Frequency: %lf Amplitude: %lf\n", maxFrequency * SAMPLE_RATE / windowSize, maxFrequencyAmplitude);
}

double DFT(double *window, int windowSize, int frequency) {
   complex double frequencyAmplitude = 0.0;
   complex double temp = 0.0;
   complex double tempExp = 0.0;

   int i = 0;
   for (i = 0; i < windowSize; i++) {
      tempExp = -I * 2 * M_PI * frequency * i / windowSize;
      temp = window[i] * cexp(tempExp);
      frequencyAmplitude += temp;
   }

   return cabs(frequencyAmplitude);
}

void FFTVisualize(double *window, int windowSize) {
   complex double frequencyBuffer[WINDOW_SIZE];
   double frequencyVisualizeBuffer[WINDOW_SIZE];

   FFT(window, windowSize, 1, frequencyBuffer);   

   int i = 0;
   for (i = 0; i < windowSize; i++) {
      frequencyVisualizeBuffer[i] = cabs(frequencyBuffer[i]);
   }

   updateFrequencies(frequencyVisualizeBuffer);
}

void FFTAll(double *window, int windowSize) {
   complex double frequencyBuffer[WINDOW_SIZE];
   double maxFrequency = 0.0;
   double maxFrequencyAmplitude = 0.0;

   FFT(window, windowSize, 1, frequencyBuffer);   

   int i = 0;
   for (i = 0; i < windowSize / 2; i++) {
      if (cabs(frequencyBuffer[i]) > maxFrequencyAmplitude) {
         maxFrequency = i;
         maxFrequencyAmplitude = cabs(frequencyBuffer[i]);
      }
      //printf("f: %d, a: %lf\n", i * 44100 / WINDOW_SIZE, cabs(frequencyBuffer[i]));
   }

   printf("Max Frequency: %lf Amplitude: %lf\n", maxFrequency * SAMPLE_RATE / windowSize, maxFrequencyAmplitude);
}

void FFT(double *window, int windowSize, int stride, complex double *frequencyBuffer) {
   if (windowSize == 1) {
      frequencyBuffer[0] = window[0];
   }
   else if (windowSize == 0) {
      printf("ASDF\n"); 
   }
   else {
      complex double *result = malloc(sizeof(complex double) * windowSize);

      FFT(window, windowSize / 2, 2 * stride, result);
      FFT(window + stride, windowSize / 2, 2 * stride, result + windowSize / 2);

      int i = 0;
      complex double temp = 0.0;

      for (i = 0;  i < windowSize / 2; i++) {
         frequencyBuffer[i] = result[i] + cexp(-I * 2 * M_PI * i / windowSize) * result[i + windowSize / 2];
         frequencyBuffer[i + windowSize / 2] = result[i] - cexp(-I * 2 * M_PI * i / windowSize) * result[i + windowSize / 2];
      }

      free(result);
   }
}
