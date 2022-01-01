//WEB Server
//ESP8266WebServer server(80);

//int configjws.ihti = 2;


// Header kirim data
byte head = 0xA5;
byte target[] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};
void bersihkan();
void kiriScroll();
void kiriKanan();
void kiriScrollTulisan();
void Jam_adzan();
//FORMAT PENULISAN TEKS
void bersihkan() {
  String kata = {"\n\n\n\n\n\n\n\n"};
  for (int i = 0; i < 8; i = i + 1) {
    Serial1.write(head);
    Serial1.write(target[i]);
    Serial1.print(kata);
  }
}
void kiriKanan(String kata) {

  int a = 0;
  for (int i = 0; i < kata.length();) {
    Serial1.write(0xA5);
    Serial1.write(target[a]);
    Serial1.print(kata[i]);
    a += 1;
    i += 1;
    if (a >= 8 ) {
      a = 0;
    }
    //    delay(50);
  }
}
void kiriScroll(String data) {

  Serial.println(data);
  data = data + "\n\n\n\n\n\n\n\n";
  for (int i = 0; i < data.length();) {
    Serial1.write(0xA5);
    Serial1.write(0x11);
    Serial1.print(data[i - 7]);

    Serial1.write(0xA5);
    Serial1.write(0x12);
    Serial1.print(data[i - 6]);

    Serial1.write(0xA5);
    Serial1.write(0x13);
    Serial1.print(data[i - 5]);

    Serial1.write(0xA5);
    Serial1.write(0x14);
    Serial1.print(data[i - 4]);

    Serial1.write(0xA5);
    Serial1.write(0x15);
    Serial1.print(data[i - 3]);

    Serial1.write(0xA5);
    Serial1.write(0x16);
    Serial1.print(data[i - 2]);

    Serial1.write(0xA5);
    Serial1.write(0x17);
    Serial1.print(data[i - 1]);

    Serial1.write(0xA5);
    Serial1.write(0x18);
    Serial1.print(data[i]);
    delay(350);
    i += 1;
  }
}

void kiriScrollTulisan(String data) {

  Serial.println(data);
  data = data + "\n\n\n\n\n\n\n\n";
  for (int i = 0; i < data.length();) {
    Serial1.write(0xA5);
    Serial1.write(0x11);
    Serial1.print(data[i - 7]);

    Serial1.write(0xA5);
    Serial1.write(0x12);
    Serial1.print(data[i - 6]);

    Serial1.write(0xA5);
    Serial1.write(0x13);
    Serial1.print(data[i - 5]);

    Serial1.write(0xA5);
    Serial1.write(0x14);
    Serial1.print(data[i - 4]);

    Serial1.write(0xA5);
    Serial1.write(0x15);
    Serial1.print(data[i - 3]);

    Serial1.write(0xA5);
    Serial1.write(0x16);
    Serial1.print(data[i - 2]);

    Serial1.write(0xA5);
    Serial1.write(0x17);
    Serial1.print(data[i - 1]);

    Serial1.write(0xA5);
    Serial1.write(0x18);
    Serial1.print(data[i]);
    delay(250);
    i += 1;
  }
}
//----------------------------------------------------------------------
//TAMPILAN RUNNING TEKS

void TeksBerjalanKananKiri() {

  kiriScrollTulisan("BISMILLAH");
  //kiriScroll("SELAMAT DATANG DI MASJID AL-KAROMAH LARANGAN BADUNG PALENGA'AN PAMEKASAN");
  kiriScrollTulisan(configjws.namamasjid);
  //  kiriScroll("PENGASUH K.H. MAWARDI S.Ag M.M.");
  tmpjws = 1;
}
void custom_tulisan() {
  if (configjws.custom != 0) {
    kiriScrollTulisan(configjws.informasi);
    tmpjws = 2;
  }
  else {
    tmpjws = 2;
  }

}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// ALARM SHOLAT BERJALAN SAAT MASUK WAKTU SHOLAT

