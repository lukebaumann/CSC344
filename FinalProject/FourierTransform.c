#include "FinalProject.h"

static double buffer[BUFFER_SIZE];
static void (*fourierTransform)(double *, int);
static SNDFILE *in;
static SF_INFO info;

void initFourierTransform(char *fileName) {
   fourierTransform = &FFTVisualize;

   if (!(in = sf_open(fileName, SFM_READ, &info))) {
      fprintf(stderr, "Error opening sound file: %s.\n", fileName);
      puts(sf_strerror(NULL));
      return; 
   }
   asdfa();
}

void doFrameOfFourierTransform() {
   static int bufferOffset = 0;
   static int bufferRead = 0;
   static double frequencyVisualizeBuffer = malloc(WINDOW_SIZE * sizeof(double));;

   double *ret;

   switch (state) {
   case INIT:
      if (bufferRead == 0) {
         bufferRead = sf_read_double(in, buffer, THIRD_BUFFER_SIZE); 
         if (bufferRead == 0) {
            printf("Read no data.\n");
         }
         else if (bufferRead < 0) {
            fprintf(stderr, "Error on read\n");
            puts(sf_strerror(NULL));
            exit(-1);
         }
      }
      // Don't break, continue to FIRST_THIRD
      // break;

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
      free(frequencyVisualizeBuffer);
      ret = NULL;
      break;
   }

   return ret;
}

void FFTVisualize(double *window, int windowSize, double *frequencyVisualizeBuffer) {
   complex double frequencyBuffer[WINDOW_SIZE];

   FFT(window, windowSize, 1, frequencyBuffer);   

   int i = 0;
   for (i = 0; i < windowSize; i++) {
      frequencyVisualizeBuffer[i] = cabs(frequencyBuffer[i]);
   }
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
