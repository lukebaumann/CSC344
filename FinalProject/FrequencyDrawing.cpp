#include "FinalProject.hpp"

extern double *doNextFrameOfFourierTransform();
extern void *initFourierTransform();

int main(int argc, char *argv[]) {
   double *frequencyAmplitudes = new double[WINDOW_SIZE];
   int maxAmplitudeIndex = 0;
   double maxAmplitude = 0.0;
   char frequencyInfoBuffer[CHARACTER_BUFFER_SIZE];
   char maxInfoBuffer[CHARACTER_BUFFER_SIZE];
   char topFrequencyBuffer[CHARACTER_BUFFER_SIZE];
   char middleFrequencyBuffer[CHARACTER_BUFFER_SIZE];
   char bottomFrequencyBuffer[CHARACTER_BUFFER_SIZE];
   sf::Text frequencyInfoText;
   sf::Text maxInfoText;
   sf::Text bottomFrequencyText;
   sf::Text middleFrequencyText;
   sf::Text topFrequencyText;
   
   sf::Font font;

   if (argc != 2) {
      fprintf(stderr, "usage: %s fileName.wav\n", argv[0]);
      exit(-1);
   }

   font.loadFromFile("Arial.ttf");
   sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fast Fourier Transform");

   sprintf(frequencyInfoBuffer, "Number of Frequencies: %d\tFrequency Spacing: %6.3lf Hz\nBottom Frequency: %6.3lf Hz\tTop Frequency: %6.3lf Hz", NUMBER_OF_FREQUENCIES, FREQUENCY_SPACING, FREQUENCY_SPACING * 1, FREQUENCY_SPACING * NUMBER_OF_FREQUENCIES);
   frequencyInfoText = makeText(frequencyInfoBuffer, &font, 0, 0);

   sprintf(maxInfoBuffer, "Max Frequency: %6.3lf Hz\tMax Amplitude: %6.3lf", maxAmplitudeIndex * FREQUENCY_SPACING, maxAmplitude);
   maxInfoText = makeText(maxInfoBuffer, &font, 0, TEXT_LINE_HEIGHT * 2);

   sprintf(bottomFrequencyBuffer, "%6.3lf Hz", FREQUENCY_SPACING * 1);
   bottomFrequencyText = makeText(bottomFrequencyBuffer, &font, 0, HEIGHT - TEXT_LINE_HEIGHT);

   sprintf(middleFrequencyBuffer, "%6.3lf Hz", FREQUENCY_SPACING * NUMBER_OF_FREQUENCIES / 2);
   middleFrequencyText = makeText(middleFrequencyBuffer, &font, WIDTH / 2 - 50, HEIGHT - TEXT_LINE_HEIGHT);

   sprintf(topFrequencyBuffer, "%6.3lf Hz", FREQUENCY_SPACING * NUMBER_OF_FREQUENCIES);
   topFrequencyText = makeText(topFrequencyBuffer, &font, WIDTH - 100, HEIGHT - TEXT_LINE_HEIGHT);

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
         window.draw(makeBar(i, frequencyAmplitudes[i] / fmax(1.0, maxAmplitude)));
      }

      sprintf(maxInfoBuffer, "Max Frequency: %6.3lf\tMax Amplitude: %6.3lf", maxAmplitudeIndex * FREQUENCY_SPACING, maxAmplitude);
      maxInfoText.setString(maxInfoBuffer);

      window.draw(frequencyInfoText);
      window.draw(maxInfoText);
      window.draw(bottomFrequencyText);
      window.draw(middleFrequencyText);
      window.draw(topFrequencyText);

      window.display();

      // To slow it down if necessary
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

sf::Text makeText(char *textBuffer, sf::Font *font, int x, int y) {
   sf::Text text(textBuffer, *font);
   text.setCharacterSize(16);
   text.setColor(sf::Color::Black);
   text.setPosition(x, y);

   return text;
}