void AlarmSholat() {

  //  server.handleClient();
  int hours, minutes, seconds;
  RtcDateTime now = Rtc.GetDateTime();

  int Hari = now.DayOfWeek();
  int Hor = now.Hour();
  int Min = now.Minute();
  int Sec = now.Second();

  int durasi = 30000; // 30 Detik atau satu menit
  unsigned long time_now = 0;
  time_now = millis();

  JadwalSholat();

  // Tanbih Imsak
  get_float_time_parts(times[0], hours, minutes);
  minutes = minutes + configjws.ihti;

  if (minutes < 10) {

    minutes = 60 - minutes;
    hours --;

  } else {

    minutes = minutes - 10 ;

  }

  if (configjws.s_imsak != 0) {
    minutes = minutes + configjws.s_imsak;
  }
  if (Hor == hours && Min == minutes) {

    while (millis() < time_now + durasi) {
      kiriScroll("WAKTU IMSAK");
    }
    Jam_adzan();
    tmputama = 0;

  }

  // Subuh
  get_float_time_parts(times[0], hours, minutes);
  minutes = minutes + configjws.ihti;

  if (minutes >= 60) {

    minutes = minutes - 60;
    hours ++;

  }
  if (configjws.s_subuh != 0) {
    minutes = minutes + configjws.s_subuh;
  }
  if (Hor == hours && Min == minutes) {
    while (millis() < time_now + durasi) {
      kiriScroll("ADZAN SUBUH");
    }
    Jam_adzan();
    tmputama = 1;

  }


  // Dzuhur
  get_float_time_parts(times[2], hours, minutes);
  minutes = minutes + configjws.ihti;

  if (minutes >= 60) {

    minutes = minutes - 60;
    hours ++;

  }
  if (configjws.s_duhur != 0) {
    minutes = minutes + configjws.s_duhur;
  }
  if (Hor == hours && Min == minutes && Hari != 5) {
    while (millis() < time_now + durasi) {
      kiriScroll("ADZAN DZUHUR");
    }
    Jam_adzan();
    tmputama = 1;

  } else if (Hor == hours && Min == minutes && Hari == 5) {
    while (millis() < time_now + durasi) {
      kiriScroll("ADZAN JUM'AT");
    }
    Jam_adzan();
    tmputama = 1;

  }


  // Ashar
  get_float_time_parts(times[3], hours, minutes);
  minutes = minutes + configjws.ihti;

  if (minutes >= 60) {

    minutes = minutes - 60;
    hours ++;

  }
  if (configjws.s_asar != 0) {
    minutes = minutes + configjws.s_asar;
  }
  if (Hor == hours && Min == minutes) {
    while (millis() < time_now + durasi) {
      kiriScroll("ADZAN ASHAR");
    }
    Jam_adzan();
    tmputama = 1;

  }

  // Maghrib
  get_float_time_parts(times[5], hours, minutes);
  minutes = minutes + configjws.ihti;

  if (minutes >= 60) {

    minutes = minutes - 60;
    hours ++;

  }
  if (configjws.s_magrib != 0) {
    minutes = minutes + configjws.s_magrib;
  }
  if (Hor == hours && Min == minutes) {
    while (millis() < time_now + durasi) {
      kiriScroll("ADZAN MAGHRIB");
    }
    Jam_adzan();
    tmputama = 1;

  }

  // Isya'
  get_float_time_parts(times[6], hours, minutes);
  minutes = minutes + configjws.ihti;

  if (minutes >= 60) {

    minutes = minutes - 60;
    hours ++;

  }
  if (configjws.s_isya != 0) {
    minutes = minutes + configjws.s_isya;
  }
  if (Hor == hours && Min == minutes) {
    while (millis() < time_now + durasi) {
      kiriScroll("ADZAN ISYA");
    }
    Jam_adzan();
    tmputama = 1;

  }

}

