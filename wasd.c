
#include <stdint.h>   // Declarations of uint_32 and the like 
#include <stdlib.h>
#include <pic32mx.h>  // Declarations of system-specific addresses etc 
#include "functiondefinitions.h"  // Declarations for functions 



int direction_gun=0;        //0=upp, 1=höger, 2=ned, 3=vänster, 4=upp-höger, 5=ned-höger, 6=ned-vänster, 7=upp-vänster
int kula[3][100];   //kula[0]==x, kula[1]==y, kula[2]==riktning
int fiendekoordinat[3][100];    //fiendes koordinater, fiendekoordinat[0][i]=x, fiendekoordinat[1][i]=y, fiendekoordinat[2][i]=(NULL, 1 eller 2 där 1: vänster och 2: höger, NULL innebär att fienden är död och skulle då kunna återanvändas)
int lost=0;                  //0 betyder att man lever och 1 att spelet är förlorat och att man bör gå vara i menyn
int score=0;                  //highscore som sedan ska sparas
int variabelTillRandom=76;    //variabel som förändras och sedan blir till synes random då den beror på input

// C har inte public

void initdetmesta()
{
  for(int i2=0;i2<=2;i2++)
  {
    for(int i=0;i<=99;i++)
    {
      kula[i2][i]=1000;
      fiendekoordinat[i2][i]=1000;
    }
  }
}

int tillSynesrandom(int max)            //ger till synes random int mellan 0 och max
{
  variabelTillRandom=(variabelTillRandom % max);
  return variabelTillRandom;
}
/*
--display--
bredd: 128
höjd: 32
-----------
*/


/*
-------------------
Button Inputs
By: Isak Erikson Winbäck
Modified by: Simon Svanberg 
modifieringen gäller bara ändringar av tex: 4 till 0x4 av någon anledning samt av spaktryck så att den använder en klass i stället för kula[][]
*/

// bit1 == bit2 is not valid in C. Use bit1 & bit2 instead.
int x_mainCharacter=64;     //fixa till logisk start sen
int y_mainCharacter=14;     //fixa till logisk start sen
void knapptryck()
{
  variabelTillRandom++;
    int buttons = getbtns();
    int btn1 = getbtn1();
    if (0x4 & buttons) {      // Button 4 (mapped to RD11)
        x_mainCharacter--;     
    }
    if (0x2 & buttons) {      // Button 3 (mapped to RD10)
        y_mainCharacter--;    
    }
    if (0x1 & buttons) {      // Button 2 (mapped to RD9)
        y_mainCharacter++;    
    }
    if (0x8 & btn1) {      // Button 1 (mapped to RF1)
        x_mainCharacter++;    
    }
}

// Test shooting with switches
// On switch activation, spawn a "bullet" in the direction of the switch
int bullet_x, bullet_y;
extern const uint8_t filled_square[][5];
// bullet_init
void bullet_init() {
    bullet_x = x_mainCharacter;
    bullet_y = y_mainCharacter;
}

int should_spawn_bullet() {
  return 1;
}


int bullet_direction;  // Default direction
// spaktryck
void spaktryck(int* x_speed, int* y_speed) {
    int switches = getsw();
    should_fire_bullet = 0;
    *x_speed = 0;
    *y_speed = 0;
    if (0x8 & switches) {      // Switch 4 (mapped to RD3)
        //vänster  
        *x_speed = DIRECTION_LEFT;
        should_fire_bullet = 1;
    }
    if (0x4 & switches) {      // Switch 3 (mapped to RD2)
        //upp
        *y_speed = DIRECTION_UP;
        should_fire_bullet = 1;
    }
    if (0x2 & switches) {      // Switch 2 (mapped to RD1)
        //höger
        *y_speed = DIRECTION_DOWN;
        should_fire_bullet = 1;
    }
    if (0x1 & switches) {      // Switch 1 (mapped to RD0)
        //ned
        *x_speed = DIRECTION_RIGHT;
        should_fire_bullet = 1; 
    }

    
}

// Bullet spawn? Idk, den måste iaf ritas med display_image                                   //jag ser ingen anledning till varför det någonsin skulle göras här?
/*
void bullet_spawn() {
    display_image(bullet_x, bullet_y, 5, filled_square);
}
*/

void spawnakula() 
{
    if(12==getsw())
    {
        //vänster-upp
        direction_gun=7;
        avfyrametod(direction_gun,x_mainCharacter,y_mainCharacter);
    }
    if(6==getsw())
    {
        //upphöger
        direction_gun=4;
        avfyrametod(direction_gun,x_mainCharacter,y_mainCharacter);
    }
    if(3==getsw())
    {
        //ned-höger
        direction_gun=5;
        avfyrametod(direction_gun,x_mainCharacter,y_mainCharacter);
    }
    if(9==getsw())
    {
        //ned-vänster
        direction_gun=6;
        avfyrametod(direction_gun,x_mainCharacter,y_mainCharacter);
    }
    else
    {
        //gevär förblir som det var
    }
  }

