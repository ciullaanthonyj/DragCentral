//---------------------------------------------------------------------------

#ifndef race_gameandflashH
#define race_gameandflashH
//---------------------------------------------------------------------------
#include "race_game.h"
#include "race_flash.h"
#include "caudio.h"
#define SOUNDIDLERPM 700
#define ENGINESOUNDRPM 5000

struct cGameAndFlash {
public:
  cGameAndFlash() {
    randomize();

  }
  void LoadAudio();

  void msgGameStart(int msecTimerInterval);
  void msgGameEnd();
  void OnCreate();
  void OnGameTimer();
  bool LightsPlayed[5];
  bool EndMessagePlayed;
  void CheckPlayLights(int num,AnsiString sound);
  AnsiString carid;
  // used for sound
  int OldSpeed;
  float OldDamage;
};

extern cGameAndFlash GameAndFlash;

#endif
