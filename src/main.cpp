#include <BluetoothSerial.h>
#include <Arduino.h>
#include <NeoPixelBus.h>

const uint8_t PIXELNUM = 24; // this example assumes 4 strips, making it smaller will cause a failure
const uint8_t PIXELPIN = 12; // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128

// three element strips, in different order and speeds
NeoPixelBus<NeoGrbFeature, NeoEsp32BitBang800KbpsMethod> strip(PIXELNUM, PIXELPIN);

BluetoothSerial SerialBT; //define SerialBT, referance to library
uint8_t dataS[3];         //uint = byte

void rainbow(uint8_t wait);
RgbColor Wheel(uint8_t WheelPos);
void stopRainbow();
void rgbManager();

bool rainbowFlag = false;
RgbColor color;
uint8_t pos;

void setup()
{
    Serial.begin(115200); //speed of serial
    SerialBT.begin("Eliot");
    strip.Begin();
    // rainbowFlag = true; ___ Begin rainbow
}

void loop()
{
  rgbManager();

    if (rainbowFlag)
    {
        rainbow(10); //speed
    }
}

void stopRainbow(){
      RgbColor myColor(0,0,0); //define data RGB
            rainbowFlag = false; 
            for (int i = 0; i < PIXELNUM; i++)
            {
                strip.SetPixelColor(i, myColor);
               
            }
             strip.Show();
                delay(5);
              
}

void rgbManager(){
      if (SerialBT.available())
    {
        stopRainbow();

        for (int i = 0; i < 3; i++)
        {
            dataS[i] = SerialBT.read();
            Serial.println(dataS[i]);
        }
        if (1)
        {
            RgbColor myColor(dataS[0], dataS[1], dataS[2]); //define data RGB

            for (int i = 0; i < PIXELNUM; i++)
            {
                strip.SetPixelColor(i, myColor);
                strip.Show();
                delay(5);
            }
        }
    }

     if (Serial.available() > 4)
    {

        String command = Serial.readString();
        if (command == "rainbow\n")
        {
                Serial.println("rainbow enabled");
            rainbowFlag = true;
        } else {
            Serial.println(command);
        }
    }else if (Serial.available() < 4 && Serial.available() > 1)
    {
       stopRainbow();

        for (int i = 0; i < 3; i++)
        {
            dataS[i] = Serial.read();
            Serial.println(dataS[i]);
        }
        if (1)
        {
            RgbColor myColor(dataS[0], dataS[1], dataS[2]); //define data RGB

            for (int i = 0; i < PIXELNUM; i++)
            {
                strip.SetPixelColor(i, myColor);
                strip.Show();
                delay(5);
            }
        }
    }
}

void rainbow(uint8_t wait)
{
    for (uint16_t j = 0; j < 256 ; j++) // complete 5 cycles around the color wheel
    {
       
        for (uint16_t i = 0; i < PIXELNUM; i++)
        {
            // generate a value between 0~255 according to the position of the pixel
            // along the strip
            pos = ((i * 256 / PIXELNUM) + j) & 0xFF;
            // calculate the color for the ith pixel
            color = Wheel(pos);
            // set the color of the ith pixel
            strip.SetPixelColor(i, color);
      
        }
        strip.Show();
        // strip.Darken(100);
        delay(wait);
        
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
RgbColor Wheel(uint8_t WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}