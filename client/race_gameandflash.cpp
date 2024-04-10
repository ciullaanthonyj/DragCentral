//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop

#include "race_gameandflash.h"
#include "myvcl.h"
#include "cprofiler.h"


cGameAndFlash GameAndFlash;
void cGameAndFlash::msgGameStart(int msecTimerInterval) {
  RaceFlash.StagingLength = Game.StagingLength;
  RaceFlash.TrackLength = Game.TrackLength;
  RaceFlash.StagingDistance = Game.StagingDistance;
  RaceFlash.StartLightsWholeLength = Game.StartLightsWholeLength;


  Game.IterateLength = (double)msecTimerInterval / 1000;
  Game.msgGameStart();
  RaceFlash.msgGameStart();
  Audio.PlaySample("engine");
  Audio.SetSamplePitch("engine",(float)SOUNDIDLERPM/ENGINESOUNDRPM);
  LightsPlayed[0] = false;
  LightsPlayed[1] = false;
  LightsPlayed[2] = false;
  LightsPlayed[3] = false;
  EndMessagePlayed = false;
  OldSpeed = Game.CarPhys.speednum;
  OldDamage = Game.CarPhys.enginedamage;
  RaceFlash.StartLights(0,false,"left");
  RaceFlash.HideCar("left");
  RaceFlash.ShowInfo("left",false);
  RaceFlash.SelectCar("left","1");
  RaceFlash.SelectCar("right",carid);
  RaceFlash.SetMinSpeed("-1");
  RaceFlash.SetMaxSpeed(IntToStr(Game.CarPhys.maxspeednum));
  Game.CarLogic.minspeednum=-1;
}

void cGameAndFlash::OnCreate() {

  RaceFlash.StagingLength = Game.StagingLength;
  RaceFlash.TrackLength = Game.TrackLength;
  RaceFlash.StagingDistance = Game.StagingDistance;
  RaceFlash.StartLightsWholeLength = Game.StartLightsWholeLength;
  carid = "1";

}
void cGameAndFlash::msgGameEnd() {
  Audio.FadeOutSample("engine",2.5);
  Audio.StopSample("slippery");
}

void cGameAndFlash::LoadAudio() {
    AnsiString prefix = ExtractFilePath(Application->ExeName);
    Audio.LoadSample("engine",prefix+"aud\\rpm.wav",1);
    Audio.SetPitchVolume("engine",0.1,3,6.*0.5,1.*0.5);
    Audio.SetSampleLoop("engine");
    Audio.LoadSample("slippery",prefix+"aud\\slippery.wav",1.2);
    Audio.SetSampleLoop("slippery");
    Audio.LoadSample("speed",prefix+"aud\\speed.wav",1.3);
    Audio.LoadSample("youlose",prefix+"aud\\finish-youlose.mp3",1.5);
    Audio.LoadSample("youwin",prefix+"aud\\finish-youwin.mp3",4);
    Audio.LoadSample("yellow",prefix+"aud\\lights-yellow.wav",4);
    Audio.LoadSample("green",prefix+"aud\\lights-green.wav",4);
    Audio.LoadSample("nitro",prefix+"aud\\nitro.wav",1.4);
    Audio.LoadSample("damage",prefix+"aud\\damage.wav",1);
}

