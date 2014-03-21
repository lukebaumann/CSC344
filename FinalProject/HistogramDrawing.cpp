#include "FinalProject.hpp"

extern double *doNextFrameOfFourierTransform();
extern void *initFourierTransform();

int main(int argc, char *argv[]) {
   double *frequencyAmplitudes = new double[WINDOW_SIZE];
   int maxAmplitudeIndex = 0;
   double maxAmplitude = 0.0;
   char maxAmplitudeBuffer[CHARACTER_BUFFER_SIZE];
//   char topFrequencyBuffer[CHARACTER_BUFFER_SIZE];
//   char midFrequencyBuffer[CHARACTER_BUFFER_SIZE];
//   char bottomFrequencyBuffer[CHARACTER_BUFFER_SIZE];
   sf::Font font;

   if (argc != 2) {
      fprintf(stderr, "usage: %s fileName.wav\n", argv[0]);
      exit(-1);
   }

   font.loadFromFile("Arial.ttf");
   sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Histogram");

   initFourierTransform(argv[1]);

   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) {
            window.close();
         }
      }

      window.clear(sf::Color::White);

      getFrequencyAmplitudes(frequencyAmplitudes, WINDOW_SIZE);
      maxAmplitudeIndex = getMaxAmplitudeIndex(frequencyAmplitudes, NUMBER_OF_FREQUENCIES); 
      maxAmplitude = frequencyAmplitudes[maxAmplitudeIndex];

      for (int i = 1; i < NUMBER_OF_FREQUENCIES; i++) {
         window.draw(makeBar(i, maxAmplitude > 1.0 ? frequencyAmplitudes[i] / maxAmplitude : frequencyAmplitudes[i] * maxAmplitude));
      }

      sprintf(maxAmplitudeBuffer, "Number of Frequencies: %d\tFrequency Spacing: %6.3lf\nBottom Frequency: %6.3lf\tTop Frequency: %6.3lf\nMax Frequency: %6.3lf\tMax Amplitude: %6.3lf", NUMBER_OF_FREQUENCIES, FREQUENCY_SPACING, FREQUENCY_SPACING * 1, FREQUENCY_SPACING * NUMBER_OF_FREQUENCIES, maxAmplitudeIndex * SAMPLE_RATE / (double) WINDOW_SIZE, maxAmplitude);
      sf::Text text(maxAmplitudeBuffer, font);
      text.setCharacterSize(16);
      text.setColor(sf::Color::Black);

      window.draw(text);


      window.display();

      //sf::sleep(sf::seconds(1));
   }

   delete[] frequencyAmplitudes;
}

void getFrequencyAmplitudes(double *frequencyAmplitudes, int frequencyAmplitudesSize) {
   double *frequencies;
   if ((frequencies = doNextFrameOfFourierTransform())) {
      memcpy(frequencyAmplitudes, frequencies, frequencyAmplitudesSize * sizeof(double));
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
   for (int i = 1; i < frequencyAmplitudesSize; i++) {
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
