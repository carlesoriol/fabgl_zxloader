
#include "fabgl.h"
#include "zxbitmap.h"

fabgl::VGAController VGAController;
fabgl::Canvas        canvas(&VGAController);

#include "FS.h"
#include "SD.h"
#include "SPI.h"

SPIClass myspi(HSPI);

void setup() 
{       
  Serial.begin(115200);
  
  VGAController.begin();
  VGAController.setResolution(VGA_320x200_75Hz, 320, 200); 

  canvas.setBrushColor(RGB888(0, 0, 0)); // Use as BORDER color
  canvas.selectFont(&fabgl::FONT_4x6);
  canvas.setPenColor( RGB888(0x40, 0x40, 0x40) );
  canvas.clear();
  canvas.drawText(32, 200-6, "ZX Spectrum Slideshow - Insert SD - by Carles Oriol 2020");
  canvas.waitCompletion();

  SPIClass myspi(HSPI);  
  myspi.begin(14,2,12,13); //CLK,MISO,MOIS,SS
    // if sd not ready retry every 2 seconds
  while(!SD.begin( 13, myspi)) { delay(2000); } 
  while(SD.cardType() == CARD_NONE){ delay(2000); }

  static uint8_t buf[6912];

  while( true )
  {
    File root = SD.open("/scr");
    File file = root.openNextFile();
    while(file) {
  
      if(!file.isDirectory()) {
          file.read( buf, 6912);
          ZXBitmap zxscreen(buf);
          
          canvas.clear();
          canvas.drawBitmap( 32, 0, &zxscreen );
          canvas.drawText(32, 200-6, file.name());
           
          canvas.waitCompletion();
          delay(2000);
      }
      file = root.openNextFile();
    }
  }
}



void loop()
{
}
