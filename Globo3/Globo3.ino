/*Código do módulo que fica dentro do Globo Terrestre
 * 
 * 
 */
#include "ServoTimer2.h"
#include <VirtualWire.h>

ServoTimer2 Servo_rotacao;
ServoTimer2 Servo_pendulo;

char cmd[2] = {'X', 'X'};
String comando;
int angulo_esquerda = 2160;
int angulo_centro = 2060;
int angulo_direita = 1960;

void setup()
{
  delay(2000);
  Servo_rotacao.attach(3);
  Servo_pendulo.attach(4);
  Serial.begin(9600);	
  Serial.println("setup");

  vw_set_ptt_inverted(true); // Requerido para Módulo de Link RX
  vw_setup(2000);	 // Bits por segundos
  vw_rx_start();   // Start the receiver PLL running

  Servo_pendulo.write(angulo_centro);
  Servo_rotacao.write(1500);  // Stop
  Serial.println("Listening...");
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    int i;
    for (i = 0; i < buflen; i++)
    {
      cmd[i] = buf[i];
    }

    comando = String(cmd[0]) + String(cmd[1]);
    Serial.print(" c: |");
    Serial.print(comando);
    Serial.println("|");

    if (comando == "XX")
    {
      Servo_pendulo.write(angulo_centro);  // pendulo no centro
      Servo_rotacao.write(1500);  // parar
    }
    else if (comando == "N ")
    {
      Servo_pendulo.write(angulo_centro);  // pendulo no centro
      Servo_rotacao.write(1300);  // sentido um
    }
    else if (comando == "S ")
    {
      Servo_pendulo.write(angulo_centro);  // pendulo no centro
      Servo_rotacao.write(1700);  // sentido dois
    }
    else if (comando == "L ")
    {
      Servo_pendulo.write(angulo_esquerda);  // pendulo à esquerda
    }
    else if (comando == "O ")
    {
      Servo_pendulo.write(angulo_direita);  // pendulo à direita
    }
    else if (comando == "NE")
    {
      Servo_pendulo.write(angulo_esquerda);
      Servo_rotacao.write(500);
    }
    else if (comando == "SE")
    {
      Servo_pendulo.write(angulo_esquerda);
      Servo_rotacao.write(2000);
    }
    else if (comando == "NO")
    {
      Servo_pendulo.write(angulo_direita);
      Servo_rotacao.write(500);
    }
    else if (comando == "SO")
    {
      Servo_pendulo.write(angulo_direita);
      Servo_rotacao.write(2000);
    }
  }
}
