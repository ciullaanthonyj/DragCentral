//---------------------------------------------------------------------------

#ifndef race_flashH
#define race_flashH
#include "myflash.h"
#include "mymath.h"
#define VisibleLength 60

class cRaceFlash {
public:
  double StagingLength;
  double TrackLength;
  double StagingDistance;
  double StartLightsWholeLength;
  double TreesDistance;
  FlashFlic flroad;
  TShockwaveFlash* flupview;
  TShockwaveFlash* flcarpanel;
  TShockwaveFlash* flstaging;
  cRaceFlash() {
    TreesDistance = 80;
  }
  void HideCar(AnsiString prefix);

  void ShowInfo(AnsiString prefix, bool visible);
  void SetMinSpeed(AnsiString speed);
  void SetMaxSpeed(AnsiString value);

  void SetUpview(double pos,AnsiString prefix);
  void ShowBracketTime(double time,int track);
  void ShowBreakOut(AnsiString prefix,bool visible);
  bool GetBraking();

  void FinishedInfo(double time,bool BreakOut,AnsiString prefix);
  double GetGas();
  void SetSpeedNum(int newspeednum);
  int GetSpeedNum();
  void showLostTraction(bool visible);
  void msgGameStart();
  void ShowEndMessage(AnsiString mess);
  void PrestageLightsVisible(bool visible,double Position);
  // track 0 or 1
  void StartLights(double time,bool FalseStart, AnsiString prefix);
  void ReactionTime(double time, bool FalseStart, AnsiString track);
  void LightFalseStart();
  void LoadFlashMovies();
  void ShowCar(double Distance);
  bool LightsTurned[5];
  void SelectCar(AnsiString prefix,AnsiString carid);
  //directset only one time when start to disable first shitty frame
  void ChangeTerrain(double Position,bool directset=false);
  void InitTerrain();
  void InitStaging();
  void StagingChange(double Position);
  bool SkipStaging();
  void HideSkipStagingButton();

  AnsiString racertrack;

};

extern cRaceFlash RaceFlash;



//---------------------------------------------------------------------------
#endif
