char setwaktu[] PROGMEM = R"=====(

<!DOCTYPE HTML>
<html>
<meta name='viewport' content='width=device-width, initial-scale=1'>

<head>
<title>MASJID AL-KAROMAH</title>
<style> body { width: 97% ; text-align: center; } input, select, button, textarea { max-width: 100% ; margin: 5px; padding: 5px; border-radius: 7px; } meter, progress, output { max-width: 100% ; } h1 { color: grey; } .note { color: #555; font-size:1em;} .info { background-color: #eee; border-radius: 3px; padding-top: 17px; margin: 5px;} .tombol { background-color: #ccc; margin: 20px; padding: 10px; border-radius: 10px; text-decoration: none;}</style>
</head>

<body onload='updateData()'>

<h1>MASJID AL KAROMAH</h1>
<h4><a href="https://intip.in/masjidAK">MASJID AL-KAROMAH</a></h4>


<div class='info'>
  <span id='day'></span>/<span id='month'></span>/<span id='year'></span><span> </span>
  <span id='hour'></span>:<span id='minute'></span>:<span id='second'></span><span> </span>
  <span id='temp'></span><span>C</span><br><br>
</div>

<h1>Set WAKTU SOLAT</h1>
<h3>Pengaturan</h3>

<form>
<table width='100%'>
  <tr>
    <td>
      <label for="s_imsak">Selisih Imsak</label><br>
      <input id="s_imsak" placeholder="0" maxlength="3" size="3"/>
    </td>
    <td>
      <label for="s_subuh">Selisih Subuh</label><br>
      <input id="s_subuh" placeholder="0" maxlength="3" size="3"/>
    </td>
    <td>
      <label for="s_duhur">Selisih Dzuhur</label><br>
      <input id="s_duhur" placeholder="0" maxlength="3" size="3"/>
    </td>    
  </tr>
  <tr>
    <td>
      <label for="s_asar">Selisih Ashar</label><br>
      <input id="s_asar" placeholder="0" maxlength="3" size="3"/>
    </td>
    <td>
      <label for="s_magrib">Selisih Maghrib</label><br>
      <input id="s_magrib" placeholder="0" maxlength="3" size="3"/>
    </td>
    <td>
      <label for="s_isya">Selisih Isya</label><br>
      <input id="s_isya" placeholder="0" maxlength="3" size="3"/>
    </td>    
  </tr>
</table>
<table width='100%'>
  <tr>
    <td>
      <label for="latitude">Latitude</label><br>
      <input id="latitude" placeholder="-7.1127062165250035" size="9"/>
    </td>
    <td>
      <label for="longitude">Longitude</label><br>
      <input id="longitude" placeholder="113.48328982000642" size="9"/>
    </td>    
  </tr>
  <tr>
    <td>
      <label for="zonawaktu">Zona Waktu</label><br>
      <input id="zonawaktu" placeholder="7" size="3"/>
    </td>
    <td>
      <label for="ihti">Ihtiyati</label><br>
      <input id="ihti" placeholder="2" maxlength="2" size="3"/>
    </td>
  </tr>
  <tr>
    <td colspan="2">
      <label for="hilal">Hilal</label><br>
      <input id="hilal" placeholder="2" maxlength="2" size="3"/>
    </td>
  </tr>
</table>
<br>
  <td>
      <label for="custom">CUSTOM</label><br>
      <input id="custom" placeholder="1" maxlength="1" size="3"/>
  </td>
<br>
<div>
  <button onClick="setJws()"> Simpan </button>
</div>
</form>

<br><br><br>

<h3>Informasi</h3>

<form hidden>
  <div>
    <label for="namamasjid">Nama Masjid</label><br>
    <textarea rows="3" id="namamasjid" placeholder="Masjid Al Karomah"/></textarea>
  </div>
  <br><br>
   <div>
    <label for="informasi">Informasi</label><br>
    <textarea rows="3" id="informasi" placeholder="PENGASUH K.H. MAWARDI S.Ag M.M."/></textarea>
  </div>
  <div>
    <br>
    <button onClick="setJws()"> Simpan </button>
  </div>
</form>

<br><br>
<hr/>
<br><br>

<h2>Ubah Waktu</h2>

<table width='100%'>
  <tr>
    <td>
      <form>
        <h4>Tanggal</h4>  
        <input type='date' name='date' min='2021-01-01'><br><br>
        <input type='submit' value='Ubah Tanggal'> 
      </form>  
    </td>
    <td>
      <form>  
        <h4>Jam</h4>
        <input type='TIME' name='time'><br><br>
        <input type='submit' value='Ubah Jam'> 
      </form>
    </td>
  </tr>
</table>
<br><br><br>
<hr/>
<br><br>
</body>


<script>

var xmlHttp=createXmlHttpObject();

function createXmlHttpObject(){
 if(window.XMLHttpRequest){
    xmlHttp=new XMLHttpRequest();
 }else{
    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');// code for IE6, IE5
 }
 return xmlHttp;
}

function updateData() {
  prosesdatajws();
  proseswaktu();
}


function proseswaktu(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
   xmlHttp.open('PUT','xmlwaktu',true);
   xmlHttp.onreadystatechange=handleServerResponseWaktu;
   xmlHttp.send(null);
 }
 setTimeout('proseswaktu()',1000);
}

