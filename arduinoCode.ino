//DEPENDÊNCIAS EXTERNAS || BIBLIOTECAS
#include <Ultrasonic.h>
#include <DHT11.h>
// DEFINIÇÃO DAS VARIAVEIS DE ENTRADA
#define pinUmidade A0
#define pinTemperatura A1
#define pinLuminosidade A2
// DEFINIÇÃO DAS VARIAVEIS DE SAIDA
#define bomba 9
#define cooler 22
#define lampada 50

// DEFINIÇÃO DOS PINOS DO SENSOR ULTRASSÔNICO
float sensorUltrassonico;
float sensorUltrassonico2;
Ultrasonic ultrassomEspaco(52, 53); //ESPAÇO 
Ultrasonic ultrassomReservatorio(30, 31); //RESERVATORIO 
//DHT11
DHT11 dht11(2);

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200); // COMUNICAÇÃO COM SERIAL DO ESP01 TX RX 18 19
  pinMode(pinUmidade, INPUT);
  pinMode(pinLuminosidade, INPUT);
  pinMode(pinTemperatura, INPUT);
  pinMode(bomba, OUTPUT);
  pinMode(cooler, OUTPUT);
  pinMode(lampada, OUTPUT);
}

void loop() {
  // UMIDADE CONFIG
  float UmidadeLida = analogRead(pinUmidade);
  float umidade = (UmidadeLida / 1023) * 100.0;
  if (umidade > 80)
    digitalWrite(bomba, LOW);
  else
    digitalWrite(bomba, HIGH);

  // TEMPERATURA CONFIG
int temperatura = dht11.readTemperature();
  if (temperatura >= 24)
    digitalWrite(cooler, LOW);
  else
    digitalWrite(cooler, HIGH);

  // LUMINOSIDADE CONFIG
  float luzMedida = analogRead(pinLuminosidade);
  float luminosidade = (luzMedida / 1023) * 100;
  if (luminosidade >= 80)
    digitalWrite(lampada, LOW);
  else
    digitalWrite(lampada, HIGH);

  //LEITURA DO ESPAÇO
  const float espacoEstufa = 40.0;// DEFINIR PELA ALTURA DA ESTUFA
  sensorUltrassonico = ultrassomEspaco.Ranging(CM); // MEDIDA EM CM
  float espacoDisponivel = (sensorUltrassonico / espacoEstufa) * 100;
  float espaco = 100 - espacoDisponivel;

 //LEITURA DO RESERVATÓRIO 
  const float espacoReservatorio = 30.0; // A DEFINIR  
  sensorUltrassonico2 = ultrassomReservatorio.Ranging(CM); // EM CM
  float reservatorio = (espacoReservatorio - sensorUltrassonico2); // RESERVATORIO EM PORCENTAGEM
  if(umidade > 80){
  }
  // MANDANDO OS DADOS PARA O ESP01
  Serial1.print(" ");
  Serial1.print(umidade);
        Serial.println("UMIDADE: "); // DEPURAÇÃO
        Serial.println(umidade); // DEPURAÇÃO
  Serial1.print(" ");
  Serial1.print(temperatura);
        Serial.println("TEMPERATURA: "); // DEPURAÇÃO
        Serial.println(temperatura); // DEPURAÇÃO
  Serial1.print(" ");
  Serial1.print(luminosidade);
        Serial.println("LUMINOSIDADE: "); // DEPURAÇÃO
        Serial.println(luminosidade); // DEPURAÇÃO
  Serial1.print(" ");
  Serial1.print(espaco);
       Serial.println("ESPACO OCUPADO EM %: "); // DEPURAÇÃO
       Serial.println(espaco); // DEPURAÇÃO
  Serial1.print(" ");
  Serial1.print(reservatorio);
       Serial.println("RESERVATORIO: "); // DEPURAÇÃO
       Serial.println(reservatorio); // DEPURAÇÃO
delay(2200);
}
