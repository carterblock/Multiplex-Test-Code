/**
 * TCA9548 I2CScanner.pde -- I2C bus scanner for Arduino
 *
 * Based on code c. 2009, Tod E. Kurt, http://todbot.com/blog/
 *
 */

#include "Wire.h"
#include "Adafruit_VL6180X.h"
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}


Adafruit_VL6180X vl = Adafruit_VL6180X();

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 3) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i); //Bit shift
  Wire.endTransmission();  
}


// standard Arduino setup()
void setup()
{
    while (!Serial);
    delay(1000);

    Wire.begin();
    
    Serial.begin(115200);
    Serial.println("\nTCAScanner ready!");
    
    for (uint8_t t=0; t<4; t++) {
      tcaselect(t);
      Serial.print("TCA Port #"); Serial.println(t);

      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;
      
        uint8_t data;
        if (! twi_writeTo(addr, &data, 0, 1, 1)) {
           Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
        }
      }
    }
    Serial.println("\ndone");
    vl.begin();
}

void loop() 
{
  Serial.println("Loop es running");
  tcaselect(2);
  vl.begin();

  float lux = vl.readLux(VL6180X_ALS_GAIN_5);

  Serial.print("Lux: ");
  Serial.println(lux);

  uint8_t range = vl.readRange();
  Serial.print("Range: ");
  Serial.println(range);
  
  uint8_t status = vl.readRangeStatus();
  delay(500);
  
}

