int x_mainCharacter=64;     //fixa till logisk start sen
int y_mainCharacter=14;     //fixa till logisk start sen
/*
int riktninggevär=0;        //0=upp, 1=höger, 2=ned, 3=vänster, 4=upp-höger, 5=ned-höger, 6=ned-vänster, 7=upp-vänster
int[] kula[3][1];   //kula[0]==x, kula[1]==y, kula[2]==riktning
int[] fiendekoordinat[2][1];    //fiendes koordinater
*/
// C har inte public


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
*/

// bit1 == bit2 is not valid in C. Use bit1 & bit2 instead.

void knapptryck()
{
    int buttons = getbtns();
    if (0x8 & buttons) {      // Button 4 (mapped to RD11)
        x_mainCharacter--;     // y_mainCharacter moves left
    }
    if (0x4 & buttons) {      // Button 3 (mapped to RD10)
        y_mainCharacter++;    // x_mainCharacter moves up
    }
    if (0x2 & buttons) {      // Button 2 (mapped to RD9)
        y_mainCharacter--;    // x_mainCharacter moves down
    }
    if (0x1 & buttons) {      // Button 1 (mapped to RD8)
        x_mainCharacter--;    // x_mainCharacter moves right
    }
}

/* Bortkommenterat för testning av player movement

void spacktryck()
{
    //getsw()           kom ihåg att inte skjuta åt flera håll samtidigt
    if(8==getsw())
    {
        //vänster
        riktninggevär=3;
        avfyrametod(riktninggevär,x_mainCharacter,y_mainCharacter)
    }
    if(4==getsw())
    {
        //upp
        riktninggevär=0;
        avfyrametod(riktninggevär,x_mainCharacter,y_mainCharacter)
    }
    if(2==getsw())
    {
        //höger
        riktninggevär=1;
        avfyrametod(riktninggevär,x_mainCharacter,y_mainCharacter)
    }
    if(1==getsw())
    {
        //ned
        riktninggevär=2;
        avfyrametod(riktninggevär,x_mainCharacter,y_mainCharacter)
    }
    if(12==getsw())
    {
        //vänster-upp
        riktninggevär=7;
        avfyrametod(riktninggevär,x_mainCharacter,y_mainCharacter)
    }
    if(6==getsw())
    {
        //upphöger
        riktninggevär=4;
        avfyrametod(riktninggevär,x_mainCharacter,y_mainCharacter)
    }
    if(3==getsw())
    {
        //ned-höger
        riktninggevär=5;
        avfyrametod(riktninggevär,x_mainCharacter,y_mainCharacter)
    }
    if(9==getsw())
    {
        //ned-vänster
        riktninggevär=6;
        avfyrametod(riktninggevär,x_mainCharacter,y_mainCharacter)
    }
    else
    {
        //gevär förblir som det var
    }
}


void avfyrametod(int riktning,int x,int y)                                        //metod för att skjuta,       timer för hastighet av skott
{
    int i=0;
    while(kula[0][i]!=NULL)
    {
      i++;
      //tar bara reda på hur mycket kulor som finns
    }
    kula[0][i]=x;                   //spawnar kulan i spelaren
    kula[1][i]=y;
    kula[2][i]=riktning;
  
}

void kulfärd(int x, int y, int riktning)                                                        //metod för att kulor ska färdas i sina rikningar, eventuellt även för annat som ska förflyttas
{
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
}


void fiendemanövrering(int fiendenummer)              //flyttar fiende mot spelaren
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

void skada()                                        //ska se om spelare och fiender tar skada
{

}



*/


//det behövs nog även en timer för när olika metoder får anropas dvs timer innan den tittar om knappar tryckta med knapptryck(); men även kulfärd();