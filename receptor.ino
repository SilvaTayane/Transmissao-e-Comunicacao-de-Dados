#include <RH_ASK.h>
#include <SPI.h>

RH_ASK tx(2000, -1, 18, -1);

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
}

void loop()
{
  const char *msg = "DATA|OLA MUNDO";

  tx.send((uint8_t*)msg, strlen(msg));
  tx.waitPacketSent();

  Serial.println("Enviado: DATA|OLA MUNDO");

  delay(2000);
}