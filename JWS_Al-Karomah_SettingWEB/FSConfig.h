// ----------------------
//WEB Server
//ESP8266WebServer server(80);

uint8_t tmputama;
uint8_t tmpjws;
uint8_t tmpinfo;
//uint8_t custom;
//float latitud = -7.1127062165250035;
//float longitud = 113.48328982000642;
//int zonawaktu = 7;
//int hilal = 0;

struct ConfigJws {
  int8_t s_imsak; // menit
  int8_t s_subuh; // menit
  int8_t s_duhur; // menit
  int8_t s_asar; // menit
  int8_t s_magrib; // menit
  int8_t s_isya; // menit
  uint8_t custom;
  uint8_t ihti; // Koreksi Waktu Menit Jadwal Sholat
  float latitude;
  float longitude;
  int8_t zonawaktu;
  uint8_t hilal;
  char namamasjid[512];
  char informasi[1024];
};

const char *fileconfigjws = "/configjws.json";
ConfigJws configjws;

String message, XML;
// -------------------------------------------
// Membuat file config JWS JSON di File Sistem

void membuatDataAwal() {

  String dataawal = "{\"s_imsak\":\"0\",\"s_subuh\":\"0\",\"s_duhur\":\"0\",\"s_asar\":\"0\",\"s_magrib\":\"0\",\"s_isya\":\"0\",\"custom\":\"1\",\"ihti\":\"2\",\"latitude\":\"-7.1127062165250035\",\"longitude\":\"113.48328982000642\",\"zonawaktu\":\"7\",\"hilal\":\"0\",\"namamasjid\":\"SELAMAT DATANG DI MASJID AL-KAROMAH LARANGAN BADUNG PALENGA'AN PAMEKASAN\",\"informasi\":\"PENGASUH K.H. MAWARDI S.Ag M.M.\"}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawal);

  File configFileJws = LittleFS.open(fileconfigjws, "w");

  if (!configFileJws) {
    Serial.println("Gagal membuat file configjws.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }

  serializeJson(doc, configFileJws);

  if (error) {

    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;

  } else {

    configFileJws.close();
    Serial.println("Berhasil membuat file configjws.json");

  }

}



// -------------------------------------------
// Membaca file config JWS JSON di File Sistem

void loadJwsConfig(const char *fileconfigjws, ConfigJws &configjws) {

  File configFileJws = LittleFS.open(fileconfigjws, "r");

  if (!configFileJws) {

    Serial.println("Gagal membuka file configjws.json untuk dibaca");
    membuatDataAwal();
    Serial.println("Sistem restart...");
    ESP.restart();

  }

  size_t size = configFileJws.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFileJws.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("Gagal parse fileconfigjws");
    return;
  }
  
  configjws.s_imsak = doc["s_imsak"];
  configjws.s_subuh = doc["s_subuh"];
  configjws.s_duhur = doc["s_duhur"];
  configjws.s_asar = doc["s_asar"];
  configjws.s_magrib = doc["s_magrib"];
  configjws.s_isya = doc["s_isya"];
  configjws.custom = doc["custom"];
  configjws.ihti = doc["ihti"];
  configjws.latitude = doc["latitude"];
  configjws.longitude = doc["longitude"];
  configjws.zonawaktu = doc["zonawaktu"];
  configjws.hilal = doc["hilal"];
  strlcpy(configjws.namamasjid, doc["namamasjid"] | "", sizeof(configjws.namamasjid));
  strlcpy(configjws.informasi, doc["informasi"] | "", sizeof(configjws.informasi));

  configFileJws.close();

}


// -------------------------------------------
// MEMBACA PARAMETER YANG TERSIMPAN

void bacaParameter() {

  Serial.println(" ");
  Serial.println("PARAMETER TERSIMPAN");
  Serial.print("Selisih Imsak   : "); Serial.println(configjws.s_imsak);
  Serial.print("Selisih Subuh   : "); Serial.println(configjws.s_subuh);
  Serial.print("Selisih Dzuhur  : "); Serial.println(configjws.s_duhur);
  Serial.print("Selisih Ashar   : "); Serial.println(configjws.s_asar);
  Serial.print("Selisih Maghrib : "); Serial.println(configjws.s_magrib);
  Serial.print("Selisih Isya    : "); Serial.println(configjws.s_isya);
  Serial.print("Customisasi   : "); Serial.println(configjws.custom);
  Serial.print("Ihtiyati       : "); Serial.println(configjws.ihti);
  Serial.print("Latitude       : "); Serial.println(configjws.latitude);
  Serial.print("Longitude      : "); Serial.println(configjws.longitude);
  Serial.print("Zona Waktu     : "); Serial.println(configjws.zonawaktu);
  Serial.print("Derajat Hilal  : "); Serial.println(configjws.hilal);
  Serial.print("Nama Masjid    : "); Serial.println(configjws.namamasjid);
  Serial.print("informasi      : "); Serial.println(configjws.informasi);
  Serial.println(" ");

}




// PENGHITUNG JADWAL SHOLAT

void JadwalSholat() {

  /*
    CALCULATION METHOD
    ------------------
    Jafari,   // Ithna Ashari
    Karachi,  // University of Islamic Sciences, Karachi
    ISNA,     // Islamic Society of North America (ISNA)
    MWL,      // Muslim World League (MWL)
    Makkah,   // Umm al-Qura, Makkah
    Egypt,    // Egyptian General Authority of Survey
    Custom,   // Custom Setting

    JURISTIC
    --------
    Shafii,    // Shafii (standard)
    Hanafi,    // Hanafi

    ADJUSTING METHOD
    ----------------
    None,        // No adjustment
    MidNight,   // middle of night
    OneSeventh, // 1/7th of night
    AngleBased, // angle/60th of night

    TIME IDS
    --------
    Fajr,
    Sunrise,
    Dhuhr,
    Asr,
    Sunset,
    Maghrib,
    Isha

  */

  set_calc_method(Karachi);
  set_asr_method(Shafii);
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);
  set_isha_angle(18);

  get_prayer_times(rTah, rBul, rTgl, configjws.latitude, configjws.longitude, configjws.zonawaktu, times);

}



