#include "heltec.h"
#include "images.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define BAND      433E6  //you can set band here directly,e.g. 868E6,915E6

#define WIFI_SSID "WLAN"
#define WIFI_PWD  "99112288"

unsigned int counter = 0;

String rssi = "RSSI --";
String packSize = "--";
String packet ;

WebServer  server(80);

const int led = 25;

RF24 radio(3, 2);
const uint64_t address = 0xF0F0F0F0E1LL;

void setup()
{
  pinMode(led, OUTPUT);

  digitalWrite(led, 1);
  delay(1000);
  digitalWrite(led, 0);
  delay(1000);
  digitalWrite(led, 1);
  delay(1000);
  digitalWrite(led, 0);
  
  Serial.begin(115200);
  Serial.println("CTF v1.0");

  Serial.println("+ LoraWAN");
  Heltec.begin(true, true, true, true, BAND);

  Serial.println("+ RF433MHz");
  //pinMode(13,OUTPUT);
  //vw_set_tx_pin(12); 

  Serial.println("+ NRF24L01");
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.print("+ WIFI");
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("+ IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  
  server.onNotFound(handleNotFound);
  
  server.on("/level1", []() {
    server.send(200, "text/plain", "Level1: Inicie la recepción en 433MHz, se enviara 3 veces la bandera.\nEnvie la bandera como lorawan{valor encontrado}");
    level1();
  });

  server.on("/level2", []() {
    server.send(200, "text/plain", "Level2: Inicie la recepción en 433MHz, se enviara 3 veces la bandera.");
    level2();
  });

  server.on("/level3", []() {
    server.send(200, "text/plain", "level3");
  });
  
  server.on("/level4", []() {
    server.send(200, "text/plain", "level4");
  });

  server.on("/level5", []() {
    server.send(200, "text/plain", "level5");
  });

  server.on("/level6", []() {
    server.send(200, "text/plain", "level6");
  });

  server.on("/level7", []() {
    server.send(200, "text/plain", "level7");
  });

  server.on("/level8", []() {
    server.send(200, "text/plain", "level8");
  });

  server.on("/level9", []() {
    server.send(200, "text/plain", "level9");
  });
     
  server.begin();

  Serial.println("+ OLED");
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->drawXbm(32, 0, logo_width, logo_height, logo_bits);
  Heltec.display->display();

  Serial.println("+ INIT OK");
}

void sendLora(char* message) {
  LoRa.beginPacket();
  LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(message);
  LoRa.endPacket();
}

void sendRF(char* message, int bps) {
  digitalWrite(13,1); 
  //vw_setup(bps)
  //vw_send((uint8_t *)message, strlen(message));
  //vw_wait_tx(); 
  digitalWrite(13,0); 
}

void sendNRF24L01(char* message) {
  radio.write(message, sizeof(message));
}

void loop()
{
  server.handleClient();
}

void level1() {
  for(int i = 0; i < 3 ; i++) {
    sendLora("U");
    delay(500);
  }
}

void level2() {
  for(int i = 0; i < 3 ; i++) {
    sendLora("lorawan{prueba}");
    delay(500);
  }
}

void level3() {
  Serial.println("level3");
}

void level4() {
  Serial.println("level4");
}

void level5() {
  Serial.println("level5");
}

void level6() {
  Serial.println("level6");
}

void level7() {
  Serial.println("level7");
}

void level8() {
  Serial.println("level8");
}

void level9() {
  Serial.println("level9");
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", getHome());
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

String getHome() {
  return "<!DOCTYPE HTML>"
    "<html lang='es'><head><meta name='viewport' content='width=device-width,initial-scale=1'><link rel='icon' href='data:,'><link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bulma@0.9.4/css/bulma.min.css'><title>radiospace.ar - ctf</title></head>"
    "<body class='is-light'><div class='block m-5'><h1 class='is-size-1 has-text-danger has-text-centered'>radiospace.ar</h1><h2 class='is-size-4 has-text-centered'>Bienvenidos a nuestro <strong>CTF</strong>!</h2></div>"
    "<div class='card m-5'><div class='card-content'><div class='media'><div class='media-content'><figure class='image mb-1'>"
    "    <img style='height:100px;width:auto;' src='https://www.thethingsnetwork.org/docs/lorawan/icon.png'>"
    "    </figure>"
    "    <button class='button is-success is-small m-2' href='/level1'>nivel 1 (100 pts)</button>"
    "    <button class='button is-info is-small m-2' href='/level2'>nivel 2 (200 pts)</button>"
    "    <button class='button is-danger is-small m-2' href='/level3'>nivel 3 (600 pts)</button>"
    "</div></div></div></div>"
    "<div class='card m-5'><div class='card-content'><div class='media'><div class='media-content'><figure class='image mb-1'>"
    "    <img style='height:100px;width:auto;' src='https://cdn2.expertreviews.co.uk/sites/expertreviews/files/styles/er_main_wide/public/2019/02/what_is_zigbee.jpg?itok=8HWhkdW0'>"
    "    </figure>"
    "    <button class='button is-success is-small m-2' href='/level4'>nivel 1 (100 pts)</button>"
    "    <button class='button is-info is-small m-2' href='/level5'>nivel 2 (200 pts)</button>"
    "    <button class='button is-danger is-small m-2' href='/level6'>nivel 3 (600 pts)</button>"
    "</div></div></div></div>"
    "<div class='card m-5'><div class='card-content'><div class='media'><div class='media-content'><figure class='image mb-1'>"
    "    <img style='height:100px;width:auto;' src='https://cirilocabos.vteximg.com.br/arquivos/ids/176192-1000-1000/modulo-receptor-rf433-mhz-robotica-arduino-902110-02.jpg?v=637199884949970000'>"
    "    </figure>"
    "    <button class='button is-success is-small m-2' href='/level7'>nivel 1 (100 pts)</button>"
    "    <button class='button is-info is-small m-2' href='/level8'>nivel 2 (200 pts)</button>"
    "    <button class='button is-danger is-small m-2' href='/level9'>nivel 3 (600 pts)</button>"
    "</div></div></div></div>"
    "</body></html>";
}
