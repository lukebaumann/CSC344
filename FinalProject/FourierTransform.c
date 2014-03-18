#include "FinalProject.h"

int main(int argc, char *argv[]) {
   double *buffer = malloc(BUFFER_SIZE * sizeof(double));
   int bufferOffset = 0;
   int bufferRead = 0;

   if (argc < 2) {
      printf("usage: %s fileName [optional -d for DFT]\n", argv[0]);
      exit(-1);
   }

   void (*fourierTransform)(double *, int);

   if (argc > 2 && !strcmp(argv[2], "-d")) {
      fourierTransform = &DFTAll;
   }
   else {
      fourierTransform = &FFTAll;
   }

   char *fileName = argv[1];
   SNDFILE *in;
   SF_INFO info;

   if (!(in = sf_open(fileName, SFM_READ, &info))) {
      fprintf(stderr, "Error opening sound file: %s.\n", fileName);
      puts(sf_strerror(NULL));
      exit(-1);
   }
   else {
      printf("Successful open\n");
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

void FFTAll(double *window, int windowSize) {
   complex double frequencyBuffer[WINDOW_SIZE];
   double maxFrequency = 0.0;
   double maxFrequencyAmplitude = 0.0;

   FFT(window, windowSize, 1, frequencyBuffer);
   
   /*int j = 0;
   double testBuffer[100000];
   complex double testFrequencyBuffer[100000];
   for (j = 0; j < 100000; j++) {
      testBuffer[j] = sin(2 * M_PI * 30 * j / SAMPLE_RATE);
   }

   FFT(testBuffer, WINDOW_SIZE, 1, testFrequencyBuffer);
   for (j = 0; j < WINDOW_SIZE; j++) {
      printf("Frequency: %0.4lf FFT: %0.4lf\n", j * (double)(SAMPLE_RATE) / WINDOW_SIZE, cabs(testFrequencyBuffer[j]));
   }
  */

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

