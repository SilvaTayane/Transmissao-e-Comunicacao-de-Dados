#include <RH_ASK.h>
#include <SPI.h>

RH_ASK tx(2000, -1, 18, -1);

String imagem[8] =
{
  "00111100",
  "01000010",
  "10100101",
  "10000001",
  "10100101",
  "10011001",
  "01000010",
  "00111100"
};

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

  if(!tx.init())
  {
    Serial.println("Erro RF");
  }
  else
  {
    Serial.println("Transmissor pronto");
  }

  delay(2000);
}

void loop()
{
  // ==========================
  // ENVIO DE TEXTO
  // ==========================

  String mensagem = "OLA MUNDO";

  int checksum = calculaChecksum(mensagem);

  String quadro =
  "TXT|" +
  mensagem +
  "|" +
  String(checksum);

  tx.send((uint8_t*)quadro.c_str(), quadro.length());
  tx.waitPacketSent();

  Serial.println("Texto enviado:");
  Serial.println(quadro);

  delay(5000);

  // ==========================
  // ENVIO DA IMAGEM
  // ==========================

  for(int i = 0; i < 8; i++)
  {
    String linha =
    "IMG|" +
    String(i) +
    "|" +
    imagem[i];

    tx.send((uint8_t*)linha.c_str(), linha.length());
    tx.waitPacketSent();

    Serial.println(linha);

    delay(1000);
  }

  delay(10000);
}