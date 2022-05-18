 //
// Animated GIF demo for 01Space 0.42" OLED boards
//
// written by Larry Bank (bitbank@pobox.com)
// 2022-05-12
//
// This sketch provides example code of how to use my AnimatedGIF and OneBitDisplay
// libraries to display a smooth animation on I2C OLED displays
// Normally these displays are driven at 100Khz or 400Khz clock speed, but they are
// capable of running reliably at up to 2Mhz since the same controller chip supports
// an SPI bus speed of even higher than 2Mhz
//
#include <Wire.h>
#include <OneBitDisplay.h>
#include <AnimatedGIF.h>
// Compile the GIF file animation into FLASH with the code
#include "spiral_72x40.h"

ONE_BIT_DISPLAY obd;
AnimatedGIF gif;
static uint8_t *pBuffer; // holds current frame for OLED
#define DISPLAY_WIDTH 72
#define DISPLAY_HEIGHT 40

#ifdef ARDUINO_ARCH_MBED
#define SDA_PIN 22
#define SCL_PIN 23
extern MbedI2C *pWire;
#else
extern TwoWire *pWire;
#define SDA_PIN 5
#define SCL_PIN 6
#endif


//
// This doesn't have to be super efficient
// It will receive one pixel at a time
// from the GIFDraw callback function
//
void DrawPixel(int x, int y, uint8_t ucColor)
{
uint8_t ucMask;
int index;

  if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT)
     return;
  ucMask = 1 << (y & 7);
  index = x + ((y >> 3) * DISPLAY_WIDTH);
  if (ucColor)
     pBuffer[index] |= ucMask; // set the pixel (white)
  else
     pBuffer[index] &= ~ucMask; // clear the pixel (black)
}
//
// Called once per scanline as the GIF is decoded
//
void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    int x, y, iWidth;
    static uint8_t ucPalette[256]; // thresholded palette

    if (pDraw->y == 0) // first line, convert palette to 0/1
    {
      for (x = 0; x < 256; x++)
      {
        uint16_t usColor = pDraw->pPalette[x];
        int gray = (usColor & 0xf800) >> 8; // red
        gray += ((usColor & 0x7e0) >> 2); // plus green*2
        gray += ((usColor & 0x1f) << 3); // plus blue
        ucPalette[x] = (gray >> 9); // 0->511 = 0, 512->1023 = 1
      }
    }
    y = pDraw->iY + pDraw->y; // current line
    iWidth = pDraw->iWidth;
    if (iWidth > DISPLAY_WIDTH)
       iWidth = DISPLAY_WIDTH;

    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
      for (x=0; x<iWidth; x++)
      {
        if (s[x] == pDraw->ucTransparent)
           s[x] = pDraw->ucBackground;
      }
      pDraw->ucHasTransparency = 0;
    }
    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency) // if transparency used
    {
      uint8_t c, ucTransparent = pDraw->ucTransparent;
      int x;
      for(x=0; x < iWidth; x++)
      {
        c = *s++;
        if (c != ucTransparent)
             DrawPixel(pDraw->iX + x, y, ucPalette[c]);
      }
    }
    else
    {
      s = pDraw->pPixels;
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      for (x=0; x<pDraw->iWidth; x++)
        DrawPixel(pDraw->iX + x, y, ucPalette[*s++]);
    }
    if (pDraw->y == pDraw->iHeight-1) // last line, render it to the display
       obd.display();
} /* GIFDraw() */

void setup() {

  obd.setI2CPins(SDA_PIN, SCL_PIN);
  obd.I2Cbegin(OLED_72x40);
  obd.allocBuffer();
  pBuffer = (uint8_t *)obd.getBuffer();
  obd.fillScreen(0);
  obd.setFont(FONT_8x8);
  //obd.setContrast(40);
  obd.print("GIF Demo");
  obd.display();
  delay(2000);
  gif.begin(LITTLE_ENDIAN_PIXELS);
} /* setup() */

void loop() {
  // Loop continuously over the animated sequence
  if (gif.open((uint8_t *)spiral_72x40, sizeof(spiral_72x40), GIFDraw)) // play the GIF from FLASH storage
  {
//    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    while (gif.playFrame(false, NULL))
    {
    }
    gif.close();
  }
} /* loop() */
