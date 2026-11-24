/*
 * TiltScope — 3-Axis Accelerometer (LCD + Serial + MATLAB 3-D plot)
 *
 * Board : MSP-EXP432P401R / MSP-EXP432P4111 LaunchPad + Educational BoosterPack MKII
 *         (Kionix KXTC9-2050 analog MEMS accelerometer)
 * IDE   : Energia
 *
 * Reads X/Y/Z acceleration with the 12-bit ADC, removes the ~mid-scale offset,
 * shows the values on the HX8353E LCD and streams them over serial (9600 baud)
 * in the format "X: <x> Y: <y> Z: <z>" for matlab/accelerometer_3d_plot.m.
 *
 * Display helpers (headerFooter / infoScreen) adapted from the MSP432 LCD
 * framework by Qilin Si & Max Steele.
 */

#include <SPI.h>
#include <LCD_screen.h>
#include <LCD_screen_font.h>
#include <LCD_utilities.h>
#include <Screen_HX8353E.h>
#include <Terminal12e.h>
#include <Terminal6e.h>
#include <Terminal8e.h>

void headerFooter(String header_text = "TiltScope",
                  String footer_text = "3-Axis Monitor",
                  uint16_t color = blueColour);
void infoScreen();

// Accelerometer pins on the BoosterPack MKII
const int AccXPin = 23;
const int AccYPin = 24;
const int AccZPin = 25;

// ADC mid-scale offset (12-bit, ~0 g on each axis)
const int ACC_OFFSET = 2024;

Screen_HX8353E myScreen;

void setup()
{
    Serial.begin(9600);
    analogReadResolution(12);      // 0-4095

    myScreen.begin();
    myScreen.setPenSolid(true);

    infoScreen();
    delay(2000);

    myScreen.clear();
    headerFooter();
}

void loop()
{
    int rX = analogRead(AccXPin);
    int rY = analogRead(AccYPin);
    int rZ = analogRead(AccZPin);

    int x = rX - ACC_OFFSET;
    int y = rY - ACC_OFFSET;
    int z = rZ - ACC_OFFSET;

    // Trailing spaces overwrite leftover digits from the previous value
    myScreen.gText(10, 10, "X-Axis: " + String(x) + "   ");
    myScreen.gText(10, 30, "Y-Axis: " + String(y) + "   ");
    myScreen.gText(10, 50, "Z-Axis: " + String(z) + "   ");

    // Serial frame parsed by MATLAB
    Serial.print("X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.print(y);
    Serial.print(" Z: ");
    Serial.println(z);

    delay(100);   // give MATLAB time to read and plot
}

/* --- Display helpers --- */

void headerFooter(String header_text, String footer_text, uint16_t color)
{
    uint16_t header_x = (myScreen.screenSizeX() - myScreen.fontSizeX() * header_text.length()) / 2;
    uint16_t footer_x = (myScreen.screenSizeX() - myScreen.fontSizeX() * footer_text.length()) / 2;
    myScreen.gText(header_x, 0, header_text, color);
    myScreen.gText(footer_x, myScreen.screenSizeY() - myScreen.fontSizeY() - 1, footer_text, color);
    myScreen.dLine(0, myScreen.fontSizeY() + 2, myScreen.screenSizeX(), 1, color);
    myScreen.dLine(0, myScreen.screenSizeY() - myScreen.fontSizeY() - 3, myScreen.screenSizeX(), 1, color);
}

void infoScreen()
{
    myScreen.clear(cyanColour);
    String line1 = " TiltScope ";
    String line2 = " 3-Axis Monitor ";
    uint16_t t1_width = myScreen.fontSizeX() * line1.length();
    uint16_t t2_width = myScreen.fontSizeX() * line2.length();
    myScreen.dLine((myScreen.screenSizeX() - t1_width) / 2, 4, t1_width, 1, blueColour);
    myScreen.gText((myScreen.screenSizeX() - t1_width) / 2, 5, line1, whiteColour, blueColour);
    myScreen.gText((myScreen.screenSizeX() - t2_width) / 2, 5 + myScreen.fontSizeY(), line2, whiteColour, blueColour);
}