function prosesdatajws(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
   xmlHttp.open('PUT','xmldatajws',true);
   xmlHttp.onreadystatechange=handleServerResponseDataJWS;
   xmlHttp.send(null);
 }
}


function handleServerResponseWaktu(){
  
 if(xmlHttp.readyState==4 && xmlHttp.status==200){
   xmlResponse=xmlHttp.responseXML;

   xmldoc = xmlResponse.getElementsByTagName('Tahun');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('year').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('Bulan');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('month').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('Tanggal');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('day').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('Jam');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('hour').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('Menit');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('minute').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('Detik');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('second').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('Suhu');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('temp').innerHTML=message;
 }

}


function handleServerResponseDataJWS(){
  
 if(xmlHttp.readyState==4 && xmlHttp.status==200){
   xmlResponse=xmlHttp.responseXML;

   xmldoc = xmlResponse.getElementsByTagName('SelisihImsak');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('s_imsak').value=message;

   xmldoc = xmlResponse.getElementsByTagName('SelisihSubuh');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('s_subuh').value=message;

   xmldoc = xmlResponse.getElementsByTagName('SelisihDuhur');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('s_duhur').value=message;

   xmldoc = xmlResponse.getElementsByTagName('SelisihAsar');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('s_asar').value=message;

   xmldoc = xmlResponse.getElementsByTagName('SelisihMagrib');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('s_magrib').value=message;

   xmldoc = xmlResponse.getElementsByTagName('SelisihIsya');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('s_isya').value=message;

   xmldoc = xmlResponse.getElementsByTagName('Custom');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('custom').value=message;

   xmldoc = xmlResponse.getElementsByTagName('Ihtiyati');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('ihti').value=message;

   xmldoc = xmlResponse.getElementsByTagName('Latitude');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('latitude').value=message;

   xmldoc = xmlResponse.getElementsByTagName('Longitude');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('longitude').value=message;

   xmldoc = xmlResponse.getElementsByTagName('ZonaWaktu');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('zonawaktu').value=message;

   xmldoc = xmlResponse.getElementsByTagName('Hilal');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('hilal').value=message;

   xmldoc = xmlResponse.getElementsByTagName('NamaMasjid');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('namamasjid').value=message;

   xmldoc = xmlResponse.getElementsByTagName('Informasi');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('informasi').value=message;
 }

}


function setJws() {
  
  console.log("tombol telah ditekan!");
  var s_imsak = document.getElementById("s_imsak").value;
  var s_subuh = document.getElementById("s_subuh").value;
  var s_duhur = document.getElementById("s_duhur").value;
  var s_asar = document.getElementById("s_asar").value;
  var s_magrib = document.getElementById("s_magrib").value;
  var s_isya = document.getElementById("s_isya").value;
  var custom = document.getElementById("custom").value;
  var ihti = document.getElementById("ihti").value;
  var latitude = document.getElementById("latitude").value;
  var longitude = document.getElementById("longitude").value;
  var zonawaktu = document.getElementById("zonawaktu").value;
  var hilal = document.getElementById("hilal").value;
  var namamasjid = document.getElementById("namamasjid").value;
  var informasi = document.getElementById("informasi").value;
  
  var datajws = {s_imsak:s_imsak, s_subuh:s_subuh, s_duhur:s_duhur, s_asar:s_asar, s_magrib:s_magrib, s_isya:s_isya, custom:custom, ihti:ihti, latitude:latitude, longitude:longitude, zonawaktu:zonawaktu, hilal:hilal, namamasjid:namamasjid, informasi:informasi};
//  var datajws = {custom:custom, ihti:ihti, latitude:latitude, longitude:longitude, zonawaktu:zonawaktu, hilal:hilal, namamasjid:namamasjid, informasi:informasi};
  
  var xhr = new XMLHttpRequest();
  var url = "/simpandatajws";
  
  xhr.onreadystatechange = function() {
    if(this.onreadyState == 4  && this.status == 200) {
      console.log(xhr.responseText);
    }
  };
  xhr.open("POST", url, true);
  xhr.send(JSON.stringify(datajws));
  
};


</script>

</html>


)=====";
