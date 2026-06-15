#include <RH_ASK.h>
#include <SPI.h>

RH_ASK tx(2000, -1, 18, -1);
// velocidade, RX, TX, PTT

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

  if (!tx.init())
  {
    Serial.println("Erro ao iniciar RF");
  }
  else
  {
    Serial.println("Transmissor pronto");
  }
}

void loop()
{
  String mensagem = "OLA MUNDO";

  int checksum = calculaChecksum(mensagem);

  String quadro = "DATA|" + mensagem + "|" + String(checksum);

  tx.send((uint8_t*)quadro.c_str(), quadro.length());
  tx.waitPacketSent();

  Serial.print("Enviado: ");
  Serial.println(quadro);

  delay(2000);
}