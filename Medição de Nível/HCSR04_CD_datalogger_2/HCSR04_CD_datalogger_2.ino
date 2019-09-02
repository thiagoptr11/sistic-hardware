#include <DHT.h>
#include <Average.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

/*
#define trigPin PB6
#define echoPin PB7
#define DHTPIN PB8
#define DHTTYPE DHT22
#define amostra 500

#define SPI1_NSS_PIN PA4 
*/

#define trigPin1 PA9
#define echoPin1 PA10

#define trigPin2 PB6
#define echoPin2 PB7

#define trigPin3 PB8
#define echoPin3 PB9

#define DHTPIN PA8
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

int amostra = 100;

void setup()
{
  Serial.begin(9600); // Starts the serial communication
  
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin3, INPUT); // Sets the echoPin as an Input

  delay(3000);
  
  dht.begin(); 
}

void loop()
{
  int v1[100], cont1[100], v2[100], cont2[100], v3[100], cont3[100];
  delay(6000);
  
  float te = mediaTemperatura();
  
  Serial.print("Temperatura: ");
  Serial.print(te);
  Serial.println(" *C");
  
  float divisor = 10000/(sqrt((te+273.15)/273.15) * 331.45); 
      
  for(int i = 0; i < 100; i++)
  {
    float distancia_cm1 = calcularDistancia(trigPin1, echoPin1, divisor);
    float distancia_cm2 = calcularDistancia(trigPin2, echoPin2, divisor);
    float distancia_cm3 = calcularDistancia(trigPin3, echoPin3, divisor);
    
    int distancia_int1 = int(distancia_cm1);
    int distancia_int2 = int(distancia_cm2);
    int distancia_int3 = int(distancia_cm3);

    v1[i] = distancia_int1;
    v2[i] = distancia_int2;
    v3[i] = distancia_int3;

    delay(10);
    
    //ave.push(distancia_int);
  }
  
  //Serial.println(String("Distância: " + String(ave.mode())));
  int dist1 = calcularModa (100, v1, cont1);
  int dist2 = calcularModa (100, v2, cont2);
  int dist3 = calcularModa (100, v3, cont3);

  int dist = (dist1 + dist2 + dist3)/3;

  Serial.print(dist1);
  Serial.print(" ");
  Serial.print(dist2);
  Serial.print(" ");
  Serial.println(dist3);
  
  delay(1000);
  Serial.print("Distância: ");
  Serial.print(dist);
  Serial.println(" cm");
  
  delay(10);
}

float mediaTemperatura(){
  float mediatemperatura = 0, temp;
  for (int i = 0; i < amostra; i++)
    {
      temp = dht.readTemperature();

      if ( i == 0 ) {
        mediatemperatura = temp;
      }

      else {
        mediatemperatura=((mediatemperatura + temp)/2);
      }
      
      delay(10);
    }
    return mediatemperatura;
}

float calcularDistancia(int trig, int echo, float divisor)
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int tempo = pulseIn(echo, HIGH);
  
  float distancia_cm = (tempo / divisor) / 2;

  return distancia_cm;
}

int calcularModa (int tamanho, int v[], int cont[]){

  int conta = 0; int moda = 0;
  for(int i=0;i<tamanho;i++){
        for(int j=i+1;j<tamanho;j++){
          
      if(v[i]==v[j]){
        cont[i]++;
          if(cont[i]>conta){
            conta=cont[i];
            moda=v[i];
          }
      }
          
        }
        cont[i]=0;
    }

    return moda;
}

  
