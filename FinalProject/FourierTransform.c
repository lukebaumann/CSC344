#include "FinalProject.h"

int main(int argc, char *argv[]) {
   double *buffer = malloc(BUFFER_SIZE * sizeof(double));
   int bufferOffset = 0;
   int bufferRead = 0;

   void (*fourierTransform)(double *, int) = &FFTAll; 

   if (argc < 2) {
      printf("usage: %s fileName\n", argv[0]);
      exit(-1);
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

   bufferRead = sf_read_double(in, buffer, HALF_BUFFER_SIZE); 

   int i = 0;
   while(++i) {
      for (bufferOffset %= BUFFER_SIZE; bufferOffset < bufferRead - WINDOW_SIZE; bufferOffset += WINDOW_DELTA) {
         (*fourierTransform)(buffer, bufferOffset);
      }

      if ((bufferRead = sf_read_double(in, buffer + HALF_BUFFER_SIZE, HALF_BUFFER_SIZE)) < WINDOW_DELTA) {
         break;
      }

      for (; bufferOffset < HALF_BUFFER_SIZE + bufferRead - WINDOW_SIZE; bufferOffset += WINDOW_DELTA) {
         (*fourierTransform)(buffer, bufferOffset);
      }

      if ((bufferRead = sf_read_double(in, buffer, HALF_BUFFER_SIZE)) < WINDOW_DELTA) {
         break;
      }
      for (; bufferOffset < BUFFER_SIZE; bufferOffset += WINDOW_DELTA) {
         (*fourierTransform)(buffer, bufferOffset);
      }

   }

   sf_close(in);
   free(buffer);

   return 0;
}

void openWaveFile(char *inFileName, SNDFILE *in, SF_INFO *info) {
}

double DFT(double *buffer, int bufferOffset, double frequency) {
   complex double frequencyAmplitude = 0.0;
   complex double temp = 0.0;
   complex double tempExp = 0.0;

   int i = 0;
   if (bufferOffset < BUFFER_SIZE - WINDOW_SIZE) {

      for (i = 0; i < WINDOW_SIZE; i++) {
         tempExp = -I * 2 * M_PI * frequency * i / WINDOW_SIZE;
         temp = buffer[i + bufferOffset] * cexp(tempExp);
         frequencyAmplitude += temp;
      }
   }

   else {
      for (i = 0; i < BUFFER_SIZE - bufferOffset; i++) {
         tempExp = -I * 2 * M_PI * frequency * i / WINDOW_SIZE;
         temp = buffer[i + bufferOffset] * cexp(tempExp);
         frequencyAmplitude += temp;
      }

      for (i = 0; i < WINDOW_SIZE - BUFFER_SIZE + bufferOffset; i++) {
         tempExp = -I * 2 * M_PI * frequency * (i + BUFFER_SIZE - bufferOffset) / WINDOW_SIZE;
         temp = buffer[i] * cexp(tempExp);
         frequencyAmplitude += temp;
      }
   }

   return cabs(frequencyAmplitude);
}

void FFTAll(double *buffer, int bufferOffset) {
   complex double frequencyBuffer[WINDOW_SIZE];
   double maxFrequency = 0.0;
   double maxFrequencyAmplitude = 0.0;

   //FFT(buffer + bufferOffset, WINDOW_SIZE, 1, frequencyBuffer);
   
   int j = 0;
   double testBuffer[1000];
   complex double testFrequencyBuffer[1000];
   for (j = 0; j < 1000; j++) {
      //testBuffer[j] = sin(2 * M_PI * j / 250);
      testBuffer[j] = j; 
   }

   FFT(testBuffer, 8, 1, testFrequencyBuffer);
   for (j = 0; j < 8; j++) {
      printf("Frequency: %d FFT: %lf\n", j, creal(testFrequencyBuffer[j]));
   }
  /*




   int i = 0;
   for (i = 0; i < WINDOW_SIZE; i++) {
      if (cabs(frequencyBuffer[i]) > maxFrequencyAmplitude) {
         maxFrequency = i;
         maxFrequencyAmplitude = cabs(frequencyBuffer[i]);
      }
      printf("f: %d, a: %lf\n", i * 44100 / WINDOW_SIZE, cabs(frequencyBuffer[i]));
   }

   printf("Max Frequency: %lf Amplitude: %lf\n", maxFrequency * 44100 / WINDOW_SIZE, maxFrequencyAmplitude);
   */
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
         temp = result[i];
         frequencyBuffer[i] = temp + result[i + windowSize / 2];
         frequencyBuffer[i + windowSize / 2] = temp - result[i + windowSize / 2];
         //frequencyBuffer[i] = temp + cexp(-I * 2 * M_PI * i / windowSize) * result[i + windowSize / 2];
         //frequencyBuffer[i + windowSize / 2] = temp - cexp(-I * 2 * M_PI * i / windowSize) * result[i + windowSize / 2];
      }

      free(result);
   }
}



void DFTAll(double *buffer, int bufferOffset) {
   double frequency = 0.0;
   double frequencyAmplitude = 0.0;
   double maxFrequency = 0.0;
   double maxFrequencyAmplitude = 0.0;

   for (frequency = 0.0; frequency < WINDOW_SIZE; frequency = frequency + FREQUENCY_DELTA) {
      frequencyAmplitude = DFT(buffer, bufferOffset, frequency);

      if (frequencyAmplitude > maxFrequencyAmplitude) {
         maxFrequency = frequency;
         maxFrequencyAmplitude = frequencyAmplitude;
      }
   }

   printf("Max Frequency: %lf Amplitude: %lf\n", maxFrequency, maxFrequencyAmplitude);
}
