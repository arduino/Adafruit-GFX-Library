

#ifndef _PIMAGE_H
#define _PIMAGE_H

#include <SD.h>

class Adafruit_GFX;

/// This class mimics Processing's PImage, but with fewer
/// capabilities. It allows an image stored in the SD card to be
/// drawn to the display.
class PImage {
public:
  PImage() : _valid(false) { }
  
  void draw(Adafruit_GFX & glcd, int16_t x, int16_t y);
  
  static PImage loadImage(const char * fileName);
  
  
  bool isValid() { return _valid; }
  
private:
  File _bmpFile;
  int  _bmpWidth, _bmpHeight;   // W+H in pixels
  uint8_t  _bmpDepth;              // Bit depth (currently must be 24)
  uint32_t _bmpImageoffset;        // Start of image data in file
  uint32_t _rowSize;               // Not always = bmpWidth; may have padding
  bool     _flip; 
  
  bool     _valid;
  
  PImage(File & bmpFile, int bmpWidth, int bmpHeight, uint8_t  bmpDepth, uint32_t bmpImageoffset, uint32_t rowSize, bool     flip) : 
  _bmpFile(bmpFile),
  _bmpWidth(bmpWidth),
  _bmpHeight(bmpHeight),
  _bmpDepth(bmpDepth),
  _bmpImageoffset(bmpImageoffset),
  _rowSize(rowSize),
  _flip(flip),
  _valid(true)
  { }

};

#endif // _PIMAGE_H
