#include "FinalProject.hpp"

extern double *doNextFrameOfFourierTransform();
extern void *initFourierTransform();

int main(int argc, char *argv[]) {
   double *frequencyAmplitudes = new double[WINDOW_SIZE];
   int maxAmplitudeIndex = 0;
   double maxAmplitude = 0.0;
   double runningAverageMaxAmplitude = 0.0;
   char maxAmplitudeBuffer[30];

   if (argc != 2) {
      fprintf(stderr, "usage: %s fileName.wav\n", argv[0]);
      exit(-1);
   }

   initFourierTransform(argv[1]);

   sf::Font font;
   font.loadFromFile("Arial.ttf");

   sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Histogram");

   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) {
            window.close();
         }
      }

      window.clear(sf::Color::White);

      getFrequencyAmplitudes(frequencyAmplitudes, WINDOW_SIZE);
      maxAmplitudeIndex = getMaxAmplitudeIndex(frequencyAmplitudes, WINDOW_SIZE); 
      maxAmplitude = frequencyAmplitudes[maxAmplitudeIndex];

      runningAverageMaxAmplitude = runningAverageMaxAmplitude * 0.8 + maxAmplitude * 0.2;

      if (maxAmplitude < runningAverageMaxAmplitude) {
         maxAmplitude = runningAverageMaxAmplitude;
      }

      for (int i = 0; i < WINDOW_SIZE; i++) {
         window.draw(makeBar(i, frequencyAmplitudes[i] / maxAmplitude));
      }

      sprintf(maxAmplitudeBuffer, "Max\nFrequency: %10.3lf\nAmplitude: %10.3lf", maxAmplitudeIndex * SAMPLE_RATE / (double) WINDOW_SIZE, maxAmplitude);
      sf::Text text(maxAmplitudeBuffer, font);
      text.setCharacterSize(16);
      text.setColor(sf::Color::Black);

      window.draw(text);


      window.display();

      sf::sleep(sf::seconds(1));
   }

   delete[] frequencyAmplitudes;
}

void getFrequencyAmplitudes(double *frequencyAmplitudes, int frequencyAmplitudesSize) {
   double *frequencies;
   if ((frequencies = doNextFrameOfFourierTransform())) {
      memcpy(frequencyAmplitudes, frequencies, frequencyAmplitudesSize);
   }
   else {
      for (int i = 0; i < frequencyAmplitudesSize; i++) {
         frequencyAmplitudes[i] = 0.0;
      }
   }
}

int getMaxAmplitudeIndex(double *frequencyAmplitudes, int frequencyAmplitudesSize) {
   double maxAmplitude = 0.0;
   int maxAmplitudeIndex = 0;
   for (int i = 0; i < frequencyAmplitudesSize; i++) {
      printf("freq: %lf ampl: %lf\n", i * SAMPLE_RATE / (double) WINDOW_SIZE, frequencyAmplitudes[i]);
      if (frequencyAmplitudes[i] > maxAmplitude) {
         maxAmplitude = frequencyAmplitudes[i];
         maxAmplitudeIndex = i;
      }
   }

   return maxAmplitudeIndex;
}

sf::VertexArray makeBar(int frequencyIndex, double normalizedAmplitude) {
   sf::VertexArray quad(sf::Quads, 4);

   int left_x = LEFT_MOST_FREQUENCY_BAR + frequencyIndex * FREQUENCY_BAR_WIDTH;
   int right_x = left_x + FREQUENCY_BAR_WIDTH; 
   int bottom_y = BOTTOM_OF_FREQUENCY_BARS;
   int top_y = TOP_OF_FREQUENCY_BARS + (1.0 - normalizedAmplitude) * (bottom_y - TOP_OF_FREQUENCY_BARS);

   quad[0].position = sf::Vector2f(left_x, top_y);
   quad[0].color = sf::Color::Red;
   quad[1].position = sf::Vector2f(right_x, top_y);
   quad[1].color = sf::Color::Blue;
   quad[2].position = sf::Vector2f(right_x, bottom_y);
   quad[2].color = sf::Color::Green;
   quad[3].position = sf::Vector2f(left_x, bottom_y);
   quad[3].color = sf::Color::Black;

   return quad;
}
