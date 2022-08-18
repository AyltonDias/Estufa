#include <Wire.h> // livraria do I2c
#include <LiquidCrystal_I2C.h> // biblioteca do LiquidCrystal


#define pino_trig 13
#define pino_echo  12
#define pino_led_vermelho 11
#define pino_led_amarelo 10
#define pino_led_verde 9
#define pino_led_sol 8
#define pino_led_standby 6
#define pino_buzina 5
#define pino_sinal_analogico_umidade A0
#define pino_sinal_analogico_ldr A1

long duracao, distancia;
int valor_analogico_umidade, valor_analogico_ldr;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2); // codigo do endereço do display


void setup()
{
  Serial.begin(9600);
  pinMode(pino_echo, INPUT);
  pinMode(pino_trig, OUTPUT);
  pinMode(pino_led_vermelho, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(pino_led_verde, OUTPUT);
  pinMode(pino_led_sol, OUTPUT);
  pinMode(pino_led_standby, OUTPUT);
  pinMode(pino_buzina, OUTPUT);
  pinMode(pino_sinal_analogico_umidade, INPUT);
  pinMode(pino_sinal_analogico_ldr, INPUT);

  //inicializa o LCD

  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);

  //Iniciando com LOW
  digitalWrite(pino_trig, LOW);
  digitalWrite(pino_echo, LOW);
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_verde, LOW);
  digitalWrite(pino_led_sol, LOW);
  digitalWrite(pino_led_standby, LOW);
  digitalWrite(pino_buzina, LOW);
  digitalWrite(pino_sinal_analogico_umidade, LOW); //Conferir se pode começar com LOW
  digitalWrite(pino_sinal_analogico_ldr, LOW); //Conferir se pode começar com LOW
}

void loop() {
  //long duracao, distancia;
  digitalWrite(pino_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(pino_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pino_trig, LOW);
  duracao = pulseIn(pino_echo, HIGH);
  distancia = (duracao / 2) * 0.034;

  if (distancia < 50) {

    digitalWrite(pino_led_standby, LOW);


    //Codigo placa
    //le os valores analogicos
    valor_analogico_umidade = analogRead(pino_sinal_analogico_umidade);
    valor_analogico_ldr = analogRead(pino_sinal_analogico_ldr);
    //Limpa a tela
    lcd.clear();
    //Mostra os valores no lcd
    lcd.setCursor(0, 0);
    lcd.print("Luminosidade: ");
    lcd.setCursor(0, 1);// 8 primeiros digitos da segunda linha
    lcd.print("LDR: ");
    lcd.setCursor(9, 1);//muda o cursor para o 9 digito da segunda linha
    lcd.print("Cm: ");

    //Mostra o valor da porta analogica no serial monitor

    Serial.print(" Sensor umidade: ");
    Serial.print(valor_analogico_umidade);
    Serial.print("\n");
    Serial.print(" Sensor LDR: ");
    Serial.print(valor_analogico_ldr);
    Serial.print("\n");
    Serial.print(" Distancia: ");
    Serial.print(distancia);
    Serial.print("\n");

    //mostrando no LCD
    lcd.setCursor(0, 0); // Set the cursor on the third column and first row.

    if (valor_analogico_ldr < 300) {
      Serial.println(" Status: Sem Luminosidade");
      //Limpa a tela
      lcd.clear();
      lcd.print("Sem Luminosidade");
      digitalWrite(pino_led_sol, HIGH);
    } else {
      digitalWrite(pino_led_sol, LOW);
    }

    //Solo umido, acende o led verde
    if (valor_analogico_umidade > 0 && valor_analogico_umidade < 350)
    {
      Serial.println(" Status: Solo umido");
      //Limpa a tela
      lcd.clear();
      lcd.print("Solo umido");
      apagaleds();
      digitalWrite(pino_led_verde, HIGH);
    } else if (valor_analogico_umidade > 350 && valor_analogico_umidade < 500)
    {
      Serial.println(" Status: Umidade moderada");
      //Limpa a tela
      lcd.clear();
      lcd.print("Umidade moderada");
      apagaleds();
      digitalWrite(pino_led_amarelo, HIGH);
    } else if (valor_analogico_umidade > 500 && valor_analogico_umidade < 800)
    {
      Serial.println(" Status: Solo seco");
      //Limpa a tela
      lcd.clear();
      lcd.print("Solo seco");
      apagaleds();
      digitalWrite(pino_led_vermelho, HIGH);
      digitalWrite(pino_buzina, HIGH);
    }
    //Fim codigo placa
  }
  else {
    //Limpa a tela
    apagaleds();
    lcd.clear();
    lcd.print("StandBy");
    Serial.print("Sistema em StandBy - ");
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");
    digitalWrite(pino_led_standby, HIGH);


    /*Manter o recurso de led sol funcionando*/
    valor_analogico_ldr = analogRead(pino_sinal_analogico_ldr);
    if (valor_analogico_ldr < 300) {
      digitalWrite(pino_led_sol, HIGH);
    } else {
      digitalWrite(pino_led_sol, LOW);
    }
    /*Manter o recurso de led sol funcionando*/
  }
  delay(1500);
}

void apagaleds()
{
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_verde, LOW);
  digitalWrite(pino_buzina, LOW);
}
