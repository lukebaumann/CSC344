#include "FinalProject.h"

int main(int argc, char *argv[]) {
   int *buffer = malloc(BUFFER_SIZE * sizeof(int));
   int bufferOffset = 0;
   int bufferRead = 0;


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

   bufferRead = sf_read_int(in, buffer, HALF_BUFFER_SIZE); 

   int i = 0;
   while(++i) {
      printf("i: %d\n", i);
      for (bufferOffset %= BUFFER_SIZE; bufferOffset < bufferRead - WINDOW_SIZE; bufferOffset += WINDOW_DELTA) {
         findAllFrequencyAmplitudes(buffer, bufferOffset);
      }

      if ((bufferRead = sf_read_int(in, buffer + HALF_BUFFER_SIZE, HALF_BUFFER_SIZE)) < WINDOW_DELTA) {
         break;
      }
      for (; bufferOffset < HALF_BUFFER_SIZE + bufferRead - WINDOW_SIZE; bufferOffset += WINDOW_DELTA) {
         findAllFrequencyAmplitudes(buffer, bufferOffset);
      }

      if ((bufferRead = sf_read_int(in, buffer, HALF_BUFFER_SIZE)) < WINDOW_DELTA) {
         break;
      }
      for (; bufferOffset < BUFFER_SIZE; bufferOffset += WINDOW_DELTA) {
         findAllFrequencyAmplitudes(buffer + HALF_BUFFER_SIZE + bufferOffset, WINDOW_SIZE);
      }
   }

   sf_close(in);
   free(buffer);

   return 0;
}

void openWaveFile(char *inFileName, SNDFILE *in, SF_INFO *info) {
}

double findOneFrequencyAmplitude(int *buffer, int bufferOffset, double frequency) {
   complex double frequencyAmplitude = 0.0;
   complex double temp = 0.0;
   complex double tempExp = 0.0;

   int i = 0;
   if (bufferOffset < BUFFER_SIZE - WINDOW_SIZE) {

      for (i = bufferOffset; i < bufferOffset + WINDOW_SIZE; i++) {
         tempExp = -I * 2 * M_PI * frequency * i / WINDOW_SIZE;
         temp = buffer[i] * cexp(tempExp);
         frequencyAmplitude += temp;
      }
   }

   else {
      for (i = bufferOffset; i < BUFFER_SIZE; i++) {
         tempExp = -I * 2 * M_PI * frequency * i / WINDOW_SIZE;
         temp = buffer[i] * cexp(tempExp);
         frequencyAmplitude += temp;
      }

      for (i = 0; i < WINDOW_SIZE - BUFFER_SIZE + bufferOffset; i++) {
         tempExp = -I * 2 * M_PI * frequency * i / WINDOW_SIZE;
         temp = buffer[i] * cexp(tempExp);
         frequencyAmplitude += temp;
      }
   }

   return cabs(frequencyAmplitude) / WINDOW_SIZE;
}

void findAllFrequencyAmplitudes(int *buffer, int bufferOffset) {
   double frequency = 0.0;
   double frequencyAmplitude = 0.0;

   for (frequency = 0.0; frequency < MAX_FREQUENCY * 2; frequency = frequency + FREQUENCY_DELTA) {
      frequencyAmplitude = findOneFrequencyAmplitude(buffer, bufferOffset, frequency);
      if (frequencyAmplitude > 100) {
         printf("Frequency: %lf Amplitude: %lf\n", frequency, frequencyAmplitude);
      }
   }
}