void cGameAndFlash::OnGameTimer() {
  PSTARTP("begin","gameandflash");

  float Distance = Game.OppPhys.position - Game.CarPhys.position + 10;

  if(!Game.Finished)
  if(KeyPressed(39)) {
    if(Game.CarPhys.donitro())
      Audio.PlaySample("nitro");
  }

  if(Game.OppLogic.OffLine) {
    RaceFlash.StartLights(0,false,"left");
    RaceFlash.HideCar("left");
    RaceFlash.ShowInfo("left",false);
  }
  else {
    RaceFlash.ShowCar(Distance);
    RaceFlash.ShowInfo("left",true);
  }
  
  if(Game.CarPhys.speednum!=OldSpeed) {
    Audio.PlaySample("speed");
    OldSpeed = Game.CarPhys.speednum;
  }

  if(Game.CarPhys.enginedamage!=OldDamage) {
    Audio.PlaySample("damage");
    OldDamage = Game.CarPhys.enginedamage;
  }

  if(Game.CarPhys.losttraction) {
    if(!Audio.IsSamplePlaying("slippery"))
      Audio.PlaySample("slippery");
  } else {
      Audio.StopSample("slippery");
  }
  RaceFlash.showLostTraction(Game.CarPhys.losttraction);

  // STAGING
  if(Game.CarLogic.Staging) {
	if(RaceFlash.SkipStaging()) {
	  RaceFlash.HideSkipStagingButton();                 
	  Game.CarPhys.position = 0;
	  Game.CarPhys.velocity = 0;
	}
	RaceFlash.flupview->Visible = false;
	RaceFlash.flstaging->Visible = true;
	RaceFlash.StartLights(Game.CarLogic.TimeFromFirstStartLight,false,"right");
	RaceFlash.PrestageLightsVisible(true,Game.CarPhys.position);

	if(!Game.OppLogic.OffLine) {
	  RaceFlash.ShowBracketTime(Game.OppLogic.BracketTime,0);
	  RaceFlash.StartLights(Game.CarLogic.TimeFromFirstStartLight,false,"left");
	}
	if(Game.OppLogic.OffLine && Game.OppLogic.Finished) {
	  RaceFlash.ShowBracketTime(Game.OppLogic.BracketTime,0);
	}
	RaceFlash.ShowBracketTime(Game.CarLogic.BracketTime,1);
  }

  if(Game.CarLogic.EndStagingMessage) {
	// to disable macro recorders

	RaceFlash.HideSkipStagingButton();
    flsetvar(RaceFlash.flroad.mv,"_root.braking","0",false);	
	RaceFlash.SetSpeedNum(0);
	RaceFlash.SetMinSpeed("0");
	Game.CarLogic.minspeednum = 0;
	Game.CarLogic.EndStagingMessage = false;
    RaceFlash.flupview->Visible = true;
    RaceFlash.flstaging->Visible = false;
  }

  // WAITING FOR OTHER BRACKET
  if(Game.CarLogic.Waiting) {
     RaceFlash.PrestageLightsVisible(false,0);
     RaceFlash.StartLights(Game.CarLogic.TimeFromFirstStartLight,
       Game.CarLogic.FalseStart,"right");
  }

  if(Game.CarLogic.StartLights) {
     RaceFlash.StartLights(Game.CarLogic.TimeFromFirstStartLight,
       Game.CarLogic.FalseStart,"right");
  }

  if(!Game.OppLogic.OffLine)
  if(Game.OppLogic.StartLights) {
     RaceFlash.StartLights(Game.OppLogic.TimeFromFirstStartLight,
       Game.OppLogic.FalseStart,"left");
  }

  if(Game.CarLogic.FalseStart) {
     RaceFlash.ReactionTime(0,true,"right");
     RaceFlash.LightFalseStart();
  }

  // RACE

  if(!Game.OppLogic.OffLine)
  if(Game.OppLogic.Race) {
     RaceFlash.StartLights(Game.OppLogic.TimeFromFirstStartLight,false, "left");
     RaceFlash.ReactionTime(Game.OppLogic.ReactionTime,Game.OppLogic.FalseStart,"left");
  }

  if(Game.CarLogic.Race) {
     RaceFlash.StartLights(Game.CarLogic.TimeFromFirstStartLight,
       Game.CarLogic.FalseStart,"right");
     RaceFlash.ReactionTime(Game.CarLogic.ReactionTime,Game.CarLogic.FalseStart,"right");
  }

  if(Game.OppLogic.Finished && !Game.OppLogic.OffLine) {
    RaceFlash.ReactionTime(Game.OppLogic.ReactionTime,Game.OppLogic.FalseStart,"left");
    RaceFlash.FinishedInfo(Game.OppLogic.RaceTime,Game.OppLogic.BreakOut,"left");
  }

  if(Game.CarLogic.Finished) {
    RaceFlash.FinishedInfo(Game.CarLogic.RaceTime,Game.CarLogic.BreakOut,"right");
    // IF ANSWERING CHALLENGE
    if(Game.OppLogic.OffLine && Game.OppLogic.Finished) {
       RaceFlash.FinishedInfo(Game.OppLogic.RaceTime,Game.OppLogic.BreakOut,"left");
       RaceFlash.ReactionTime(Game.OppLogic.ReactionTime,Game.OppLogic.FalseStart,"left");
       RaceFlash.ShowInfo("left",true);
    }
  }

  if(Game.YouWin) {
    RaceFlash.ShowEndMessage("YOU WIN!!");
    if(!EndMessagePlayed) {
      EndMessagePlayed = true;
      Audio.PlaySample("youwin");
    }
  };

  if(Game.YouLose) {
    RaceFlash.ShowEndMessage("YOU LOSE!!");
    if(!EndMessagePlayed) {
      EndMessagePlayed = true;
      Audio.PlaySample("youlose");
    }
  };

  if(Game.Draw) {
    RaceFlash.ShowEndMessage("Draw");
    EndMessagePlayed = true;
  }


  if(!Game.Testing.CarAutoDrive) {
    Game.CarPhys.Braking = RaceFlash.GetBraking();
    Game.CarLogic.SetSpeedNum(RaceFlash.GetSpeedNum());
    Game.CarPhys.throttle = (float)(RaceFlash.GetGas());
  }
  Game.Iterate();
  CheckPlayLights(0,"yellow");
  CheckPlayLights(1,"yellow");
  CheckPlayLights(2,"yellow");
  CheckPlayLights(3,"green");

  // ROAD INDICATOR
  RaceFlash.SetUpview(Game.CarPhys.position,"right");
  if(!Game.OppLogic.OffLine)
    RaceFlash.SetUpview(Game.OppPhys.position,"left");
  // INDICATORS
  float Angle;
  // NITROUS INDICATOR
  flvisible(RaceFlash.flcarpanel,"nitrogauge",Game.CarPhys.Nitro.Installed);
  flvisible(RaceFlash.flcarpanel,"nitro",Game.CarPhys.Nitro.Installed);
  if(Game.CarPhys.Nitro.Installed) {
    Angle = prop(0,100,Game.CarPhys.Nitro.Left,-140,140);
    flsetvar(RaceFlash.flcarpanel,"nitro._rotation",FloatToStrF(Angle,ffFixed,10,2) );
  }
  // SPEED INDICATOR
  Angle = prop(0,160,mod(Game.CarPhys.MPH),-142,+142);
  flsetvar(RaceFlash.flcarpanel,"MPH._rotation",FloatToStrF(Angle,ffFixed,10,2) );

  /// RPM INDICATOR
  Angle = prop(0,8000,mod(Game.CarPhys.RPM),-142,+142);
  if(Angle>142) Angle = 142;
  flsetvar(RaceFlash.flcarpanel,"RPM._rotation",FloatToStrF(Angle,ffFixed,10,2));

  PEND("begin");
  PSTARTP("end","gameandflash");


  float SoundRPM = Game.CarPhys.RPM;
  if(SoundRPM<SOUNDIDLERPM) SoundRPM = SOUNDIDLERPM;
  Audio.SetSamplePitch("engine",(float)SoundRPM/ENGINESOUNDRPM);
  // RACE ANIMATION
  PSTARTP("terrain","gameandflash");
  RaceFlash.ChangeTerrain(Game.CarPhys.position);
  PEND("terrain");
  // STAGING INDICATOR
  if(Game.CarLogic.Staging)
    RaceFlash.StagingChange(Game.CarPhys.position);
  PEND("end");
};
//---------------------------------------------------------------------------
void cGameAndFlash::CheckPlayLights(int num,AnsiString sound) {
  if(RaceFlash.LightsTurned[num] && !this->LightsPlayed[num]) {
    Audio.PlaySample(sound);
    LightsPlayed[num] = true;
  }
}


#pragma package(smart_init)
