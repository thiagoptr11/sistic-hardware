#include <LiquidCrystal.h> 
#include <DHT.h>
#include <Average.h>
#include <SD.h>
#include <SPI.h>

#define DHTPIN 7
#define DHTTYPE DHT22
#define amostra 500



DHT dht(DHTPIN, DHTTYPE);

File sdCard;

long duration;
int distance;
float divisor, distancia_cm;
float h, t;
int i, distancia_int;

String sdFileName = "HCSR04_ensaio_1.txt";

float media = 0;

void setup()
{
  
  Serial.begin(9600); // Starts the serial communication
  dht.begin();
  
  SD.begin();
}

void loop()
{
  Average<int> ave(500);
  t = mediaTemperatura();
  
  divisor = 10000/(sqrt((t+273.15)/273.15) * 331.45);   
      
  for(int i = 0; i < amostra; i++)
  {
    distancia_cm = calcularDistancia(divisor);

    distancia_int = distancia_cm;
    
    ave.push(distancia_int);
  }
  
  Serial.println(String("Distância: " + String(ave.mode())));
  
  sdCard = SD.open(sdFileName, FILE_WRITE);// abre o arquivo de texto onde são salvas as informações;
  
 
  delay(10);
}
    
void mostrarValores(float valor){

  Serial.println(valor);
  
 
}

float mediaTemperatura(){
  float mediatemperatura = 0, t;
  for (i = 0; i < 100; i++)
    {
      t = dht.readTemperature();

      if ( i == 0 ) mediatemperatura = t;

      else mediatemperatura=((mediatemperatura+t)/2);
      
      delay(10);
    }

    return t;
}


  
