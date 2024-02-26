#pragma GCC optimize ("Ofast")
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028.h"

LGFX gfx; 
LGFX_Sprite sMeterSprite(&gfx);
LGFX_Sprite sNeedleSprite(&gfx);

#define LGFX_VERTICAL_MODE    0
#define LGFX_HORIZONTAL_MODE  1

// Dimensioni dello schermo
int w = 320;
int h = 240;

// Coordinate del cardine dell'ago
int cx = 160;
int cy = 240;

//Posizione in gradi di inizio della scala lineare
int a0 = 225;
//Posizione in gradi di fine della scala lineare (fino S9)
int aS9 = 280;
//Posizione in gradi di fine della scala (+60db)
int a1 = 315;

// imposta i pixel di spostamento dell'ago per i passi larghi
int iNeedlePixelLargeMove = 3;
// il valore minimo di lettura che equivale a 0
int minAnalogValue = 680;
// il valore minimo di lettura che equivale a 9+60
int maxAnalogValue = 4095;

float ranlg[10] = {27.0,   215.0, 640.0, 1350.0, 2130.0, 2705.0, 3125.0, 3460.0, 3760.0, 4000.0};
float rsign[10] = {0.0,     1.0,   2.0,    3.0,    5.0,    7.0,    9.0,   10.0,   12.0,   15.0};
float alpha[10];

// Disegna lo sfondo del SMeter (Scala, ecc...)
void drawSMeter(LGFX_Sprite & sprt)
{
// Tutte le funzioni di disegno sono disponibili come nel display LCD.
// I colori possono essere specificati allo stesso modo dell'LCD, ma vengono automaticamente convertiti in scala di grigi.
// (Il rapporto durante la conversione in scala di grigi è R1:G2:B1. Il verde è espresso leggermente più luminoso.)


  sprt.setColorDepth(8);
  sprt.createSprite(320,240);
  sprt.fillSprite(TFT_WHITE);

  sprt.setPivot(160, 120);

  sprt.fillArc(cx, cy, 206, 200 - 1, 280, a1, TFT_RED); // Arco (parziale) di misurazione rosso (in alto)
  sprt.fillArc(cx, cy, 200, 200 - 1, a0, a1, TFT_BLACK); // Arco di misurazione esterno (in alto)
  sprt.fillArc(cx, cy, 197, 197 - 1, a0, a1, TFT_BLACK); // Arco di misurazione interno (in alto)
  
  int deltaS9 = (aS9 - a0) / 9; // Da S0 a S9 i livelli sono lineari, dopo S9 diventano logaritmici
  int deltaS = (a1 - a0) / 14;  // Da S0 a +60 
  int scale = 0;                // La scala di S (Santiago) da 0 a 9
  int xt = 0;                   // coordinate x e y dei punti di ancoraggio per il teso
  int yt = 0;                   //  -translati da "cartesiani" in mappa del display
  float radAngle = 0.00;        // le funzioni sin() e cos() accettano parametro in radianti

  for (int ai = a0; ai <= a1; ai += deltaS) // Ciclo gli angoli da S0 a S9
  {
      radAngle = (float)ai * (M_PI) /180.0;                 // Trasformo gradi degli angoli in radianti

      xt = (223.0 * cos(radAngle)) + cx;               // Traslo x da "Cartesiano" a mappa LCD
      yt = abs((-1 * (223.0 * sin(radAngle))) - cy);   // Traslo y da "Cartesiano" a mappa LCD

    if ((scale % 2) || (scale == 0)) // Individuo 0 e i valori dispari
    {
      sprt.fillArc(cx, cy, 208, 200, ai, ai + 1, TFT_BLACK);  // Segni superiori lunghi neri  
      sprt.fillArc(cx, cy, 197, 192, ai, ai + 1, TFT_BLACK);  // Segni inferiori corti

      if (scale < 10)
      {
        sprt.setFont(&fonts::FreeSansBold9pt7b);
        sprt.setTextColor(TFT_BLACK);
        sprt.drawNumber(scale, xt, yt);
      }
    }
    else
    {
      sprt.fillArc(cx, cy, 205, 200, ai, ai + 1, TFT_BLACK);  // Segni superiori corti neri  
    }

    if (scale > 9)
    {
      //sprt.fillArc(cx, cy, 205, 200, ai, ai + 1, TFT_BLACK);     // S0
      sprt.setTextColor(TFT_RED);

      Serial.print("scale: ");
      Serial.println(scale);

      switch (scale)
      {
        case (11):
          sprt.drawString("+10", xt - 20, yt - 5, &fonts::FreeMonoBold9pt7b);
          break;
        case (13):
          sprt.drawString("+30", xt - 25, yt - 10, &fonts::FreeMonoBold9pt7b);
          break;
        case (15):
          sprt.drawString("+60", xt - 30, yt - 20, &fonts::FreeMonoBold9pt7b);
          break;
      }
    }

    scale++;

  }

  sprt.setTextColor(TFT_BLUE);
  sprt.drawString("IU8NQI", 120 , 100,&fonts::FreeMonoBold12pt7b);

  sprt.setTextColor(TFT_BLACK);
  sprt.drawString("SIGNAL", 90 , 140,&fonts::FreeSansBold18pt7b);

  sprt.fillArc(cx, cy, 0, 40, 180, 360, TFT_DARKGRAY); // sede dell'ago
      
}