// ------------------------------
// UPDATE HISAB TANGGAL

F1kM_Hisab HisabTanggal;

int Hjr_Date, Hjr_Month, Hjr_Year;

void TanggalHijriah() {

  HisabTanggal.setLocationOnEarth(configjws.latitude, configjws.longitude); //Latitude Longitude Al-Karomah
  HisabTanggal.setHilalVisibilityFactor(configjws.hilal);
  HisabTanggal.Greg2HijrDate(rTgl, rBul, rTah, Hjr_Date, Hjr_Month, Hjr_Year);

}



//-----------------------------------
// UPDATE WAKTU

void UpdateWaktu() {

  //  server.handleClient();
  static long pM;
  static uint16_t d;


  if (millis() - pM > 1000) {
    d++;
    if (d < 2) {
      BacaRTC();
      JadwalSholat();
      TanggalHijriah();
    } else if (d < 60) { // Update setiap 60 detik
      rDet++;
      if (rDet > 59) {
        rMen++;
        rDet = 0;
        if (rMen > 59) {
          rJam++;
          rMen = 0;
        }
      }
    } else {
      d = 0;
    }
    pM = millis();
  }

}

// XML UNTUK JEMBATAN DATA MESIN DENGAN WEB

void XMLWaktu() {

  XML = "<?xml version='1.0'?>";
  XML += "<t>";

  XML += "<Tahun>";
  XML += rTah;
  XML += "</Tahun>";
  XML += "<Bulan>";
  XML += rBul;
  XML += "</Bulan>";
  XML += "<Tanggal>";
  XML += rTgl;
  XML += "</Tanggal>";
  XML += "<Jam>";
  if (rJam < 10) {
    XML += "0";
    XML += rJam;
  } else {
    XML += rJam;
  }
  XML += "</Jam>";
  XML += "<Menit>";
  if (rMen < 10) {
    XML += "0";
    XML += rMen;
  } else {
    XML += rMen;
  }
  XML += "</Menit>";
  XML += "<Detik>";
  if (rDet < 10) {
    XML += "0";
    XML += rDet;
  } else {
    XML += rDet;
  }
  XML += "</Detik>";
  XML += "<Suhu>";
  XML += celsius - 5;
  XML += "</Suhu>";

  XML += "</t>";
}


void XMLDataJWS() {

  XML = "<?xml version='1.0'?>";
  XML += "<t>";
  
  XML += "<SelisihImsak>";
  XML += configjws.s_imsak;
  XML += "</SelisihImsak>";
  XML += "<SelisihSubuh>";
  XML += configjws.s_subuh;
  XML += "</SelisihSubuh>";
  XML += "<SelisihDuhur>";
  XML += configjws.s_duhur;
  XML += "</SelisihDuhur>";
  XML += "<SelisihAsar>";
  XML += configjws.s_asar;
  XML += "</SelisihAsar>";
  XML += "<SelisihMagrib>";
  XML += configjws.s_magrib;
  XML += "</SelisihMagrib>";
  XML += "<SelisihIsya>";
  XML += configjws.s_isya;
  XML += "</SelisihIsya>";
  XML += "<Custom>";
  XML += configjws.custom;
  XML += "</Custom>";
  XML += "<Ihtiyati>";
  XML += configjws.ihti;
  XML += "</Ihtiyati>";
  XML += "<Latitude>";
  XML += configjws.latitude;
  XML += "</Latitude>";
  XML += "<Longitude>";
  XML += configjws.longitude;
  XML += "</Longitude>";
  XML += "<ZonaWaktu>";
  XML += configjws.zonawaktu;
  XML += "</ZonaWaktu>";
  XML += "<Hilal>";
  XML += configjws.hilal;
  XML += "</Hilal>";
  XML += "<NamaMasjid>";
  XML += configjws.namamasjid;
  XML += "</NamaMasjid>";
  XML += "<Informasi>";
  XML += configjws.informasi;
  XML += "</Informasi>";

  XML += "</t>";
}

// XML UNTUK JEMBATAN DATA MESIN DENGAN WEB

void buildXML() {

  RtcDateTime now = Rtc.GetDateTime();
  RtcTemperature temp = Rtc.GetTemperature();
  XML = "<?xml version='1.0'?>";
  XML += "<t>";
  XML += "<rYear>";
  XML += now.Year();
  XML += "</rYear>";
  XML += "<rMonth>";
  XML += now.Month();
  XML += "</rMonth>";
  XML += "<rDay>";
  XML += now.Day();
  XML += "</rDay>";
  XML += "<rHour>";
  if (now.Hour() < 10) {
    XML += "0";
    XML += now.Hour();
  } else {
    XML += now.Hour();
  }
  XML += "</rHour>";
  XML += "<rMinute>";
  if (now.Minute() < 10) {
    XML += "0";
    XML += now.Minute();
  } else {
    XML += now.Minute();
  }
  XML += "</rMinute>";
  XML += "<rSecond>";
  if (now.Second() < 10) {
    XML += "0";
    XML += now.Second();
  } else {
    XML += now.Second();
  }
  XML += "</rSecond>";
  XML += "<rTemp>";
  XML += temp.AsFloatDegC();
  XML += "</rTemp>";
  XML += "</t>";
}
