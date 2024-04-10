//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "test_game_form.h"
#include "mymath.h"
#include "myflash.h"
#include "race_flash.h"
#include "race_game.h"
#include "race_gameandflash.h"
#include "caudio.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma link "ShockwaveFlashObjects_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

float PartAngle(float StartAngle,float EndAngle,float Part) {
   return StartAngle + (EndAngle-StartAngle) * Part;
}


void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  GameAndFlash.OnGameTimer();
  int i = 0;
  Memo1->Lines->Strings[i++] = "Opponent.Speed:"+FloatToStrF(Game.Opponent.speednum,ffFixed,5,3);
  Memo1->Lines->Strings[i++] = "Opponent.Throttle:"+FloatToStrF(Game.Opponent.throttle,ffFixed,5,3);
  Memo1->Lines->Strings[i++] = "Opponent.Velocity:"+FloatToStrF(Game.Opponent.velocity,ffFixed,5,3);
  Memo1->Lines->Strings[i++] = "Opponent.Position:"+FloatToStrF(Game.Opponent.position,ffFixed,5,3);
  Memo1->Lines->Strings[i++] = "Opponent.StartTime:"+FloatToStrF(Game.OpponentState.StartLightsCurLength,ffFixed,5,3);
  Memo1->Lines->Strings[i++] = "Opponent.CompStartLights:"+BoolToStr(Game.OpponentState.StartLights);
  Memo1->Lines->Strings[i++] = "Opponent.LostTraction:"+BoolToStr(Game.Opponent.losttraction);
  Memo1->Lines->Strings[i++] = "Opponent.A:"+FloatToStrF(Game.Opponent.a,ffFixed,5,3);
  Memo1->Lines->Strings[i++] = "Opponent.CarVelocity:"+FloatToStrF(Game.Opponent.carvelocity,ffFixed,5,3);
  if(Game.Finished)
    this->Caption = "Finished";

  return;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  pb->Canvas->Brush->Color = clWhite;
  pb->Canvas->FillRect(Rect(0,0,pb->Width,pb->Height));
  for(int i=0;i<20000;i++) {
    float torque = Game.Car.torqueRPM(i);
    int x,y;
    x = i*pb->Width/20000;
    y = pb->Height - torque * pb->Height/1000-1;
    pb->Canvas->Pixels[x][y] = clRed;
  }
}
//---------------------------------------------------------------------------




