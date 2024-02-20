public int x_mainCharacter=64;     //fixa till logisk start sen
public int y_mainCharacter=32;     //fixa till logisk start sen
public int direction_gun=0;     //0=upp, 1=höger, 2=ned, 3=vänster, 4=upp-höger, 5=ned-höger, 6=ned-vänster, 7=upp-vänster


/*
bredd: 128
höjd: 64*/

int should_spawn_bullet() {
    return 1; // always spawn a bullet
}
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

void spaktryck()
{
    //getsw()           kom ihåg att inte skjuta åt flera håll samtidigt
    if(8==getsw())
    {
        //vänster
        direction_gun=3;
        should_spawn_bullet();
        //avfyrametod(direction_gun)
    }
    if(4==getsw())
    {
        //upp
        direction_gun=0;
        should_spawn_bullet();
        //avfyrametod(direction_gun)
    }
    if(2==getsw())
    {
        //höger
        direction_gun=1;
        should_spawn_bullet();
        //avfyrametod(direction_gun)
    }
    if(1==getsw())
    {
        //ned
        direction_gun=2;
        should_spawn_bullet();
        //avfyrametod(direction_gun)
    }
    if(12==getsw())
    {
        //vänster-upp
        direction_gun=7;
        should_spawn_bullet();
        //avfyrametod(direction_gun)
    }
    if(6==getsw())
    {
        //upphöger
        direction_gun=4;
        should_spawn_bullet();
        //avfyrametod(direction_gun)
    }
    if(3==getsw())
    {
        //ned-höger
        direction_gun=5;
        should_spawn_bullet();
        //avfyrametod(direction_gun)
    }
    if(9==getsw())
    {
        //ned-vänster
        direction_gun=6;
        should_spawn_bullet();
        //avfyrametod(direction_gun)
    }
    else
    {
        //gevär förblir som det var
    }
}