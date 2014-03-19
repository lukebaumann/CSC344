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
   static double *buffer = new double[BUFFER_SIZE];
   static int bufferOffset = 0;
   static int bufferRead = 0;
   static double *frequencyVisualizeBuffer = new double[WINDOW_SIZE];

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
      if (bufferOffset <= bufferRead - WINDOW_SIZE) {
         FFTVisualize(buffer + bufferOffset, WINDOW_SIZE, frequencyVisualizeBuffer);
         bufferOffset += WINDOW_DELTA;
         state = FIRST_THIRD;

         ret = frequencyVisualizeBuffer;
         break;
      }
      // else fall through
      else {
         if ((bufferRead = sf_read_double(in, buffer + THIRD_BUFFER_SIZE, THIRD_BUFFER_SIZE)) < WINDOW_DELTA) {
            //printf("Broke at 1\n");
            state = CLEAN_UP;
         }
         else {
            state = SECOND_THIRD;
         }
      }

   case SECOND_THIRD:
      if (bufferOffset <= THIRD_BUFFER_SIZE + bufferRead - WINDOW_SIZE) {
         FFTVisualize(buffer + bufferOffset, WINDOW_SIZE, frequencyVisualizeBuffer);
         bufferOffset += WINDOW_DELTA;
         state = SECOND_THIRD;

         ret = frequencyVisualizeBuffer;
         break;
      }
      // else fall through
      else {
         if ((bufferRead = sf_read_double(in, buffer + 2 * THIRD_BUFFER_SIZE, THIRD_BUFFER_SIZE)) < WINDOW_DELTA) {
            //printf("Broke at 2\n");
            state = CLEAN_UP;
         }
         else {
            memcpy(buffer, buffer + 2 * THIRD_BUFFER_SIZE, THIRD_BUFFER_SIZE);
            state = THIRD_THIRD;
         }
      }

   case THIRD_THIRD:
      if (bufferOffset <= 2 * THIRD_BUFFER_SIZE) {
         FFTVisualize(buffer + bufferOffset, WINDOW_SIZE, frequencyVisualizeBuffer);
         bufferOffset += WINDOW_DELTA;
         state = THIRD_THIRD;
      }
      // else do FIRST_THIRD stuff
      else {
         bufferOffset -= 2 * THIRD_BUFFER_SIZE;
         FFTVisualize(buffer + bufferOffset, WINDOW_SIZE, frequencyVisualizeBuffer);
         bufferOffset += WINDOW_DELTA;
         state = FIRST_THIRD;

      }

      ret = frequencyVisualizeBuffer;
      break;

   case CLEAN_UP:
      state = FINISH;
      sf_close(in);
      delete[] buffer;
      delete[] frequencyVisualizeBuffer;
      // Fall through to FINISH

   case FINISH:
         ret = NULL;
         break;
   }

   return ret;
}

void FFTVisualize(double *window, int windowSize, double *frequencyVisualizeBuffer) {
   std::complex<double> *frequencyBuffer = new std::complex<double>[windowSize];

   FFT(window, windowSize, 1, frequencyBuffer);   

   int i = 0;
   for (i = 0; i < windowSize; i++) {
      //frequencyVisualizeBuffer[i] = 10 * log10(std::abs(frequencyBuffer[i]));
      frequencyVisualizeBuffer[i] = std::abs(frequencyBuffer[i]);

      printf("freq: %lf amp: %lf\n", i * SAMPLE_RATE / (double) windowSize, frequencyVisualizeBuffer[i]); 
   }

   delete[] frequencyBuffer;
}

void FFT(double *window, int windowSize, int stride, std::complex<double> *frequencyBuffer) {
   if (windowSize == 1) {
      //frequencyBuffer[0] = window[0] / SCALING_FACTOR;
      frequencyBuffer[0] = window[0];
   }
   else if (windowSize == 0) {
      printf("ASDF\n"); 
   }
   else {
      std::complex<double> *result =  new std::complex<double>[windowSize];

      FFT(window, windowSize / 2, 2 * stride, result);
      FFT(window + stride, windowSize / 2, 2 * stride, result + windowSize / 2);

      int i = 0;
      std::complex<double> temp = 0.0;

      for (i = 0;  i < windowSize / 2; i++) {
         frequencyBuffer[i] = result[i] + exp(std::complex<double>(0.0, 1.0) * (-2 * M_PI * i / (double) windowSize)) * result[i + windowSize / 2];
         frequencyBuffer[i + windowSize / 2] = result[i] - exp(std::complex<double>(0.0, 1.0) * (-2 * M_PI * i / (double) windowSize)) * result[i + windowSize / 2];
      }

      delete[] result;
   }
}
