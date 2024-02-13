public int x_mainCharacter=64;     //fixa till logisk start sen
public int y_mainCharacter=32;     //fixa till logisk start sen
public int riktninggevär=0;     //0=upp, 1=höger, 2=ned, 3=vänster, 4=upp-höger, 5=ned-höger, 6=ned-vänster, 7=upp-vänster


/*
bredd: 128
höjd: 64*/

void knapptryck()
{
    if(8==7|getbtns())          //1000 maskas till 1111 
      {
        y_mainCharacter--;//y_mainCharacter går vänster med a
      }
      if(4==11|getbtns())
      {
        x_mainCharacter++;//x_mainCharacter går upp med w
      }
      if(2==13|getbtns())
      {
        y_mainCharacter++;//y_mainCharacter går höger med d
      }
      if(1==15|getbtns())
      {
        x_mainCharacter--;//x_mainCharacter går nedåt med s
      }
      else
      {
        //inget antar jag, finns för om flera knappar trycks samtidigt
      }
}

void spacktryck()
{
    //getsw()           kom ihåg att inte skjuta åt flera håll samtidigt
    if(8==getsw())
    {
        //vänster
        riktninggevär=3;
        //avfyrametod(riktninggevär)
    }
    if(4==getsw())
    {
        //upp
        riktninggevär=0;
        //avfyrametod(riktninggevär)
    }
    if(2==getsw())
    {
        //höger
        riktninggevär=1;
        //avfyrametod(riktninggevär)
    }
    if(1==getsw())
    {
        //ned
        riktninggevär=2;
        //avfyrametod(riktninggevär)
    }
    if(12==getsw())
    {
        //vänster-upp
        riktninggevär=7;
        //avfyrametod(riktninggevär)
    }
    if(6==getsw())
    {
        //upphöger
        riktninggevär=4;
        //avfyrametod(riktninggevär)
    }
    if(3==getsw())
    {
        //ned-höger
        riktninggevär=5;
        //avfyrametod(riktninggevär)
    }
    if(9==getsw())
    {
        //ned-vänster
        riktninggevär=6;
        //avfyrametod(riktninggevär)
    }
    else
    {
        //gevär förblir som det var
    }
}