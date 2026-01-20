#include <LCD-I2C.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 3

LCD_I2C lcd(0x27, 20, 4); // Default address of most PCF8574 modules, change according
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


void setup() {
    // If you are using more I2C devices using the Wire library use lcd.begin(false)
    // this stop the library(LCD-I2C) from calling Wire.begin()
    Serial.begin(115200);
    lcd.begin(&Wire);
    lcd.display();
    lcd.backlight();
    Wire.setClock(400000);
    Wire.begin(0x25); // Initialize I2C (Slave Mode)
    Wire.onReceive(I2C_RxHandler);

    lcd.print("Wheel of Destiny!!!!"); // You can make spaces using well... spaces

#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

    digitalWrite(SDA, 0);
    digitalWrite(SCL, 0);
    strip.begin();
    strip.setBrightness(50);
    strip.show(); // Initialize all pixels to 'off'
}

byte RxByte[2];

void I2C_RxHandler(int numBytes) {
    int i = 0;
    Serial.println("RxHandler");
    while (Wire.available()) {
        // Read Any Received Data
        RxByte[i] = Wire.read();
        //Serial.println(RxByte[i]+" ");
        i++;
    }
}

void loop() {
    int reading = RxByte[0];

    switch (reading) {
        case 0: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Peppermint");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 1: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Jagermeister     ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 2: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Fireball");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 3: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Scurvy      ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        case 4: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Slushie      ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 5: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Mystery Bag");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 6: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Blackberry  ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 7: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Gin      ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 8: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Amaretto   ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 9: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Margaritta");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 10: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Mini Long Island");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 11: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Blue Curacao");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 12: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Triple Sec   ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 13: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Vodka     ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 14: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Cosby Colada");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 15: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Apple Pucker   ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 16: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Gasoline     ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 17: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Oil Spill  ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 18: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Kamikaze     ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 19: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Liquid Cocaine   ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 20: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Flamin Beaver   ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 21: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Woooooooooooo!!");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 22: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Polar Bear    ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 23: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Red Headed Slut   ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 24: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Washington Apple   ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 25: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Lemon Drop    ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 26: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Buddy Picks   ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 27: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("WE Pick     ");
            theaterChase(strip.Color(0, 0, 127), 50); // Blue
        }
        break;
        case 28: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Random            ");
            theaterChase(strip.Color(0, 0, 127), 500); // Blue
        }
        break;
        case 29: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("You Pick    ");
            theaterChase(strip.Color(0, 0, 127), 500); // Blue
        }
        break;
        case 30: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("3Way Dirty Pirate");
            theaterChase(strip.Color(0, 0, 127), 500); // Blue
        }
        break;
        case 50: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Destiny awaits!  ");
            colorWipe(strip.Color(0, 255, 0), 50); // Green
        }
        break;
        case 51: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Calculating!!!!!!!");
            colorWipe(strip.Color(0, 255, 0), 50); // Green
            colorWipe(strip.Color(0, 0, 255), 50); // Blue
        }
        break;
        case 52: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("SPINNING  !!!!!!!!");
            rainbow(20);
            rainbowCycle(20);
            theaterChaseRainbow(50);
        }
        break;
        case 53: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("NO CUP  !!!!!!!!");
            colorWipe(strip.Color(255, 0, 0), 50); // Red
        }
        break;
        case 54: {
            lcd.setCursor(4, 2); // Or setting the cursor in the desired position.
            lcd.print("Lee Fucked up:(");
            colorWipe(strip.Color(255, 0, 0), 50); // Red
        }
    }
    delay(100);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait) {
    uint16_t i, j;

    for (j = 0; j < 256; j++) {
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++) {
        // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
    for (int j = 0; j < 10; j++) {
        //do 10 cycles of chasing
        for (int q = 0; q < 3; q++) {
            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
                strip.setPixelColor(i + q, c); //turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
                strip.setPixelColor(i + q, 0); //turn every third pixel off
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
    for (int j = 0; j < 256; j++) {
        // cycle all 256 colors in the wheel
        for (int q = 0; q < 3; q++) {
            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
                strip.setPixelColor(i + q, Wheel((i + j) % 255)); //turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
                strip.setPixelColor(i + q, 0); //turn every third pixel off
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