void __fastcall TForm1::SpeedUp1Click(TObject *Sender)
{
  Game.incspeed();
  RaceFlash.SetSpeedNum(Game.CarState.speednum);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DownClick(TObject *Sender)
{
  Game.decspeed();
  RaceFlash.SetSpeedNum(Game.CarState.speednum);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ApplicationEvents1Message(tagMSG &Msg,
      bool &Handled)
{
  if(Msg.message == WM_RBUTTONDOWN) Handled = true;        
}
//---------------------------------------------------------------------------



void __fastcall TForm1::FormCreate(TObject *Sender)
{
  //remove blinking
  for(int i=0;i<ComponentCount;i++)
    if(dynamic_cast<TImage*>(Components[i])!=NULL) {
      TImage* im = (TImage*)(Components[i]);
      if(im->Picture->Graphic!=NULL)
        im->Parent->DoubleBuffered = true;
    }

  Audio.SetVolume(0.1);
  AnsiString prefix = ExtractFilePath(Application->ExeName);
  Audio.LoadSample("engine",prefix+"aud\\rpm.wav",2);
  Audio.SetPitchVolume("engine",0.1,3,6,1);
  Audio.SetSampleLoop("engine");
  Audio.LoadSample("slippery",prefix+"aud\\slippery.wav",1.2);
  Audio.SetSampleLoop("slippery");
  Audio.LoadSample("speed",prefix+"aud\\speed.wav",1.3);
  Audio.LoadSample("youlose",prefix+"aud\\finish-youlose.mp3",1.5);
  Audio.LoadSample("youwin",prefix+"aud\\finish-youwin.mp3",4);
  Audio.LoadSample("yellow",prefix+"aud\\lights-yellow.wav",4);
  Audio.LoadSample("green",prefix+"aud\\lights-green.wav",4);
  Audio.LoadSample("music",prefix+"aud\\music.mp3",7);
  Audio.SetSampleLoop("music");

  RaceFlash.flroad = flroad;
  RaceFlash.flupview = flupview;
  RaceFlash.flcarpanel = flcarpanel;
  RaceFlash.flstaging = flstaging;
  RaceFlash.LoadFlashMovies();
  RaceFlash.StagingLength = Game.StagingLength;
  RaceFlash.TrackLength = Game.TrackLength;
  RaceFlash.StagingDistance = Game.StagingDistance;
  RaceFlash.StartLightsWholeLength = Game.StartLightsWholeLength;

  Game.CarState.BracketTime = 21.0;
  Game.OpponentState.BracketTime = 18.0;
  StartClick(0);

}

void __fastcall TForm1::StartClick(TObject *Sender)
{
  GameAndFlash.msgGameStart(Timer1->Interval);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
  Timer1->Enabled = false;
  Game.msgGameStart();
  RaceFlash.msgGameStart();
  Game.Car.position = 0;
  RaceFlash.PrestageLightsVisible(false,0);
  RaceFlash.StartLights(0,false,0);
  RaceFlash.StartLights(0,false,1);
  Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
  Game.Car.position = Game.TrackLength - 10;
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Bracketrace1Click(TObject *Sender)
{
  Game.OpponentState.BracketTime = 15.0;
  Game.OpponentState.AI.maxthrottle = 0.8;
  Game.CarState.BracketTime = StrToFloat(InputBox("Enter Bracket Time","Bracket Time:",
    FloatToStrF(Game.CarState.BracketTime,ffFixed,5,3)));
  StartClick(0);
  Game.Testing.CarAutoDrive = true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Youwinsimulation1Click(TObject *Sender)
{
  Game.OpponentState.BracketTime = 15.0;
  Game.CarState.BracketTime = 16.0;
  StartClick(0);
  Game.OpponentState.AI.maxthrottle = 0.7;
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BreakOutCLick(TObject *Sender)
{
  Game.OpponentState.BracketTime = 15.0;
  Game.CarState.BracketTime = 18.0;
  GameAndFlash.msgGameStart(Timer1->Interval);
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Youlosesimulation1Click(TObject *Sender)
{
  Game.OpponentState.BracketTime = 15.0;
  Game.CarState.BracketTime = 13.0;
  StartClick(0);
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;
//  Timer1->Interval = 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WithracerYouwinsimulation1Click(TObject *Sender)
{

  Game.CarState.BracketTime = 16.0;
  GameAndFlash.msgGameStart(Timer1->Interval);
  Game.OpponentState.OffLine = true;
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;
  Game.OpponentState.RaceTime = 20.5;
  Game.OpponentState.BracketTime = 16.0;
  Game.OpponentState.Finished = true;
}
//---------------------------------------------------------------------------

void TForm1::DetectWin() {
  AnsiString in,out;
  in.printf("Opponent.OffLine: %d Finished: %d FalseStart: %d BreakOut: %d RaceTime: %.3f BracketTime: %.3f\r\n"
            "You.Finished: %d FalseStart: %d BreakOut: %d RaceTime: %.3f BracketTime: %.3f",
             Game.OpponentState.OffLine, Game.OpponentState.Finished, Game.OpponentState.FalseStart,
             Game.OpponentState.BreakOut, Game.OpponentState.RaceTime, Game.OpponentState.BracketTime,
             Game.CarState.Finished, Game.CarState.FalseStart, Game.CarState.BreakOut, Game.CarState.BracketTime);
  Game.DetectWin();
  out.printf("Game.YouWin: %d Game.YouLose: %d Game.Draw: %d",
              Game.YouWin, Game.YouLose, Game.Draw);

  Memo2->Lines->Add(in);
  Memo2->Lines->Add(out);
}

void __fastcall TForm1::Button4Click(TObject *Sender)
{

  Game.msgGameStart();
  Game.OpponentState.Finished = true;
  Game.OpponentState.BreakOut = true;

  DetectWin();

  Game.msgGameStart();
  Game.OpponentState.Finished = true;
  Game.OpponentState.BreakOut = true;

  Game.CarState.BreakOut = true;

  DetectWin();
}
//---------------------------------------------------------------------------

