public int x_mainCharacter;     //fixa till logisk start sen
public int y_mainCharacter;     //fixa till logisk start sen
public bool avfyrar=0;          //
public int riktninggevär=0;     //0=upp, 1=höger, 2=ned, 3=vänster, 4=upp-höger, 5=ned-höger, 6=ned-vänster, 7=upp-vänster

void knapptryck()
{
    if(8==getbtns();
      {
        y_mainCharacter--;//y_mainCharacter går vänster med a
      }
      if(4==getbtns();
      {
        x_mainCharacter++;//x_mainCharacter går upp med w
      }
      if(2==getbtns();
      {
        y_mainCharacter++;//y_mainCharacter går höger med d
      }
      if(1==getbtns();
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

    }
    if(4==getsw())
    {
        
    }
    if(2==getsw())
    {
        
    }
    if(1==getsw())
    {
        
    }
}