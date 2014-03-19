#include "FinalProject.hpp"
static SNDFILE *in;
static SF_INFO info;

void initFourierTransform(char *fileName) {
   if (!(in = sf_open(fileName, SFM_READ, &info))) {
      fprintf(stderr, "Error opening sound file: %s.\n", fileName);
      puts(sf_strerror(NULL));
      return; 
   }
}

double *doNextFrameOfFourierTransform() {
   static fourierState state;
   static double buffer[BUFFER_SIZE];
   static int bufferOffset = 0;
   static int bufferRead = 0;
   static double frequencyVisualizeBuffer[WINDOW_SIZE];

   double *ret;

   switch (state) {
   case INIT:
      bufferRead = sf_read_double(in, buffer, THIRD_BUFFER_SIZE); 
      if (bufferRead == 0) {
         printf("Read no data.\n");
      }
      else if (bufferRead < 0) {
         fprintf(stderr, "Error on read\n");
         puts(sf_strerror(NULL));
         exit(-1);
      }
      state = FIRST_THIRD;
      // Don't break, go right into the next state;

   case FIRST_THIRD:
      bufferOffset %= 2 * THIRD_BUFFER_SIZE;
      FFTVisualize(buffer + bufferOffset, WINDOW_SIZE, frequencyVisualizeBuffer);
      bufferOffset += WINDOW_DELTA;

      if (bufferOffset < bufferRead - WINDOW_SIZE) {
         if ((bufferRead = sf_read_double(in, buffer + THIRD_BUFFER_SIZE, THIRD_BUFFER_SIZE)) < WINDOW_DELTA) {
            //printf("Broke at 1\n");
            state = FINISH;
         }
         else {
            state = SECOND_THIRD;
         }
      }
      else {
         state = FIRST_THIRD;
      }
      ret = frequencyVisualizeBuffer;
      break;

   case SECOND_THIRD:
      FFTVisualize(buffer + bufferOffset, WINDOW_SIZE, frequencyVisualizeBuffer);
      bufferOffset += WINDOW_DELTA;

      if (bufferOffset < THIRD_BUFFER_SIZE + bufferRead - WINDOW_SIZE) {
         if ((bufferRead = sf_read_double(in, buffer + 2 * THIRD_BUFFER_SIZE, THIRD_BUFFER_SIZE)) < WINDOW_DELTA) {
            //printf("Broke at 2\n");
            state = FINISH;
         }
      }
      else {
         memcpy(buffer, buffer + 2 * THIRD_BUFFER_SIZE, THIRD_BUFFER_SIZE);
         state = THIRD_THIRD;
      }
      ret = frequencyVisualizeBuffer;
      break;

   case THIRD_THIRD:
      FFTVisualize(buffer + bufferOffset, WINDOW_SIZE, frequencyVisualizeBuffer);
      bufferOffset += WINDOW_DELTA;

      if (bufferOffset < 2 * THIRD_BUFFER_SIZE) {
         state = FIRST_THIRD;
      }
      else {
         state = THIRD_THIRD;
      }
      ret = frequencyVisualizeBuffer;
      break;

   case FINISH:
      state = FINISH;
      ret = NULL;
      break;
   }

   return ret;
}

void FFTVisualize(double *window, int windowSize, double *frequencyVisualizeBuffer) {
   std::complex<double> frequencyBuffer[WINDOW_SIZE];

   FFT(window, windowSize, 1, frequencyBuffer);   

   int i = 0;
   for (i = 0; i < windowSize; i++) {
      frequencyVisualizeBuffer[i] = std::abs(frequencyBuffer[i]);
   }
}

void FFT(double *window, int windowSize, int stride, std::complex<double> *frequencyBuffer) {
   if (windowSize == 1) {
      frequencyBuffer[0] = window[0];
   }
   else if (windowSize == 0) {
      printf("ASDF\n"); 
   }
   else {
      std::complex<double> *result = (std::complex<double> *) malloc(sizeof(std::complex<double>) * windowSize);

      FFT(window, windowSize / 2, 2 * stride, result);
      FFT(window + stride, windowSize / 2, 2 * stride, result + windowSize / 2);

      int i = 0;
      std::complex<double> temp = 0.0;

      for (i = 0;  i < windowSize / 2; i++) {
         frequencyBuffer[i] = result[i] + exp(std::complex<double>(0.0, -1.0) * (2 * M_PI * i / (double) windowSize)) * result[i + windowSize / 2];
         frequencyBuffer[i + windowSize / 2] = result[i] - exp(std::complex<double>(0.0, -1.0) * (2 * M_PI * i / (double) windowSize)) * result[i + windowSize / 2];
      }

      free(result);
   }
}
