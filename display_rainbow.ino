#include <Adafruit_NeoPixel.h>
#include <M5Core2.h>

#define PIN 32        // M5Stack Grobe Pin
#define NUMPIXELS 300 // Enough for 29 LEDs
#define DELAYVAL 10   // Time (in milliseconds) to pause between pixels
// RGB config
int led_color[3][3] = {{10, 0, 0}, {0, 10, 0}, {0, 0, 10}};

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  M5.begin();
  M5.Lcd.print("Hello World!!");
  pixels.begin();
}

void loop()
{
  pixels.clear();

  for (int i = 0; i < NUMPIXELS; i++)
  {
    int rgb = i % 3;
    pixels.setPixelColor(i, pixels.Color(led_color[rgb][0], led_color[rgb][1], led_color[rgb][2]));
    pixels.show();
    delay(DELAYVAL);
  }
}
