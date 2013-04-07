

#ifndef _PIMAGE_H
#define _PIMAGE_H

#include <SD.h>

class Adafruit_GFX;

/// This class mimics Processing's PImage, but with fewer
/// capabilities. It allows an image stored in the SD card to be
/// drawn to the display.
class PImage {
public:
  PImage() : valid(false) { }
  
  void draw(Adafruit_GFX & glcd, int16_t x, int16_t y);
  
  static PImage loadImage(const char * fileName);
  
  
  bool isValid() { return valid; }
  
private:
  PImage(File _fileRef) : fileRef(_fileRef) { /* TODO read bitmap header */}
  File fileRef;
  bool valid;
};

#endif // _PIMAGE_H
