

#include "PImage.h"


PImage PImage::loadImage(const char * fileName) {
  File fileRef;
  
  // Open requested file on SD card
  if ((fileRef = SD.open(fileName)) == NULL) {
    Serial.print("PImage::loadImage: file not found");
    return PImage();
  }
  
  return PImage(fileRef);
}
