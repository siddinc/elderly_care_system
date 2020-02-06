#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1);
float t=1;

void setup()   
{                
  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  display.setTextSize(t);
  display.setTextColor(WHITE);
  
  // Clear the buffer.
  display.clearDisplay();
}

void loop()
{
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  
  display.setCursor(0*t,0*t);
  display.println("Time : 10:00");
  display.display();
  
  display.setCursor(0*t,9*t);
  display.println("Date : 05/02/2020");
  display.display();
  
  display.setCursor(0*t,19*t);
  display.println("Location : Mumbai");
  display.display();
  
  display.setCursor(0*t,28*t);
  display.println("BPM : 90");
  
  display.setCursor(55*t,28*t);
  display.write(3);
  display.display();
}
