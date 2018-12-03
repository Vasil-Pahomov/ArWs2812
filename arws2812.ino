#include <Adafruit_NeoPixel.h>


#include "palette.h"

#include "anim.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define ANIMS 7 //number of animations
#define PALS 8 //number of palettes
#define INTERVAL 30000 //change interval, msec

#define USE_START_ANIMATION //start animation is used in cycling as well as other animations

#define WIFI_SSID "YOUR-SSID"
#define WIFI_PASS "YOUR-PASS"

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue, &PalXMas};

Anim anim = Anim();

unsigned long ms = 10000;//startup animation duration, 10000 for "release" AnimStart

int paletteInd = random(PALS);
int animInd = -1;

extern Adafruit_NeoPixel pixels;

ESP8266WebServer server(80);

const char HTML[] PROGMEM = 
"<!DOCTYPE HTML>\n<html lang=\"ru-RU\">\n<head>\n<meta charset=\"utf-8\"/>\n<script>\nvar ESP_SRV = \"\"\n\nfunction suspend()\n{\n\tfetch(ESP_SRV + \"/sus\")\n\t.then(function(response) {\n\t\tif (response.ok) {\n\t\t\treturn response.json();\n\t\t} else {\n\t\t\tconsole.log('Sus request error:' + response.statusText)\n\t\t\talert('ERROR');\n\t\t}\n\t})\n\t.then(function(json) {\n\t\tdocument.getElementById('animSelect').value = json.a;\n\t\tdocument.getElementById('palSelect').value = json.p;\n\t\tconsole.log(\"a=\"+json.a+\",p=\"+json.p);\n\t})\n\t.catch(function(err) {\n\t\tconsole.log(\"Sus fetch error:-S\", err);\n\t});;\n\t\n\twindow.setTimeout(suspend, 5000);\n}\n\nfunction onAnimPalChange() {\n\tfetch(ESP_SRV + \"/set?a=\" + document.getElementById('animSelect').value + \"&p=\" + document.getElementById('palSelect').value)\n\t.then(function(response) {\n\t\tif (response.ok) {\n\t\t\tconsole.log('Set OK');\n\t\t} else {\n\t\t\tconsole.log('Set request error:' + response.statusText)\n\t\t\talert('ERROR');\n\t\t}\n\t})\n\t.catch(function(err) {\n\t\tconsole.log(\"Set fetch error:-S\", err);\n\t});\n}\n\nsuspend();\n</script>\n\n<style>\nselect {\n\twidth: 100%;\n\tfont-size: 70px;\n\tmargin: 10px;\n\tborder: solid 2px;\n}\n\noption {\n\tfont-size: 20px;\n}\n</style>\n\n</head>\n<body>\n\t<select id=\"animSelect\" onchange=\"onAnimPalChange()\">\n\t\t<option value=\"-1\">Начальная</option>\n\t\t<option value=\"0\">Бег</option>\n\t\t<option value=\"1\">Эльфиная пыль</option>\n\t\t<option value=\"2\">Вспышки</option>\n\t\t<option value=\"3\">Случайный цикл</option>\n\t\t<option value=\"4\">Звезды</option>\n\t\t<option value=\"5\">Полосы</option>\n\t\t<option value=\"6\">Полет</option>\n\t</select>\n\t<select id=\"palSelect\" onchange=\"onAnimPalChange()\">\n\t\t<option value=\"0\">RGB</option>\n\t\t<option value=\"1\">Радуга</option>\n\t\t<option value=\"2\">Полосатая радуга</option>\n\t\t<option value=\"3\">Вечеринка</option>\n\t\t<option value=\"4\">Жара</option>\n\t\t<option value=\"5\">Огонь</option>\n\t\t<option value=\"6\">Лёд</option>\n\t\t<option value=\"7\">Рождество</option>\n\t</select>\n</body>\n</html>";

void setup() {
  pixels.begin();
  Serial.begin(74880);
  randomSeed(analogRead(0)*analogRead(1));
  anim.setAnim(animInd);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
  anim.doSetUp();

  WiFi.begin(WIFI_SSID, WIFI_PASS);

 // ждем соединения:
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  server.on("/", HTTP_GET, []{
      server.send(200, "text/html", HTML);
  });

  //sustains current effect for next INTERVAL. Returns JSON with current animation index and palette index:
  // {'a':<ANIMATION_INDEX,'b':'PALETTE_INDEX'}
  server.on("/sus",[](){ sustainEffect(); });

  //sets effect and palette. Inputs two parameters: a=ANIMATION_INDEX&b=PALETTE_INDEX
  server.on("/set",[](){ setEffect(); });
  server.begin();
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

  if (millis() > ms) {
    ms = millis() + INTERVAL;
    switch ( (animInd < 0) ? 0 : random(1)) {
      case 0: 
      {
        Serial.print(F("anim->"));
        int prevAnimInd = animInd;
#ifdef USE_START_ANIMATION
        while (prevAnimInd == animInd) animInd = random(ANIMS+1) - 1;
        if (animInd < 0) ms = millis() + 10000;//startup animation has fixed 10 seconds length
#else
        while (prevAnimInd == animInd) animInd = random(ANIMS);
#endif        
        anim.setAnim(animInd);
        anim.setPeriod(random(20, 40));
        anim.setPalette(pals[paletteInd]);
        anim.doSetUp();
        break;
      }
      case 1:
      {
        Serial.print(F("pal->"));
        int prevPalInd = paletteInd;
        while (prevPalInd == paletteInd) paletteInd = random(PALS);
        anim.setPalette(pals[paletteInd]);
        Serial.print(paletteInd);
        break;
      }
    }
    Serial.println();
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
    if (newAnimInd < 0 || newAnimInd >= ANIMS) {
      char buf[100];
      sprintf(buf, "Wrong a=%d", newAnimInd);
      server.send(400, "text/html", buf);
    }
    animInd = newAnimInd;
    
    int newPalInd = server.arg("p").toInt();
    if (newPalInd < 0 && newPalInd >= ANIMS) {
      char buf[100];
      sprintf(buf, "Wrong p=%d", newPalInd);
      server.send(400, "text/html", buf);
    }
    paletteInd = newPalInd;

    anim.setAnim(animInd);
    anim.setPalette(pals[paletteInd]);

    server.sendHeader("Access-Control-Allow-Origin","*");
    server.send(200,"text/html", "");
}
