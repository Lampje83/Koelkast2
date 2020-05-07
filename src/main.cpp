#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include "DHT12.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

DHT12 dht12;
Adafruit_BMP280 bme;

U8G2_SH1107_64X128_F_4W_HW_SPI u8g2 (U8G2_R1,/* cs=*/ 14, /* dc=*/ 27, /* reset=*/ 33);
U8X8_SH1107_64X128_4W_HW_SPI u8x8 (/* cs=*/ 14, /* dc=*/ 27, /* reset=*/ 33);
// i2c: 13, 25
bool status;

void setup ()
{
	u8g2.begin ();
	u8g2.setContrast (5);
	Wire.begin (25, 13);
	status = bme.begin (0x76);
}

void loop ()
{
	float tmp, rh, pressure;
	char buffer[16];
	u8g2.firstPage ();
	do {
		tmp = dht12.readTemperature ();

		sprintf (buffer, "%4.1f°C", tmp);
		u8g2.setFont (u8g2_font_courB24_tf);
		u8g2.drawStr (0, 24, buffer);

		rh = dht12.readHumidity ();

		sprintf (buffer, "%4.1f%%", rh);
		u8g2.setFont (u8g2_font_5x8_tf);
		u8g2.drawStr (0, 32, buffer);
		
		if (status) {
			pressure = bme.readPressure () / 100.0f;
			
			sprintf (buffer, "%6.1f hPa", pressure);
			u8g2.setFont (u8g2_font_5x8_tf);
			u8g2.drawStr (48, 32, buffer);

			tmp = bme.readTemperature ();
			sprintf (buffer, "%4.1f°C", tmp);
			u8g2.drawStr (48, 40, buffer);
		}

	} while ( u8g2.nextPage () );

	//u8x8.drawString (0, 0, "Hallo");
	delay (1000);
}