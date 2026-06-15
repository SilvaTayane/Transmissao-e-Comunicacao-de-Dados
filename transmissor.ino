#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RH_ASK rx(2000, 19, -1, -1);

LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.print("Iniciando");

  if(!rx.init())
  {
    Serial.println("Erro RF");
  }
  else
  {
    Serial.println("Receptor pronto");
  }
}

void loop()
{
  uint8_t buf[50];
  uint8_t buflen = sizeof(buf);

  if(rx.recv(buf, &buflen))
  {
    // Garante string terminada corretamente
    if(buflen < 50) {
      buf[buflen] = '\0';
    } else {
      buf[49] = '\0';
    }

    String msg = String((char*)buf);

    Serial.print("Recebido: ");
    Serial.println(msg);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mensagem:");

    lcd.setCursor(0,1);
    
    // Verifica se a mensagem tem pelo menos 5 caracteres
    if(msg.length() > 5) {
      lcd.print(msg.substring(5));
    } else {
      lcd.print(msg);  // Imprime tudo se for mais curta
    }
  }
}