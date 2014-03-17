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
   double frequencyBuffer[WINDOW_SIZE / 2];

   FFT(buffer, bufferOffset, WINDOW_SIZE, 1, frequencyBuffer);

   int i = 0;
   for (i = 0; i < WINDOW_SIZE; i++) {
      print("Frequency: %d Amplitude: %lf\n", i, frequencyBuffer[i]);
   }

}

void FFT(double *window, int windowOffset, int windowSize, int stride, double *frequencyBuffer) {
   if (windowSize == 1) {
      frequencyBuffer[windowOffset] = window[windowOffset];
   }
   else {
      FFT(window, windowOffset, windowSize / 2, 2 * stride, frequencyBuffer);
      FFT(window, windowOffset + stride, windowSize / 2, 2 * stride, frequencyBuffer);
      
      int i = 0;
      double temp = 0.0;
      for (i = 0; i < bufferSize / 2; i++) {
         temp = frequencyBuffer[i];
         frequencyBuffer[i] = temp + cexp(-I * 2 * M_PI * i / bufferSize) * frequencyBuffer[i + bufferSize / 2];
         frequencyBuffer[i + bufferSize / 2] = temp - cexp(-I * 2 * M_PI * i / bufferSize) * frequencyBuffer[i + bufferSize / 2];
      }
   }
}



void DFTAll(double *buffer, int bufferOffset) {
   double frequency = 0.0;
   double frequencyAmplitude = 0.0;
   double maxFrequency = 0.0;
   double maxFrequencyAmplitude = 0.0;

   for (frequency = 0.0; frequency < MAX_FREQUENCY * 2; frequency = frequency + FREQUENCY_DELTA) {
      frequencyAmplitude = DFT(buffer, bufferOffset, frequency);

      if (frequencyAmplitude > maxFrequencyAmplitude) {
         maxFrequency = frequency;
         maxFrequencyAmplitude = frequencyAmplitude;
      }
   }

   printf("Max Frequency: %lf Amplitude: %lf\n", maxFrequency, maxFrequencyAmplitude);
}
