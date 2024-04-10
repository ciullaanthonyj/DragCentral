//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "test_flash_form.h"
#include "race_flash.h"
#include "cprofiler.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgrbutton"
#pragma link "ShockwaveFlashObjects_OCX"
#pragma resource "*.dfm"
Tmainform *mainform;
//---------------------------------------------------------------------------
__fastcall Tmainform::Tmainform(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::autoexecTimer(TObject *Sender)
{
  autoexec->Enabled = false;

  RaceFlash.flroad.mv = flroad;
  RaceFlash.flupview = flupview;
  RaceFlash.flcarpanel = flcarpanel;
  RaceFlash.flstaging = flstaging;
  RaceFlash.LoadFlashMovies();

  RaceFlash.StagingLength = 30;
  RaceFlash.TrackLength = 400;
  RaceFlash.StagingDistance = 2;
  RaceFlash.StartLightsWholeLength = 5;

  RaceFlash.msgGameStart();

  race->Enabled= true;
  RaceFlash.HideCar("left");
  RaceFlash.HideCar("right");
}
//---------------------------------------------------------------------------
static float position=-6.5;
void __fastcall Tmainform::raceTimer(TObject *Sender)
{
  position += 2;
  PSTART("terrain");
  RaceFlash.ChangeTerrain(position);
  PEND("terrain");
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::ScrollBar1Change(TObject *Sender)
{
  this->Caption = IntToStr(ScrollBar1->Position);
  RaceFlash.TreesDistance = ScrollBar1->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::FormClose(TObject *Sender, TCloseAction &Action)
{
  PSAVE;        
}
//---------------------------------------------------------------------------

