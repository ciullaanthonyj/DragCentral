//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "test_computer_racer_form.h"
#include "race_game.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::goClick(TObject *Sender)
{
  Game.IterateLength = 0.030;
  Game.msgGameStart();

  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;
  Game.OppLogic.OffLine = true;
  DecimalSeparator = '.';
  Game.CarLogic.AI.maxthrottle = StrToFloat(mt->Text);
  Game.CarLogic.AI.RandomReactionTime = StrToFloat(rt->Text);
  Timer1Timer(0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{

  while(!Game.CarLogic.Finished) {
    Game.Iterate();
    carpos->Text = Game.CarPhys.position;
    static AnsiString state = "";
    static AnsiString newstate = "";
    if(Game.CarLogic.Waiting)
      newstate = "Waiting";
    if(Game.CarLogic.Staging)
      newstate = "Staging";
    if(Game.CarLogic.Race)
      newstate = "Race";
    if(newstate!=state) {
      Memo1->Lines->Add(newstate);
      state = newstate;
    }


    if(Game.CarLogic.Finished) {
      Memo1->Lines->Add(
      FloatToStrF(Game.CarLogic.AI.maxthrottle,ffFixed,10,2)
      +" Reaction: " + FloatToStr(Game.CarLogic.ReactionTime)+
      +" Race time:" + FloatToStr(Game.CarLogic.RaceTime));

    }

    Application->ProcessMessages();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::mtKeyPress(TObject *Sender, char &Key)
{
  if(Key==13)
    goClick(0);
}
//---------------------------------------------------------------------------

