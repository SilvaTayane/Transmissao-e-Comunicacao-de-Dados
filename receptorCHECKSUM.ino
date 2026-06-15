#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RH_ASK rx(2000, 19, -1, -1);

LiquidCrystal_I2C lcd(0x27, 16, 2);

int calculaChecksum(String texto)
{
  int soma = 0;

  for (int i = 0; i < texto.length(); i++)
  {
    soma += texto[i];
  }

  return soma;
}

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");

  if (!rx.init())
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
  uint8_t buf[80];
  uint8_t buflen = sizeof(buf);

  if (rx.recv(buf, &buflen))
  {
    buf[buflen] = '\0';

    String quadro = String((char*)buf);

    Serial.print("Recebido: ");
    Serial.println(quadro);

    int p1 = quadro.indexOf('|');
    int p2 = quadro.lastIndexOf('|');

    if (p1 == -1 || p2 == -1 || p1 == p2)
    {
      Serial.println("Quadro invalido");
      return;
    }

    String tipo = quadro.substring(0, p1);
    String mensagem = quadro.substring(p1 + 1, p2);
    int checksumRecebido = quadro.substring(p2 + 1).toInt();

    int checksumCalculado = calculaChecksum(mensagem);

    Serial.print("Checksum recebido: ");
    Serial.println(checksumRecebido);

    Serial.print("Checksum calculado: ");
    Serial.println(checksumCalculado);

    if (checksumRecebido == checksumCalculado)
    {
      Serial.println("CHECKSUM OK");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mensagem:");

      lcd.setCursor(0, 1);
      lcd.print(mensagem.substring(0, 16));
    }
    else
    {
      Serial.println("ERRO CHECKSUM");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ERRO");

      lcd.setCursor(0, 1);
      lcd.print("CHECKSUM");
    }

    Serial.println();
  }
}