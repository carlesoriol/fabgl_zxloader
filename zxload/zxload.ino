
#include "fabgl.h"

fabgl::VGAController VGAController;
fabgl::Canvas        canvas(&VGAController);

#include "KnightLore.scr.h"

// RGBA2222,   /**< 8 bit per pixel: AABBGGRR (bit 7=A 6=A 5=B 4=B 3=G 2=G 1=R 0=R). 
// AA = 0 fully transparent, AA = 3 fully opaque. 
// Each color channel can have values from 0 to 3 (maxmum intensity). */

const uint8_t zxpalette[2][8] = //with and without bright 2 bit pairs
{
  { 0b11000000, 0b11100000, 0b11000010, 0b11100010, 0b11001000, 0b11101000, 0b11001010, 0b11101010 }, // 0 no bright
  { 0b11000000, 0b11110000, 0b11000011, 0b11100011, 0b11001100, 0b11111100, 0b11001111, 0b11111111 }  // 1 bright
};

void setup() 
{     
  Serial.begin(115200);

  VGAController.begin();
  VGAController.setResolution(VGA_320x200_75Hz, 320, 200); 

  canvas.setBrushColor(RGB888(0, 0, 0));
  canvas.clear();
  canvas.waitCompletion();

  Bitmap zxscreen = Bitmap(256, 192, NULL, PixelFormat::RGBA2222, RGB888(0, 0, 0));
  zxscreen.width = 256;
  zxscreen.height = 192;
  //zxscreen.allocate(); why protected? There's no empty contructor!!
  zxscreen.data = (uint8_t*) malloc(256*192);
  zxscreen.dataAllocated = true;

  for (int y=0; y< 192; y++)    
    for (int x=0; x< 32; x++)
    {     
      uint8_t bytevalue = KnightLore_data[((y/64)*64 + (y%8)*8 + (y/8)%8) * 32 + x];
      uint8_t byteattrib = KnightLore_data[(y/8)*32 + x + 6144];
      for(int bitp = 0; bitp < 8; bitp++)
          zxscreen.data[y*256+x*8+bitp] = zxpalette [((byteattrib & 0b1000000)>>6)&1]
                                                    [ (byteattrib >> ((bytevalue & (0b10000000 >> bitp)) ? 0 : 3)) & 0b111];
    }

  canvas.drawBitmap( 32, 4, &zxscreen );
  canvas.waitCompletion();
}

void loop()
{
}