int liteRandomVariabel=0;                                                            //en variabel som visar vilken kula som avfyrades sist
void avfyrametod(int riktning,int x,int y)                                        //metod för att skjuta, även den har inga problem men min kamrat vill inte använda den och skapar hellre egna metoder.       timer behövs för att betsämma hastighet av skott
{
    int i=0;
    while(kula[0][i]!=1000 && i<=100)
    {
      i++;
      //tar bara reda på hur mycket kulor som finns
    }
    if(i>100)
    {
      i=0;
      if(liteRandomVariabel==0)
      {
        for(int y=50;y<=100;y++)
        {
          kula[0][y]=1000;
          kula[1][y]=1000;
          kula[2][y]=1000;
          liteRandomVariabel=1;
        }
      }
      else
      {
        for(int y=0;y<=50;y++)
        {
          kula[0][y]=1000;
          kula[1][y]=1000;
          kula[2][y]=1000;
          liteRandomVariabel=0;
        }
      }
    }
    if(riktning<800)                //800 < 1000 därför ser den om kulan ej existerar
    {
    kula[0][i]=x;                   //spawnar kulan i spelaren
    kula[1][i]=y;
    kula[2][i]=riktning;
    }
  
}



void kulfard(int vilkenkula)                                                        //metod för att kulor ska färdas i sina rikningar, eventuellt även för annat som ska förflyttas, fungerar men av någon anledning ville min kamrat inte använda den och skapar i stället en klass vilket är bra men mer ineffektivt
{
  int x=kula[0][vilkenkula];
  int y=kula[1][vilkenkula];
  int riktning=kula[2][vilkenkula];
  if(3==riktning || 6==riktning || 7==riktning)          //1000 maskas till 1111 
      {
        y--;//y går vänster
      }
      if(0==riktning || 4==riktning || 7==riktning)
      {
        x++;//x går upp
      }
      if(1==riktning || 4==riktning || 5==riktning)
      {
        y++;//y går höger
      }
      if(2==riktning || 6==riktning || 6==riktning)
      {
        x--;//x går nedåt
      }
      else
      {
        //inget antar jag, finns för om flera knappar trycks samtidigt
      }
      kula[0][vilkenkula]=x;
      kula[1][vilkenkula]=y;
}


void fiendemanovrering(int fiendenummer)              //flyttar fiende mot spelaren
{
  if(fiendekoordinat[0][fiendenummer]>x_mainCharacter)
  {
    fiendekoordinat[0][fiendenummer]--;
  }
  if(fiendekoordinat[0][fiendenummer]<x_mainCharacter)
  {
    fiendekoordinat[0][fiendenummer]++;
  }
  if(fiendekoordinat[1][fiendenummer]>y_mainCharacter)
  {
    fiendekoordinat[1][fiendenummer]--;
  }
  if(fiendekoordinat[1][fiendenummer]<y_mainCharacter)
  {
    fiendekoordinat[1][fiendenummer]++;
  }
}

int skada()                                        //ska se om spelare och fiender tar skada,   3 AE runt x och y som preliminärt värde då både fiende och spelare är 5 nu och det borde betyda att pixeln som snuddar dem också är en träff
{
  int dogDen=1;
  for(int gggg=0;gggg<100;gggg++)                   //spawnar en fiende om ingen finns
    {
      if(fiendekoordinat[0][gggg]!=1000)
      {
        dogDen=0;
      }
    }
  for(int i=0;i<=100;i++)
  {
    if(x_mainCharacter-fiendekoordinat[0][i]*x_mainCharacter-fiendekoordinat[0][i]<=9 && y_mainCharacter-fiendekoordinat[1][i]*y_mainCharacter-fiendekoordinat[1][i]<=9)
      {
        lost=1;
      }
      for(int i2=0;kula[0][i2]!=1000;i2++)
      {
      if(kula[0][i2]-fiendekoordinat[0][i]*kula[0][i2]-fiendekoordinat[0][i]<=9 && kula[1][i2]-fiendekoordinat[1][i]*kula[1][i2]-fiendekoordinat[1][i]<=9)
        {
          fiendekoordinat[0][i]=1000;   //1000 är bara ett värde utanför skärmen då den här funktionen antar att det kan hanteras (det borde defenitivt hanteras annnars), om den kan ta sig från det till skärmen innan den ersätts bör den förtjäna att återupplivas
          fiendekoordinat[1][i]=1000;
          fiendekoordinat[2][i]=1000;     //visar att en ny fiende kan skapas här
          score++;                        //score går upp för varje dödad fiende
          dogDen=1;
        }
      }
  }
  return dogDen;
}

void clearkulor(int o)        //tar bort alla kulor från och med o
{
  for(int i2=o;kula[0][i2]<=100;i2++)
      {
        kula[0][i2]=1000;
      }
}

void spawnafiender(int antal)       //while(kula[0][i]!=1000 || i<=100)
{
  int i=0;
  while (i<=antal)
  {
    int b=0;
    while(fiendekoordinat[2][b]!=1000 && b<=100)
    {
      //vill ta sig till en NULL, kan effektiviseras då jag gjorde den här sent på en lördagkväll
      b++;
    }
    if(b!=101)                    //spawnar ej nya om det på något vis finns 100
    {
    fiendekoordinat[2][b]=1;       //ska vara 1                         
    fiendekoordinat[1][b]=3;                        //spawnar fiender random till höger
    fiendekoordinat[0][b]=tillSynesrandom(28)+100;
    variabelTillRandom+=10;
    i++;
  }
  }
}






//det behövs nog även en timer för när olika metoder får anropas dvs timer innan den tittar om knappar tryckta med knapptryck(); men även kulfärd();
