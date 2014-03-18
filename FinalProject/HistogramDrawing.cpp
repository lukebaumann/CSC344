#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600
#define BOTTOM_OF_FREQUENCY_BARS 590
#define TOP_OF_FREQUENCY_BARS 10
#define LEFT_MOST_FREQUENCY_BAR 10
#define RIGHT_MOST_FREQUENCY_BAR 790
#define WINDOW_SIZE 128

void getFrequencyAmplitudes(double *frequencyAmplitudes, int frequencyAmplitudesSize);
double getMaxAmplitude(double *frequencyAmplitudes, int frequencyAmplitudesSize);
sf::VertexArray makeBar(int frequencyIndex, double normalizedAmplitude);

const static int FREQUENCY_BAR_WIDTH = LEFT_MOST_FREQUENCY_BAR + (RIGHT_MOST_FREQUENCY_BAR - LEFT_MOST_FREQUENCY_BAR) / WINDOW_SIZE * 2 - WIDTH + RIGHT_MOST_FREQUENCY_BAR;
int main() {
   double frequencyAmplitudes[WINDOW_SIZE];
   double maxAmplitude = 0.0;
   double runningAverageMaxAmplitude = 0.0;

   sf::Font font;
   font.loadFromFile("Arial.ttf");
   char maxAmplitudeBuffer[30];

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
      maxAmplitude = getMaxAmplitude(frequencyAmplitudes, WINDOW_SIZE); 
      runningAverageMaxAmplitude = runningAverageMaxAmplitude * 0.8 + maxAmplitude * 0.2;

      if (maxAmplitude < runningAverageMaxAmplitude) {
         maxAmplitude = runningAverageMaxAmplitude;
      }

      for (int i = 0; i < WINDOW_SIZE / 2; i++) {
         window.draw(makeBar(i, frequencyAmplitudes[i] / maxAmplitude));
      }

      sprintf(maxAmplitudeBuffer, "Max: %10.3lf", maxAmplitude);
      sf::Text text(maxAmplitudeBuffer, font);
      text.setCharacterSize(16);
      text.setColor(sf::Color::Black);

      window.draw(text);


      window.display();
   }
}

void getFrequencyAmplitudes(double *frequencyAmplitudes, int frequencyAmplitudesSize) {
   for (int i = 0; i < frequencyAmplitudesSize; i++) {
      frequencyAmplitudes[i] = i * 400;
   }
}

double getMaxAmplitude(double *frequencyAmplitudes, int frequencyAmplitudesSize) {
   double maxAmplitude = 0.0;
   for (int i = 0; i < frequencyAmplitudesSize; i++) {
      if (frequencyAmplitudes[i] > maxAmplitude) {
         maxAmplitude = frequencyAmplitudes[i];
      }
   }

   return maxAmplitude;
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


/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_LINE_CHARACTERS 80
#define MAX_NAME_CHARACTERS 10
#define MAX_BIN_VALUE 50

void drawOneBin(char *binName, int binValue);
void erasePreviousLine();
void eraseCurrentLine();

int main() {
   drawOneBin("Cool Bin", 6);
   drawOneBin("Really Sik", 33);
   drawOneBin("10", 3);
   usleep(1000000);
   erasePreviousLine();
   drawOneBin("10", 33);
   usleep(1000000);
   erasePreviousLine();
   usleep(1000000);
   erasePreviousLine();
   drawOneBin("Not Cool", 4);




   
}

void eraseCurrentLine() {
   char spaceCharacterBuffer[MAX_LINE_CHARACTERS];

   int i = 0;
   for (i = 0; i < MAX_LINE_CHARACTERS; i++) {
      sprintf(spaceCharacterBuffer + i, " ");
   }

   printf("\r%s\r", spaceCharacterBuffer);
}

void erasePreviousLine() {
   printf("\r\b");
   eraseCurrentLine();
}

void drawOneBin(char *binName, int binValue) {
   char binCharacterBuffer[MAX_LINE_CHARACTERS];
   int lengthOfName = strlen(binName);
   int characterIndex = lengthOfName;
   
   if (lengthOfName > MAX_NAME_CHARACTERS) {
      fprintf(stderr, "Error: bin name too long: %d should be less than %d\n", lengthOfName, MAX_NAME_CHARACTERS);
      exit(-1);
   }

   if (binValue > MAX_BIN_VALUE) {
      fprintf(stderr, "Error: bin value too high: %d should be less than %d\n", binValue, MAX_NAME_CHARACTERS);
      exit(-1);
   }

   sprintf(binCharacterBuffer, "%s", binName);

   int i = 0;
   for (i = 0; i + lengthOfName < MAX_NAME_CHARACTERS; i++) {
      sprintf(binCharacterBuffer + characterIndex++, " ");
   }

   sprintf(binCharacterBuffer + characterIndex, ": ");
   characterIndex += 2;

   for (i = 0; i < binValue; i++) {
      sprintf(binCharacterBuffer + characterIndex++, "*");
   }
   
   printf("%s\n", binCharacterBuffer);
}
*/