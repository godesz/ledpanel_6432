#include <PxMatrix.h>

#ifdef ESP8266

#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#define snake_max 200
#define panelW 64
#define panelH 32
#define apple_number 10

#endif
// Pins for LED MATRIX

//PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
PxMATRIX display(panelW, panelH, P_LAT, P_OE, P_A, P_B, P_C, P_D);
//PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);


unsigned long time_now, time_start = millis();
int ssx = 10, ssy = 10, ssc1 = 100, ssc2 = 100, ssc3 = 100;
int i;
char snake_body[snake_max][2], apple_pos[apple_number][2];
int snake_size = 5;
    
#ifdef ESP8266
// ISR for display refresh
void display_updater()
{
  //display.displayTestPattern(70);
  // display.displayTestPixel(70);
  display.display(70);
}
#endif



uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCOLOR = display.color565(0, 55, 155);

void print_score(int score)
{
  //display.clearDisplay();
  display.setTextColor(myCOLOR);
  display.setCursor(6, 7);
  if (score < 10) display.print("  ");
  else if (score < 100) display.print(" ");
  display.print(String(score));

  //display.print(":");
  //display.print(String(55));
}
void draw_time()
{
  int hossz = (millis() - time_start) / 1000;
  Serial.println(hossz);
  //display.drawLine(hossz,28,hossz,32,myGREEN);
  if (hossz < 40)  display.fillRect(0, 28, hossz, 32, myGREEN);
  if (hossz > 50)
  {
    display.fillRect(0, 28, 39, 32, myGREEN);
    display.fillRect(40, 28, 50, 32, myYELLOW);
    display.fillRect(51, 28, hossz, 32, myRED);
  }
  if ((hossz <= 50) && (hossz >= 40))   {
    display.fillRect(0, 28, 39, 32, myGREEN);
    display.fillRect(39, 28, 44, 32, myYELLOW);
  }

  if (hossz > 60) time_start = millis();
}

void screensaver()
{
  int dx = 0, dy = 0;
  if (random(0, 100) % 2 == 0) dx = random(0, 2) * 2 - 1;
  else dy = random(0, 2) * 2 - 1;

  ssx = (ssx + dx) % panelW;
  ssy = (ssy + dy) % panelH;

  if (ssx < 0) ssx = panelW - 1;
  if (ssy < 0) ssy = panelH - 1;

  ssc1 = (ssc1 + random(0, 20) - 10) % 255;
  ssc2 = (ssc2 + random(0, 20) - 10) % 255;
  ssc3 = (ssc3 + random(0, 20) - 10) % 255;

  display.drawPixel(ssx, ssy, display.color565(ssc1, ssc2, ssc3));
  Serial.print(ssx);
  Serial.print(" ");
  Serial.print(ssy);
  Serial.print("\n");
}
void reset_snake()
{
  delay(1500);
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(myRED);
  display.setCursor(1, 7);
  display.print("GAME OVER");
  delay(1000);
  
  display.clearDisplay();
  display.setTextColor(myGREEN);
  display.setCursor(1, 6);
  display.print("score:");
  display.setCursor(1, 15);
  display.print(snake_size-5);
  delay(1000);
  
  display.clearDisplay();
  
for(i = 0; i < snake_max; i++)
    {
      snake_body[i][0] = 0;
      snake_body[i][1] = 0;
    }
snake_size = 5;
snake_body[snake_size][0] = random(0,panelW);
snake_body[snake_size][1] = random(0,panelH);
for(int k=0;k<apple_number;k++)
{
apple_pos[k][0] =random(0,panelW);
apple_pos[k][1] =random(0,panelH);

display.drawPixel(apple_pos[k][0], apple_pos[k][1], display.color565(255, 0, 0)); // put apple
}
                       
  Serial.println("Snake dead.");
}

