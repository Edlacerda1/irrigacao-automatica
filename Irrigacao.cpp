#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Declarando variaveis dos sensores e seus tipos
int umidade = 0;
#define dados 4

int seco = 650;  // Variaveis para condicional 
int molhado = 300; 

OneWire OneWire(dados);
DallasTemperature sensors(&OneWire);

// Configurando arquitetura do sistema
void setup() {

  pinMode(A0, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
  
  // Iniciando o LCD 
  lcd.init();
  lcd.clear();
  lcd.backlight();

  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  digitalWrite(A0, HIGH); 
  delay(10);

  int leitura = 0;

  sensors.requestTemperatures();
  float temperatura = sensors.getTempCByIndex(0);
  //Média de 3 leituras para reduzir ruído
  int umidadeSum = 0;

  for (int i = 0; i < 3; i++) {
    umidadeSum += analogRead(A1);
    delay(100); 
  }

  leitura = umidadeSum / 3;

  // leitura == seco; 0; leitura == molhado; 100;
  int umidade = map(leitura, seco, molhado, 0, 100);

  // Limita o valor de 0 a 100
  umidade = constrain(umidade, 0, 100);

  // Exibe no monitor serial
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("% | Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");

  // Exibe no LCD
  lcd.setCursor(0, 0); 
  lcd.print("Umidade: ");
  lcd.setCursor(9, 0); 
  lcd.print(umidade);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print(" C");

  // Ativa bomba se a umidade for baixa e a temperatura estiver alta
  if (umidade < 40 && temperatura > 23) {
    digitalWrite(13, !HIGH);
    Serial.println("Ativando bomba!");
    delay(5000); // Mantém a bomba ligada por 5 segundos
  } else {
    digitalWrite(13, !LOW);
  }
  delay(5000);
}
