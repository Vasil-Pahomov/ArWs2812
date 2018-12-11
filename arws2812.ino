#include <Adafruit_NeoPixel.h>


#include "palette.h"

#include "anim.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define ANIMS 7 //number of animations (not including start one)
#define PALS 8 //number of palettes
#define INTERVAL 30000 //change interval, msec

//#define USE_START_ANIMATION //start animation is used in cycling as well as other animations

#define WIFI_SSID "YOUR-SSID"
#define WIFI_PASS "YOUR-PASS"

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue, &PalXMas};

Anim anim = Anim();

unsigned long ms = 10000;//startup animation duration, 10000 for "release" AnimStart

int paletteInd = random(PALS);
int animInd = 0;

extern Adafruit_NeoPixel pixels;

ESP8266WebServer server(80);

const char HTML[] PROGMEM = 
"<!DOCTYPE HTML><html lang=\"ru-RU\"><head><meta charset=\"utf-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><script>const ESP_SRV=\"\";let suspendTimer;function suspend(){sendCmd(\"/sus\").then(function(response){return response.json();}).then(function(data){document.body.style.display=\"\";document.getElementById('animSelect').value=data.a;document.getElementById('palSelect').value=data.p;console.log(\"a=\" + data.a + \",p=\" + data.p);document.getElementById('onBox').checked=(data.a >=0);document.getElementById('mainControls').style.display=(data.a >=0) ? '' : 'hidden';});}function onAnimPalChange(){sendCmd(\"/set?a=\" + document.getElementById('animSelect').value + \"&p=\" + document.getElementById('palSelect').value);}function sendCmd(url){if (suspendTimer){clearTimeout(suspendTimer);}return fetch(ESP_SRV + url).then(function(response){if (response.ok){suspendTimer=window.setTimeout(suspend, 5000);return response;}else{throw new Error();}}).catch(function(err){alert('Error communicating!');suspendTimer=window.setTimeout(suspend, 500);throw new Error(err);});;}function onOnBoxChange(){var mainControlsDiv=document.getElementById('mainControls'); if (document.getElementById('onBox').checked){mainControlsDiv.style.display='';document.getElementById('animSelect').value=0;onAnimPalChange();}else{mainControlsDiv.style.display='none';sendCmd(\"/set?a=-1&p=\" + document.getElementById('palSelect').value);}}document.addEventListener(\"DOMContentLoaded\", start);function start(){suspend();}</script><style>body{font-size:200%;font-family: Arial;}select{width: 100%;font-size:100%;margin: 10px 0;border: solid 2px;}</style></head><body style=\"display:none\"><label><input type=\"checkbox\" id=\"onBox\" onchange=\"onOnBoxChange()\"/>Включить</label><div id=\"mainControls\"><select id=\"animSelect\" onchange=\"onAnimPalChange()\"><option value=\"0\">Начальная</option><option value=\"1\">Бег</option><option value=\"2\">Пыльца эльфов</option><option value=\"3\">Вспышки</option><option value=\"4\">Случайный цикл</option><option value=\"5\">Звезды</option><option value=\"6\">Полосы</option><option value=\"7\">Полет</option></select><select id=\"palSelect\" onchange=\"onAnimPalChange()\"><option value=\"0\">RGB</option><option value=\"1\">Радуга</option><option value=\"2\">Полосатая радуга</option><option value=\"3\">Вечеринка</option><option value=\"4\">Жара</option><option value=\"5\">Огонь</option><option value=\"6\">Лёд</option><option value=\"7\">Рождество</option></select></div></body></html>";

