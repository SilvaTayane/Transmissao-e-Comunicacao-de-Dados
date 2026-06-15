#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RH_ASK rx(2000, 19, -1, -1);

LiquidCrystal_I2C lcd(0x27,16,2);

String imagem[8];

int calculaChecksum(String texto)
{
  int soma = 0;

  for(int i = 0; i < texto.length(); i++)
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
  uint8_t buf[80];
  uint8_t buflen = sizeof(buf);

  if(rx.recv(buf,&buflen))
  {
    buf[buflen] = '\0';

    String quadro = String((char*)buf);

    Serial.println();
    Serial.print("Recebido: ");
    Serial.println(quadro);

    // ==========================
    // TEXTO
    // ==========================

    if(quadro.startsWith("TXT|"))
    {
      int p1 = quadro.indexOf('|');
      int p2 = quadro.lastIndexOf('|');

      String mensagem =
      quadro.substring(p1 + 1, p2);

      int checksumRecebido =
      quadro.substring(p2 + 1).toInt();

      int checksumCalculado =
      calculaChecksum(mensagem);

      if(checksumRecebido ==
         checksumCalculado)
      {
        Serial.println("CHECKSUM OK");

        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("Mensagem:");

        lcd.setCursor(0,1);
        lcd.print(mensagem);
      }
      else
      {
        Serial.println("ERRO CHECKSUM");

        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("ERRO");

        lcd.setCursor(0,1);
        lcd.print("CHECKSUM");
      }
    }

    // ==========================
    // IMAGEM
    // ==========================

    if(quadro.startsWith("IMG|"))
    {
      int p1 = quadro.indexOf('|');
      int p2 = quadro.lastIndexOf('|');

      int linha =
      quadro.substring(p1+1,p2).toInt();

      String dados =
      quadro.substring(p2+1);

      imagem[linha] = dados;

      bool completa = true;

      for(int i = 0; i < 8; i++)
      {
        if(imagem[i] == "")
        {
          completa = false;
        }
      }

      if(completa)
      {
        Serial.println();
        Serial.println("IMAGEM RECONSTRUIDA");
        Serial.println();

        for(int i = 0; i < 8; i++)
        {
          for(int j = 0; j < 8; j++)
          {
            if(imagem[i][j] == '1')
            {
              Serial.print("#");
            }
            else
            {
              Serial.print(" ");
            }
          }

          Serial.println();
        }

        Serial.println();

        for(int i = 0; i < 8; i++)
        {
          imagem[i] = "";
        }
      }
    }
  }
}