void drawNeedle(LGFX_Sprite & sprt, int lenght, int radius)
{
  sprt.setColorDepth(8);
  sprt.createSprite(5, lenght);  // create the needle Sprite 11 pixels wide by 30 high

  sprt.fillSprite(TFT_WHITE); // Fill with bianco

  // Define needle pivot point
  //uint16_t piv_x = needle.width() / 2;   // pivot x in Sprite (middle)
  uint16_t piv_x = sprt.width() / 2;   // pivot x in Sprite (middle)
  uint16_t piv_y = radius;        // pivot y in Sprite
  sprt.setPivot(piv_x, piv_y);         // Set pivot point in this Sprite

  // Draw the red needle with a yellow tip
  // Keep needle tip 1 pixel inside dial circle to avoid leaving stray pixels
  // Disegna l'ago (senza punta)
  sprt.fillRect(piv_x - 1, 2, 3, lenght - 10, TFT_RED);
  // Disegna la punta dell'ago
  sprt.fillRect(piv_x - 1, 2, 3, 10, TFT_YELLOW);
}

void plotNeedle(LGFX_Sprite & background, int angle, byte ms_delay)
{
  static int16_t old_angle = 0;
  int8_t needlePixelMove = 1; 
  //angle = angle + 80;
  angle -= 45; // Starts at -120 degrees

  // Muove l'ago fino al raggiungimento della posizione
  while (angle != old_angle) {

    if (abs(old_angle - angle) < 10)
      needlePixelMove = 1;                      // movimento lento
    else
      needlePixelMove = iNeedlePixelLargeMove;  // movimento veloce

    if (old_angle > angle) 
      needlePixelMove = (-needlePixelMove);
    
    // Imposta la posizione corrente dell'ago
    old_angle += needlePixelMove;

    // Ridisegna lo sfondo
    background.pushSprite(0,0);
    // Disegna l'ago nella nuova posizione
    sNeedleSprite.pushRotated(old_angle, TFT_WHITE);
    // fa una brevissima pausa prima di muovere nuovamente l'ago
    delay(ms_delay);
  }
}

float getSignByAnalog(float anlg)
{
  float ret = 0.0;
  //float alphaTop = 0.00;
  float alphaPrev = 0.00;
  float deltaVbase = 0.00;
  float deltaAdiff = 0.00;
  float deltaVXdiff = 0.00;
  float deltaAXdiff = 0.00;
  float alphaX = 0.00;

  //Serial.println(ranlg[9]);

  if (anlg <= ranlg[9])
    for (int i = 0; i < 10; i++) 
    {
      if (anlg <= ranlg[i])
      {
        //Serial.print(i);
        //Serial.print(" = ");
        //Serial.println(anlg);
        alphaPrev = alpha[i-1];
        //alphaTop = alpha[i];
        deltaVbase = ranlg[i] - ranlg[i-1];
        deltaAdiff = alpha[i] - alpha[i-1];
        deltaVXdiff = anlg - ranlg[i-1];
        deltaAXdiff = deltaVXdiff * deltaAdiff / deltaVbase;
        alphaX = alphaPrev + deltaAXdiff;
        ret = anlg * alphaX;
        int a = ret * 100;
        ret = (float)a / 100;
        break;
      }
    }
  else{
    ret = rsign[9];
  }

  if (ret > rsign[9])
  {
    ret = rsign[9];
  }
  return ret;
}

void SetupAlpha(){
  alpha[0] = 0.0000;
  for (int i = 1; i < 10; i++) 
  {
    alpha[i] = rsign[i] / ranlg[i];
  }
}

void setup() {
  //pinMode(35, INPUT);
  Serial.begin(9600);

  // Inizializza il display
  gfx.init();
  // Ruota il display in orizzondale
  gfx.setRotation(LGFX_HORIZONTAL_MODE);

  // Disegna lo sfondo del SMeter
  drawSMeter(sMeterSprite);
  //sMeterSprite.pushSprite(0,0); // rimuovere
  
  // Disegna l'ago
  drawNeedle(sNeedleSprite, 188, 208);

  // Imposta il centro di rotazione dell'ago
  gfx.setPivot(cx, cy);  

  // Visualizza lo sprite dello sfondo del SMeter
  //sMeterSprite.pushSprite(0,0);
  // Visualizzo lo sprite dell'ago in posizione 0
  plotNeedle(sMeterSprite, 0, 1);
  delay(500);
  // Sposto l'ago in posizione fondo scala
  plotNeedle(sMeterSprite, 90, 1);
  delay(500);
  // Riporto l'ago in posizione 0
  plotNeedle(sMeterSprite, 0, 1);
  delay(250);

  SetupAlpha();
  
  
}

uint16_t readSignal()
{
  int r = 0;
  for (int i = 0; i<500; i++)
  {
      r += analogRead(35);
  }
  return (r/500);
}

void loop() {
  
  
  // test
  /*
  int s = readSignal();
  Serial.println(s);
  uint16_t angle = (uint16_t)(90.0 * s / 3000);
  plotNeedle(sMeterSprite, angle, 1);
  delay(50);
  */
  
  
  
  
  
  int s = readSignal();
  Serial.print("anlg: ");
  Serial.print(s);
  
  float sign = getSignByAnalog((float)s);

  Serial.print(" - sign: ");
  Serial.println(sign);

  uint16_t angle = (uint16_t)(90.0 * sign / 15.0);
  plotNeedle(sMeterSprite, angle, 1);
  




}