void snake_step()
{
  int dx = 0, dy = 0, newhead = 0,j, newx, newy, lookouts = 0;
  /*if (random(0, 100) % 2 == 0) dx = random(0, 2) * 2 - 1;
                          else dy = random(0, 2) * 2 - 1;*/
  
  while(1)
  {
    lookouts++;
    if (lookouts > 20) reset_snake();
    newhead = 0;
    Serial.println(newhead);
  dx = 0; 
  dy = 0;
  if (random(0, 100) % 2 == 0) dx = random(0, 2) * 2 - 1;
                          else dy = random(0, 2) * 2 - 1;
  newx = (snake_body[snake_size][0] + dx) % panelW;
  newy = (snake_body[snake_size][1] + dy) % panelH;
  if (newx < 0) newx = panelW - 1;
  if (newy < 0) newy = panelH - 1;
  
  for(j = 0; j<snake_size; j++)
      {
         if (( newx == snake_body[j][0]) &&
            ( newy== snake_body[j][1]) )newhead = 1;
      } //j
  if (newhead == 0) break;
  }
  char got_apple = 0;
  for(int k=0;k<apple_number;k++)
  if ((apple_pos[k][0] == newx) && (apple_pos[k][1] == newy))
                      {
                        display.drawPixel(apple_pos[k][0], apple_pos[k][1], display.color565(0, 0, 0)); // remove apple
                        snake_size++;
                        apple_pos[k][0] =random(0,panelW);
                        apple_pos[k][1] =random(0,panelH);
                        display.drawPixel(apple_pos[k][0], apple_pos[k][1], display.color565(255, 0, 0)); // put new apple
                        got_apple = 1;
                      }
                      
  //if (got_apple == 0)
    display.drawPixel(snake_body[0][0], snake_body[0][1], display.color565(0, 0, 0)); // remove last

  for (i = 0; i < snake_size; i++)
  {
    snake_body[i][0] = snake_body[i + 1][0];
    snake_body[i][1] = snake_body[i + 1][1];
  }

  snake_body[snake_size][0] = (snake_body[snake_size][0] + dx) % panelW;
  snake_body[snake_size][1] = (snake_body[snake_size][1] + dy) % panelH;

  if (snake_body[snake_size][0] < 0) snake_body[9][0] = panelW - 1;
  if (snake_body[snake_size][1] < 0) snake_body[9][1] = panelH - 1;

  ssc1 = (ssc1 + random(0, 20) - 10) % 255;
  ssc2 = (ssc2 + random(0, 20) - 10) % 255;
  ssc3 = (ssc3 + random(0, 20) - 10) % 255;

  display.drawPixel(snake_body[snake_size][0], snake_body[snake_size][1], display.color565(ssc1, ssc2, ssc3)); // insert new
  Serial.print(snake_body[snake_size][0]);
  Serial.print(" ");
  Serial.print(snake_body[snake_size][1]);
  Serial.print("\n");
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin(16);

  display.setMuxPattern(SHIFTREG_ABC_BIN_DE);
  display.flushDisplay();
  display.setTextSize(2);
  /*
    display.setTextColor(myCYAN);
    display.setCursor(2,0);
    display.print("Pixel");
  */

#ifdef ESP8266
  display_ticker.attach(0.004, display_updater);
#endif
for(i = 0; i < 10; i++)
    {
      snake_body[i][0] = 0;
      snake_body[i][1] = 0;
    }
for(int k=0;k<apple_number;k++)
{
apple_pos[k][0] =random(0,panelW);
apple_pos[k][1] =random(0,panelH);

display.drawPixel(apple_pos[k][0], apple_pos[k][1], display.color565(255, 0, 0)); // put apple
}
  delay(1000);
}
/*
   drawPixel
   setBrightness
   clearDisplay
*/
int pont = 0;

void loop() {
  //display.clearDisplay();
  //print_score(pont);
  //draw_time();
  //if (random(0,10) == 1) pont++;
  //screensaver();
  snake_step();
  delay(25);
}