//----------------------------------------------------------------------
// TAMPILAN SAAT SHOLAT

void TampilSaatSholat() {

  static uint32_t pM;
  uint32_t cM = millis();
  static uint32_t durasi = 10; // Detik
  //static uint32_t durasi = 3; // Detik
  static uint32_t det;

  kiriScroll("LURUSKAN DAN RAPATKAN SHAF");

  if (cM - pM >= 1000) {
    pM = cM;
    det++;

    if (durasi == det) {
      det = 0;
      tmputama = 0;
    }
  }

}




//----------------------------------------------------------------------
void TampilJamKecil() {

  int durasi = 30000; // 30 Detik atau 1 menit
  unsigned long time_now = 0;
  time_now = millis();

  while (millis() < time_now + durasi) {
    //Tunggu selama 10 detik
    //JAM
    RtcDateTime now = Rtc.GetDateTime();
    char jam[9];
    sprintf(jam, "%02d:%02d:%02d", now.Hour(), now.Minute(), now.Second());
    kiriKanan(jam);
    Serial1.print(jam);
    Serial.print(jam);
    Serial.print("\r\n");
  }

}
//TAMPILKAN TANGGAL

void TampilTanggal() {

  char hari[8];// Jumlah karakter 7+1
  char tanggal[7]; // contoh : 21 SEP// Jumlah karakter 6+1
  static uint32_t pM;
  uint32_t cM = millis();

  if (cM - pM > 3000) { //Tamilkan tanggal selama 3 detik

    pM = cM;
    sprintf(tanggal, "%s %02d %s %04d", namaHari[rHar], rTgl, namaBulan[rBul], rTah);
    kiriScrollTulisan(tanggal);

    tmpinfo = 1;

  }

}



//----------------------------------------------------------------------
//TAMPILKAN TANGGAL HIJRIAH

void TampilTanggalHijr() {

  char hari[8];
  char tanggal[7]; // 19 ROM

  static uint32_t pM;
  uint32_t cM = millis();

  if (cM - pM > 3000) {

    pM = cM;

    sprintf(tanggal, "%s %02d %s %04d", hariHijriah[rHar], Hjr_Date, bulanHijriah[Hjr_Month], Hjr_Year);

    kiriScrollTulisan(tanggal);

    tmpinfo = 2;

  }

}



//----------------------------------------------------------------------
// TAMPILKAN SUHU

void TampilSuhu() {

  char suhu[3];
  int koreksisuhu = 2; // Perkiraan selisih suhu mesin dengan suhu ruangan

  static uint32_t pM;
  uint32_t cM = millis();

  if (cM - pM > 3000) {

    pM = cM;

    sprintf(suhu, "SUHU : %02d*C", celsius - koreksisuhu);

    kiriScrollTulisan(suhu);

    tmpinfo = 3;

  }

}
//--------------------------------------
//--------------------------------------
// TAMPILKAN JADWAL SHOLAT

