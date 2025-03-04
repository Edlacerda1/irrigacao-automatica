#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

// Declarando variaveis dos sensores e seus tipos
int umidade = 0;
float temperatura = 0;

// Configurando arquitetura do sistema
void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(7, OUTPUT);
  
  // Iniciando o LCD 
  lcd.init();
  lcd.clear();
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {
  digitalWrite(A0, HIGH); 
  delay(10);

  
  int leitura = analogRead(A2); // Leitura da temperatura no pino A2
  
  float tensao = leitura * 5.0; // Converte a leitura para tensão
  tensao /= 1024.0; 
  
  temperatura = (tensao - 0.5) * 100; // Converte em °C

  // Média de 5 leituras para reduzir ruído
  int umidadeSum = 0;

  for (int i = 0; i < 5; i++) {
    umidadeSum += analogRead(A1);
    delay(100); 
  }

  umidade = umidadeSum / 5;
  float umidadePor = (umidade / 1023.0) * 100.0;

  // Exibe no monitor serial
  Serial.print("Umidade: ");
  Serial.print(umidadePor);
  Serial.print("% | Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println("°C");

  // Exibe no LCD
  lcd.setCursor(0, 0); 
  lcd.print("Umidade: ");
  lcd.print(umidadePor, 2);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperatura, 2);
  lcd.print(" C");

  // Ativa bomba se a umidade for baixa e a temperatura estiver alta
  if (umidadePor < 30 && temperatura > 23) {
    digitalWrite(7, HIGH);
    Serial.println("Ativando bomba!");
    delay(5000); // Mantém a bomba ligada por 5 segundos
  } else {
    digitalWrite(7, LOW);
  }

  delay(1000);
}
