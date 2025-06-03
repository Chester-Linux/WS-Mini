#include <SFE_BMP180.h>
#include <Wire.h>
#include "DHT.h"
#define DHT22_PIN 2
#define ALTITUDE 1655.0

DHT dht22(DHT22_PIN, DHT22);
SFE_BMP180 pressure;

int pino_d = 7; //Pino ligado ao D0 do sensor
int pino_a = A0; //Pino ligado ao A0 do sensor
int val_d = 0; //Armazena o valor lido do pino digital
int val_a = 0; //Armazena o valor lido do pino analogico

void setup() {
  Serial.begin(9600);  //Serial Monitor
  Serial1.begin(9600);  //Bluetooth Module

  Serial.println("STARTING");

  dht22.begin(); // initialize the DHT22 sensor
  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin())
  {
    Serial.println("BMP180 init success");
  }  
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    
    while(1); // Pause forever.
  }

  pinMode(pino_d, INPUT);
  pinMode(pino_a, INPUT);
  
}

void loop() {
  // BMP180
  char status;
  double T,P,p0,a;

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
  Serial.println();
  Serial.println("BMP180#");
  Serial.println("Provided altitude: " + String(ALTITUDE,0) + " meters, " + String(ALTITUDE*3.28084,0) + " feet");

  Serial1.println();
  Serial1.println("Provided altitude: " + String(ALTITUDE,0) + " meters, " + String(ALTITUDE*3.28084,0) + " feet");
  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.println("Absolute pressure: " + String(P,2) + " mb, " + String(P*0.0295333727,2) + " inHg");
          Serial1.println("Absolute pressure: " + String(P,2) + " mb, " + String(P*0.0295333727,2) + " inHg");
          
          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.println("Relative (sea-level) pressure: " + String(p0,2) + " mb, " + String(p0*0.0295333727,2) + " inHg");
          Serial1.println("Relative (sea-level) pressure: " + String(p0,2) + " mb, " + String(p0*0.0295333727,2) + " inHg");
          
          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          Serial.println("Computed altitude: " + String(a,0) + " meters, " + (a*3.28084,0) + " feet");
          Serial1.println("Computed altitude: " + String(a,0) + " meters, " + (a*3.28084,0) + " feet");

        }
        else {
          Serial.println("error retrieving pressure measurement\n");

          }
      }
      else {
        Serial.println("error starting pressure measurement\n");

        }
    }
    else {
      Serial.println("error retrieving temperature measurement\n");

      }
  }

  else
    {
    Serial.println("error starting temperature measurement\n");
    }


  // DHT22
  float humi  = dht22.readHumidity();
  float temp = dht22.readTemperature();

  // check if any reads failed
  if (isnan(humi)) {
    Serial.println("Failed to read from DHT22 sensor!");

  }

  else
  {
    Serial.println();
    Serial.println("DHT22#");
    Serial.println("Temperature: " + String(temp) + "°C" + " // " + ((9.0/5.0)*temp+32.0,2) + "F°");
    Serial.println("Humidity: " + String(humi) + "%");
    Serial.println();

    Serial1.println();
    Serial1.println("Temperature: " + String(temp) + "°C" + " // " + ((9.0/5.0)*temp+32.0,2) + "F°");
    Serial1.println("Humidity: " + String(humi) + "%");
    Serial1.println();

  }

  // Lê e armazena o valor do pino digital
  val_d = digitalRead(pino_d);

  // Lê e armazena o valor do pino analógico
  val_a = analogRead(pino_a);

  // Envia as informações para o Serial Monitor
  Serial.print("Valor digital : ");
  Serial.print(val_d);
  Serial.print(" - Valor analogico : ");
  Serial.println(val_a);

  Serial.println("YL-83#");
  Serial.println("");

  // Mostra no Serial se há chuva ou não
  if (val_d == 1)
  {
    Serial.println("Chuva: Nao");
    Serial1.println("Chuva: Nao");
  }
  else
  {
    Serial.println("Chuva: Sim");
    Serial1.println("Chuva: Sim");
  }

  // Mostra no Serial o nível de intensidade da chuva
  if (val_a > 900 && val_a <= 1023)
  {
    Serial.println("Intensidade: ---- (Nenhuma)");
    Serial1.println("Intensidade: ---- (Nenhuma)");
  }
  else if (val_a > 700 && val_a <= 900)
  {
    Serial.println("Intensidade: Fraca");
    Serial1.println("Intensidade: Fraca");
  }
  else if (val_a > 500 && val_a <= 700)
  {
    Serial.println("Intensidade: Moderada");
    Serial1.println("Intensidade: Moderada");
  }
  else if (val_a <= 500)
  {
    Serial.println("Intensidade: Forte");
    Serial1.println("Intensidade: Forte");   
  }

  // Separador para facilitar a leitura no monitor serial
  Serial.println("--------------------------");

  delay(2000);
}