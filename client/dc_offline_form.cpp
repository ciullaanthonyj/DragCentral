//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "dc_offline_form.h"
#include "race_flash.h"
#include "race_game.h"
#include "race_gameandflash.h"
#include "caudio.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ShockwaveFlashObjects_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
  RaceFlash.flroad.mv = flroad;
  RaceFlash.flupview = flupview;
  RaceFlash.flcarpanel = flcarpanel;
  RaceFlash.flstaging = flstaging;
  RaceFlash.LoadFlashMovies();
  GameAndFlash.LoadAudio();

  DecimalSeparator = '.';

}
//---------------------------------------------------------------------------

void __fastcall TForm1::autoexecTimer(TObject *Sender)
{
  autoexec->Enabled = false;



  Audio.SetVolume(0.1);


  RestartClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::gametimerTimer(TObject *Sender)
{
  GameAndFlash.OnGameTimer();
  lbenginedamage->Caption = "Engine Damage: " + FloatToStrF(Game.CarPhys.enginedamage,ffFixed,10,3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnSkipStagingClick(TObject *Sender)
{
  Game.CarPhys.position = 0;
  Game.CarPhys.velocity = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RestartClick(TObject *Sender)
{
  gametimer->Enabled = false;

  Game.msgGameStart();
  RaceFlash.msgGameStart();

  Game.CarPhys.mass = 1399; //kg
  Game.CarPhys.horses = 130; //horses (some empirical not reall values)
  Game.CarPhys.reduceslip = 0;
  Game.CarPhys.Nitro.Installed = false;
  Game.CarPhys.Nitro.Left = 0;
  Game.CarPhys.Nitro.Horses = 0;
  Game.CarPhys.Charger.turbocharger = 0;
  Game.CarPhys.Charger.supercharger = 0;
  Game.CarPhys.MagicGear.Installed = false;
  Game.CarPhys.MagicGear.g1 = false;
  Game.CarPhys.MagicGear.g2 = false;
  Game.CarPhys.MagicGear.g3 = false;
  Game.CarPhys.MagicGear.g4 = false;
  Game.CarPhys.MagicGear.g5 = false;
  Game.CarPhys.MagicGear.g6 = false;
  Game.CarPhys.maxspeednum = 6;
  GameAndFlash.carid = 1;

  // no opponent

  GameAndFlash.msgGameStart(gametimer->Interval);

  Game.OppLogic.OffLine = true;
  gametimer->Enabled = true;



}
//---------------------------------------------------------------------------

