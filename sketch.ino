#include <stdio.h>
#include <stdlib.h>
#include <WiFi.h>

//Definimos leds correspondientes a los bits de las horas en un array
byte arrayLedPin[13] = {33, 32, 15, 4, 2, 99, 98, 13, 12, 14, 27, 26, 25}; //los pines 99 y 98 son fake

#define NTP_SERVER     "pool.ntp.org" //valores del servidor NTP y ajustes
#define UTC_OFFSET     0
#define UTC_OFFSET_DST 0

struct Ahora { int hora; int minutos; int segundos; }; //Definimos structura ahora con los enteros correspondientes 

void setup() {
  Serial.begin(115200);

  // Modo de los pines 
  for (int i = 0; i < 13; i++) {
    pinMode(arrayLedPin[i
    ], OUTPUT);
  }

  Serial.println("Contectando a Wifi"); //Conexión a Wifi
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }

  Serial.println("Conexión establecida a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Conexión establecida");

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void loop() {
  struct Ahora ahora; //definimos un struct de Ahora para el tiempo actual
  ahora = getLocalTime();
  hBin(ahora.hora,ahora.minutos);//paramos los int de ahora
  Serial.print(ahora.hora);
  Serial.print(":");
  Serial.print(ahora.minutos);
  Serial.print(":");
  Serial.println(ahora.segundos);
  delay(1000); // espera 1000ms (1segundo) para repetir el loop
}

void hBin(int hora,int minutos){

  //Sumamos 128 unidades a la hora y minutos para forzar que ambos valores tengan 8 bits
  hora = hora + 128;
  minutos = minutos + 128;

  char str_hora [32]; //revisar si es necesario los 32 
  char str_minutos [32]; //revisar si es necesario los 32 
  char str_hm [32]; //revisar si es necesario los 32 

  itoa(hora, str_hora, 2); //Convierte a Binario
  itoa(minutos, str_minutos, 2); //Convierte a Binario
  
  strcpy(str_hm, str_hora);  //Contatenamos en str_hm la hora y minutos en binario
  strcat(str_hm, str_minutos);

  str_hm[0] = '0'; //Cambiamos a 0 el valor de los bits de peso 128
  str_hm[8] = '0';

  Serial.println(str_hm); //Imprimimos la hora y minutos concatenados (en binario)

  for (int i = 0; i < 13; i++) { //recorre los bits de la hora en binario para actualizar el balor ON/OFF de los leds 
    if (str_hm[i+3] == '1'){ //empieza en el 
      digitalWrite(arrayLedPin[i], HIGH);
    }
    else
      digitalWrite(arrayLedPin[i], LOW);
  }//cierra el for
} //cierra la función

// Trabajo con servidor NTP para obtener la fecha y la hora actual
Ahora getLocalTime() { //obtendremos los valores de tiempo y los guardamos en el struct Ahora definido arriba
  struct tm timeinfo;
  Ahora ahorita;
  if (!getLocalTime(&timeinfo)) {
      Serial.println("Connection Err");
      ahorita.hora = 0;
      ahorita.minutos = 0;
      ahorita.segundos = 0;
      return ahorita;
  }
    ahorita.hora = timeinfo.tm_hour;
    ahorita.minutos = timeinfo.tm_min;
    ahorita.segundos = timeinfo.tm_sec;
    return ahorita;
}