void TampilJadwalSholat() {

  static uint8_t i;
  static uint32_t pM;
  uint32_t cM = millis();
  char sholat[7];
  char jam[5];
  char TimeName[][8] = {"SUBUH", "TERBIT", "DZUHUR", "ASHAR", "TRBNM", "MAGHRIB", "ISYA"};
  int hours, minutes;
  if (cM - pM >= 3000) {

    pM = cM;

    if (i == 1) {
      i = 2; // Abaikan Terbit
    }
    if (i == 4) {
      i = 5; // Abaikan Terbenam
    }

    get_float_time_parts(times[i], hours, minutes);

    minutes = minutes + configjws.ihti;

    if (minutes >= 60) {
      minutes = minutes - 60;
      hours ++;
    }
    String sholat = TimeName[i];
    if (configjws.s_subuh != 0 || configjws.s_duhur != 0 || configjws.s_asar != 0 || configjws.s_magrib != 0 || configjws.s_isya != 0) {

      if (i == 0) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_subuh);
      }
      if (i == 2) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_duhur);
      }
      if (i == 3) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_asar);
      }
      if (i == 5) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_magrib);
      }
      if (i == 6) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_isya);
      }

    }
    else {
      //      sholat = TimeName[i];
      //sprintf(jam, " %02d:%02d", hours, minutes);
      sprintf(jam, "%s %02d:%02d", sholat, hours, minutes);
    }
    kiriScroll(jam);
    i++;

    if (i > 6) {
      get_float_time_parts(times[0], hours, minutes);
      minutes = minutes + configjws.ihti;
      if (minutes < 11) {
        minutes = 60 - minutes;
        hours --;
      } else {
        minutes = minutes - 10 ;
      }
      if (configjws.s_imsak != 0) {
        sprintf(jam, "IMSAK : %02d:%02d", hours, minutes + configjws.s_imsak);
      }
      else {
        sprintf(jam, "IMSAK : %02d:%02d", hours, minutes);
      }
      kiriScroll(jam);

      i = 0;
      tmputama = 0;
      tmpinfo = 0;
      tmpjws = 0;

    }

  }

}

void TampilJadwalSholatCepat() {

  static uint8_t i;
  static uint32_t pM;
  uint32_t cM = millis();
  char sholat[7];
  char jam[5];
  char TimeName[][8] = {"SUBUH", "TERBIT", "DZUHUR", "ASHAR", "TRBNM", "MAGHRIB", "ISYA"};
  int hours, minutes;
  if (cM - pM >= 3000) {

    pM = cM;

    if (i == 1) {
      i = 2; // Abaikan Terbit
    }
    if (i == 4) {
      i = 5; // Abaikan Terbenam
    }

    get_float_time_parts(times[i], hours, minutes);

    minutes = minutes + configjws.ihti;

    if (minutes >= 60) {
      minutes = minutes - 60;
      hours ++;
    }
    String sholat = TimeName[i];
    if (configjws.s_subuh != 0 || configjws.s_duhur != 0 || configjws.s_asar != 0 || configjws.s_magrib != 0 || configjws.s_isya != 0) {

      if (i == 0) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_subuh);
      }
      if (i == 2) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_duhur);
      }
      if (i == 3) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_asar);
      }
      if (i == 5) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_magrib);
      }
      if (i == 6) {
        sprintf(jam, "%s %02d:%02d", sholat, hours, minutes + configjws.s_isya);
      }

    }
    else {
      //      sholat = TimeName[i];
      //sprintf(jam, " %02d:%02d", hours, minutes);
      sprintf(jam, "%s %02d:%02d", sholat, hours, minutes);
    }
    kiriScroll(jam);
    i++;

    if (i > 6) {
      get_float_time_parts(times[0], hours, minutes);
      minutes = minutes + configjws.ihti;
      if (minutes < 11) {
        minutes = 60 - minutes;
        hours --;
      } else {
        minutes = minutes - 10 ;
      }
      if (configjws.s_imsak != 0) {
        sprintf(jam, "IMSAK : %02d:%02d", hours, minutes + configjws.s_imsak);
      }
      else {
        sprintf(jam, "IMSAK : %02d:%02d", hours, minutes);
      }
      kiriScroll(jam);

      i = 0;
      tmpjws = 3;

    }

  }

}
void Jam_adzan() {

  int durasi = 5000; // 5 Detik
  unsigned long time_now = 0;
  time_now = millis();

  while (millis() < time_now + durasi) {
    //Tunggu selama 10 detik
    //JAM
    RtcDateTime now = Rtc.GetDateTime();
    char jam[9];
    sprintf(jam, "%02d:%02d:%02d", now.Hour(), now.Minute(), now.Second());
    kiriKanan(jam);

  }

}
//----------------------------------------------------------------------
