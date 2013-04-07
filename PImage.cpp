

#include "PImage.h"


// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}


PImage PImage::loadImage(const char * fileName) {
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  bool     flip    = true;        // BMP is stored bottom-to-top


  // Open requested file on SD card
  if ((bmpFile = SD.open(fileName)) == NULL) {
    Serial.print("loadImage: file not found: ");
    Serial.println(fileName);
    return PImage(); // load error
  }
  
  
  
  // Parse BMP header
  if(read16(bmpFile) != 0x4D42) { // BMP signature
    Serial.println("loadImage: file doesn't look like a BMP");
    return PImage();
  }
  
  Serial.print("File size: "); Serial.println(read32(bmpFile));
  (void)read32(bmpFile); // Read & ignore creator bytes
  bmpImageoffset = read32(bmpFile); // Start of image data
  Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
  // Read DIB header
  Serial.print("Header size: "); Serial.println(read32(bmpFile));
  bmpWidth  = read32(bmpFile);
  bmpHeight = read32(bmpFile);
  if(read16(bmpFile) != 1) { // # planes -- must be '1'
    Serial.println("loadImage: invalid n. of planes");
    return PImage();
  }
  
  bmpDepth = read16(bmpFile); // bits per pixel
  Serial.print("Bit Depth: "); Serial.println(bmpDepth);
  if((bmpDepth != 24) || (read32(bmpFile) != 0)) { // 0 = uncompressed {
    Serial.println("loadImage: invalid pixel format");
    return PImage();
  }

  Serial.print("Image size: ");
  Serial.print(bmpWidth);
  Serial.print('x');
  Serial.println(bmpHeight);

  // BMP rows are padded (if needed) to 4-byte boundary
  rowSize = (bmpWidth * 3 + 3) & ~3;

  // If bmpHeight is negative, image is in top-down order.
  // This is not canon but has been observed in the wild.
  if(bmpHeight < 0) {
    bmpHeight = -bmpHeight;
    flip      = false;
  }
  
  return PImage(bmpFile, bmpWidth, bmpHeight, bmpDepth, bmpImageoffset, rowSize, flip);
}
