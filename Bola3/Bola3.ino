/*Código do módulo que fica dentro da bola de borracha


*/
#include <Firmata.h>
#include <VirtualWire.h>

int accX;
int MinX = 145;        // minimum sensor value
int MaxX = 435;        // maximum sensor value
int accY;
int MinY = 114;
int MaxY = 399;
int PrevX;
int PrevY;
//int PrevZ;
int dir_X;
int dir_Y;

int SensibilidadeX = 10;
int SensibilidadeY = 10;
//int SensibilidadeZ=6;

char *msg;

void analogWriteCallback(byte pin, int value)
{
  if (IS_PIN_PWM(pin)) {
    pinMode(PIN_TO_DIGITAL(pin), OUTPUT);
    analogWrite(PIN_TO_PWM(pin), value);
  }
}

void setup()
{
  Firmata.setFirmwareVersion(0, 1);
  Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  Firmata.begin(57600);
  Serial.begin(9600);	// Debugging only
  Serial.println("setup");

  //captura valores iniciais dos PrevX,PrevY,PrevZ
  accX = analogRead(A0);
  accX = map(accX, MinX, MaxX, 0, 255);
  accX = constrain(accX, 0, 255);
  //Eixo Y
  accY = analogRead(A1);
  accY = map(accY, MinY, MaxY, 0, 255);
  accY = constrain(accY, 0, 255);
  //Eixo Z
  //accZ = analogRead(A2);
  //accZ = map(accZ, MinZ, MaxZ, 0, 255);
  //accZ = constrain(accZ, 0, 255);

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
  //  vw_set_tx_pin(12);
}

void loop()
{
  while (Firmata.available()) {
    Firmata.processInput();
  }

  //Eixo X
  PrevX = accX; //valor previo assume o valor atual
  accX = analogRead(A0);
  accX = map(accX, MinX, MaxX, 0, 255);
  accX = constrain(accX, 0, 255);
  //Eixo Y
  PrevY = accY; //valor previo assume o valor atual
  accY = analogRead(A1);
  accY = map(accY, MinY, MaxY, 0, 255);
  accY = constrain(accY, 0, 255);
  //Eixo Z
  //PrevZ = accZ; //valor previo assume o valor atual
  //accZ = analogRead(A2);
  //accZ = map(accZ, MinZ, MaxZ, 0, 255);
  //accZ = constrain(accZ, 0, 255);

  //processa camando para o arduino Globo
  if (abs(accX - PrevX) > SensibilidadeX)
  {
    if (accX > PrevX)
      dir_X = 1;
    else
      dir_X = -1;
  } else dir_X = 0;

  if (abs(accY - PrevY) > SensibilidadeY)
  {
    if (accY > PrevY)
      dir_Y = 1;
    else
      dir_Y = -1;
  } else dir_Y = 0;

  //interpreta direcoes
  if (dir_X == -1 and dir_Y == 0)
    msg = "N ";
  else if ( dir_X == 1 && dir_Y == 0 )
    msg = "S ";
  else if ( dir_X == 0 && dir_Y == 1 )
    msg = "L ";
  else if ( dir_X == 0 && dir_Y == -1 )
    msg = "O ";
  else if ( dir_X == -1 && dir_Y == 1 )
    msg = "NE";
  else if ( dir_X == 1 && dir_Y == 1 )
    msg = "SE";
  else if ( dir_X == 1 && dir_Y == -1 )
    msg = "SO";
  else if ( dir_X == -1 && dir_Y == -1 )
    msg = "NO";
  else if ( dir_X == 0 && dir_Y == 0 )
    msg = "XX";

  //envia para o Processing via Bluetooth
  Firmata.sendAnalog(0, accX);
  Firmata.sendAnalog(1, accY);

  Serial.print("mensagem: |");
  Serial.print(msg);
  Serial.println("|");

  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx();
  delay(500);
}