void setup() {
    Serial1.begin(115200);

  Serial1.println("Entering setup");
  pixels.begin();
  randomSeed(analogRead(0)*analogRead(1));
  anim.setAnim(animInd);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
  anim.doSetUp();

  WiFi.begin(WIFI_SSID, WIFI_PASS);

 // ждем соединения:
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial1.print(".");
  }

  Serial1.print("IP address: ");Serial1.println(WiFi.localIP());


  //index page
  server.on("/", HTTP_GET, []{
      server.send(200, "text/html", HTML);
  });

  //sustains current effect for next INTERVAL. Returns JSON with current animation index and palette index:
  // {'a':ANIMATION_INDEX,'b':'PALETTE_INDEX'}
  // negative animation index means the strip is powered off
  server.on("/sus",[](){ sustainEffect(); });

  //sets effect and palette. Inputs two parameters: a=ANIMATION_INDEX&b=PALETTE_INDEX
  //negative animation index turns the strip off
  server.on("/set",[](){ setEffect(); });
  
  server.begin();
  Serial1.println("Setup done");
}

void loop() {
  yield();
  
  /* this piece of code checks for looping while trying to find different colors
  for (int pi=0;pi<PALS;pi++) {
    int c = 0;
    
    Serial.print(F("pi="));Serial.print(pi);
    Color c1 = pals[pi]->getPalColor((float)rngb()/256);
    Color c2 = c1;
    while (c1.isCloseTo(c2)) {
      c = c + 1;
      c2 = pals[pi]->getPalColor((float)rngb()/256);
    }
    Serial.print(F(" c="));Serial.println(c);
  }
  /**/

  anim.run();
  
  yield();
  
  if (millis() > ms && animInd >= 0) {// non-negative animind is for turned off strip - it never ends automatically
    Serial1.print("RSSI:");Serial1.print(WiFi.RSSI());Serial1.print(" ST:");Serial1.println(WiFi.status());
    ms = millis() + INTERVAL; 
    switch ( (animInd < 0) ? 0 : random(1)) {
      case 0: 
      {
        Serial1.print(F("anim->"));
        int prevAnimInd = animInd;
#ifdef USE_START_ANIMATION
        while (prevAnimInd == animInd) animInd = random(ANIMS+1);
        if (animInd == 0) ms = millis() + 10000;//startup animation has fixed 10 seconds length
#else
        while (prevAnimInd == animInd) animInd = random(ANIMS) + 1;
#endif        
        anim.setAnim(animInd);
        anim.setPeriod(random(20, 40));
        anim.setPalette(pals[paletteInd]);
        anim.doSetUp();
        break;
      }
      case 1:
      {
        Serial1.print(F("pal->"));
        int prevPalInd = paletteInd;
        while (prevPalInd == paletteInd) paletteInd = random(PALS);
        anim.setPalette(pals[paletteInd]);
        Serial1.print(paletteInd);
        break;
      }
    }
    Serial1.println();
  }
  /**/

  yield();

  server.handleClient();
}

void sustainEffect() {
    char buf[100];
    sprintf(buf, "{\"a\":%d,\"p\":%d}", animInd, paletteInd);
    if (animInd > 0) {
      ms = millis() + INTERVAL; //"reset" duration for any animation except start
    }
    server.sendHeader("Access-Control-Allow-Origin","*");
    server.send(200,"text/html", buf);
}

void setEffect() {
    if (!server.hasArg("a") || !server.hasArg("p")) {
      server.send(400, "text/html", "No arg");
    }

    int newAnimInd = server.arg("a").toInt();
    if (newAnimInd > ANIMS) {
      char buf[100];
      sprintf(buf, "Wrong a=%d", newAnimInd);
      server.send(400, "text/html", buf);
    }
    animInd = newAnimInd;
    
    int newPalInd = server.arg("p").toInt();
    if (newPalInd < 0 && newPalInd >= PALS) {
      char buf[100];
      sprintf(buf, "Wrong p=%d", newPalInd);
      server.send(400, "text/html", buf);
    }
    paletteInd = newPalInd;

    anim.setAnim(animInd);
    anim.setPalette(pals[paletteInd]);
    anim.doSetUp();

    server.sendHeader("Access-Control-Allow-Origin","*");
    server.send(200,"text/html", "");
}
