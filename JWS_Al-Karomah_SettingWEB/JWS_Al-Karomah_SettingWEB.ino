/*
  Software serial multple serial test
  The circuit:
   RX is digital pin 2 (connect to TX of other device)
   TX is digital pin 3 (connect to RX of other device)

   JADWAL WAKTU SHOLAT MENGGUNAKAN NODEMCU ESP8266, LED 8x10x8, RTC DS3231
   FITUR :  JADWAL SHOLAT 5 WAKTU DAN TANBIH IMSAK, JAM BESAR, TANGGAL, SUHU, ALARAM ADZAN DAN TANBIH IMSAK,
            DAN HITUNG MUNDUR IQOMAH DAN UBAH WAKTU LEWAT WIFI DENGAN BROWSER.


  Pin on  NODEMCU       Pin on  DS3231      NODEMCU
        D0                SCL             D1 (GPIO 5)
        D6                SDA             D2 (GPIO 4)
        D5                VCC             3V
        D3                GND             GND
        D7
        D8
        GND

  Eksternal Library
  - DMDESP : https://github.com/busel7/DMDESP
  - PrayerTime : https://github.com/asmaklad/Arduino-Prayer-Times
  - RTC DS3231 : https://github.com/Makuna/Rtc
  - ArduinoJson V6 : https://github.com/bblanchon/ArduinoJson
  - F1kM_Hisab : https://github.com/wardi1971/F1kM_Hisab

  Diskusi Grup Untuk1000Masjid : https://www.facebook.com/groups/761058907424496/

  Tools :
  - Desain bitmap dan font : http://dotmatrixtool.com
  - LittleFS Uploader : https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases


  Updated : 19 Desember 2021
*/
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

#include <F1kM_Hisab.h>
#include <PrayerTimes.h>
#include "RTC.h"
#include "FSConfig.h"
#include "WebServer.h"
#include "WebPage.h"
#include "JWS.h"

void tampiljws();
void tampilinfo();
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// SETUP
void setup() {
  //  custom = 1;
  // put your setup code here, to run once:
  Serial.begin(4800);
  // set the data rate for the Serial JWS port
  Serial1.begin(4800);

  //Mulai RTC
  mulaiRTC();

  //FILE SYSTEM
  LittleFS.begin();
  loadJwsConfig(fileconfigjws, configjws);

  //WIFI
  wifiConnect();

  server.on("/", []() {
    server.send_P(200, "text/html", setwaktu);

    // Kalau ada perubahan tanggal
    if (server.hasArg("date")) {

      uint16_t jam;
      uint8_t menit;
      uint8_t detik;
      String sd = server.arg("date");
      String lastSd;

      jam = ((sd[0] - '0') * 1000) + ((sd[1] - '0') * 100) + ((sd[2] - '0') * 10) + (sd[3] - '0');
      menit = ((sd[5] - '0') * 10) + (sd[6] - '0');
      detik = ((sd[8] - '0') * 10) + (sd[9] - '0');

      if (sd != lastSd) {
        RtcDateTime now = Rtc.GetDateTime();
        uint8_t hour = now.Hour();
        uint8_t minute = now.Minute();
        Rtc.SetDateTime(RtcDateTime(jam, menit, detik, hour, minute, 0));
        lastSd = sd;
      }
      kiriScroll("DONE");
      BacaRTC();

      server.send ( 404 , "text", message );

    }

    // Kalau ada perubahaan jam
    if (server.hasArg("time")) {

      String st = server.arg("time");
      String lastSt;
      uint8_t jam = ((st[0] - '0') * 10) + (st[1] - '0');
      uint8_t menit = ((st[3] - '0') * 10) + (st[4] - '0');

      if (st != lastSt) {
        RtcDateTime now = Rtc.GetDateTime();
        uint16_t year = now.Year();
        uint8_t month = now.Month();
        uint8_t day = now.Day();
        Rtc.SetDateTime(RtcDateTime(year, month, day, jam, menit, 0));
        lastSt = st;
      }
      kiriScroll("DONE");
      BacaRTC();

      server.send ( 404 , "text", message );

    }
  });

  server.on("/simpandatajws", HTTP_POST, handleSettingJwsUpdate);

  server.on("/xmlwaktu", handleXMLWaktu); // http://192.168.4.1/xmlwaktu
  server.on("/xmldatajws", handleXMLDataJWS); // http://192.168.4.1/xmldatajws

  httpUpdater.setup(&server);
  server.begin();
  Serial.println("HTTP server started");

  // Tampilkan Parameter yang tersimpan
  bacaParameter();

  UpdateWaktu();
  //Manual Tahun, Bulan, Tanggal, Jam, Menit, Detik
  //    Rtc.SetDateTime(RtcDateTime(2021, 12, 14, 03, 30, 50));

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  switch (tmputama) {
    case 0 :
      tampiljws();
      server.handleClient();
      break;

    case 1 :
      TampilSaatSholat();
      break;

  }
  UpdateWaktu();
  AlarmSholat();

}
void tampiljws() {
  server.handleClient();
  switch (tmpjws) {

    case 0 :
      TeksBerjalanKananKiri();
      //      custom_tulisan();
      break;
    case 1 :
      custom_tulisan();
      break;
    case 2:
      TampilJadwalSholatCepat();
      break;
    case 3 :
      TampilJamKecil();
      tampilinfo();
      break;

  }

}
void tampilinfo() {
  server.handleClient();
  switch (tmpinfo) {

    case 0 :
      TampilTanggal();
      break;

    case 1 :
      TampilTanggalHijr();
      break;

    case 2 :
      TampilSuhu();
      break;

    case 3 :
      TampilJadwalSholat();
      break;

  }
}
