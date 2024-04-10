//---------------------------------------------------------------------------

#ifndef race_gameH
#define race_gameH
//---------------------------------------------------------------------------
#include <math.hpp>
#include "race_car_engine.h"
// THIS IS RACE ALGORITHMS
// STAGING AND RACING
// NO INTERFACE JUST INTERNAL VARIABLES
#define DefStagingDistance 4
#define DefStagingLength 30
#define DefTrackLength 402.325
#define DefStartLightsWholeLength 5
#define COMPUTERCROSSLINE (1.6)
double Dif(double a, double b) {

  double d = a-b;
  if(d<0) d = -d;
  return d;

}
struct sCarLogic {
  // GAME FLAGS
  bool Staging;
  bool EndStagingMessage; // to reset speed to neutral
  bool Waiting; // depends on bracket time
  bool StartLights;
  bool FalseStart;
  bool Race;
  bool Finished;
  int speednum;
  int maxspeednum;
  int minspeednum;
  float TimeFromFirstStartLight; // from begining of StartLights
  float WaitingTimeLeft;
  bool OffLine;
  // RESULTS
  double RaceTime;
  double ReactionTime;
  double BracketTime;
  bool BreakOut;
  double WholeTime() {return RaceTime+ReactionTime;}
  double LeftToBracket() { return WholeTime()-BracketTime; }
  bool Foul() { return BreakOut || FalseStart; }
  // TEMP
  struct sAI {
    float prevvelocity;
    float maxthrottle;
    bool  dofalsestart;
    float RandomReactionTime; // this is for computer
    void msgGameStart() {
//      maxthrottle = 0.9;
      prevvelocity = 0;
      dofalsestart = false;
      RandomReactionTime = 0;
    }
  } AI;
  void SetSpeedNum(int value) {
    if(value<minspeednum)
      speednum = minspeednum;
    else
    if(value>maxspeednum)
      speednum = maxspeednum;
    else
      speednum = value;
  }
  void msgGameStart() {
    speednum = 0;
    Staging = true;
    Waiting = false;
    StartLights = false;
    Race = false;
    Finished = false;
    FalseStart = false;
    EndStagingMessage = false;
    TimeFromFirstStartLight = 0;
    RaceTime = 0;
    ReactionTime = -1; // UNDEF
    BreakOut = false;
    OffLine = false;
    maxspeednum = 5;
    minspeednum = -1;
    AI.msgGameStart();
  }
  void DoWaiting(float IterateLength) {
    if(Waiting) {
     WaitingTimeLeft -= IterateLength;
     if(WaitingTimeLeft<0) {
       Waiting = false;
       StartLights = true;
     }
    }
  }
  void DoStartLights(double IterateLength,float StartLightsWholeLength,float velocity, float position) {
    if(StartLights) {
       if(AI.RandomReactionTime==0) {
         // reaction to start driving depending on startlights
         AI.RandomReactionTime = 0.510+(float)random(330)/1000;
       }
       TimeFromFirstStartLight += IterateLength;

       if(position>DefStagingDistance) {
          StartLights = false;
          Race = true;
          if(TimeFromFirstStartLight<StartLightsWholeLength)
            FalseStart = true;
       }
    }
  }
  void DoRace(double IterateLength,float position, float TrackLength, float velocity) {
    if(Race) {
       TimeFromFirstStartLight += IterateLength;
       RaceTime += IterateLength;
       // REACTION TIME UNSET AND CAR STARTED DRIVING
       // SET REACTION TIME
       if(ReactionTime==-1 && position>DefStagingDistance) {
          double extrapath = position-DefStagingDistance;
          double extratime = extrapath/velocity;
          ReactionTime = TimeFromFirstStartLight-DefStartLightsWholeLength-extratime;
          ReactionTime = RoundTo(ReactionTime,-3);
          if(ReactionTime < 0.5) FalseStart = true;
          RaceTime += extratime;
       }
       // CHECK IF FINISHED
       if(position>TrackLength) {
         Race = false;
         Finished = true;
         double extrapath = position-TrackLength;
         double extratime = extrapath/velocity;
         RaceTime = RoundTo(RaceTime-extratime,-3);
         BracketTime = RoundTo(BracketTime,-3);
         if(BracketTime!=0)
//           BreakOut = (ReactionTime+RaceTime<BracketTime);
           BreakOut = (RaceTime<BracketTime);
       }
    }
  }
};

class cGame {
private:
public:
  // CONSTANTS
  float StagingDistance;
  float StagingLength;
  float TrackLength;
  float StartLightsWholeLength;
  // RESULTS
  bool YouWin;
  bool YouLose;
  bool Draw;
  bool Finished;
  cCarPhys CarPhys;
  cCarPhys OppPhys;
  sCarLogic CarLogic;
  sCarLogic OppLogic;

  struct sTesting {
    bool CarAutoDrive;
    bool NoStaging;
    void Reset() {NoStaging = false; CarAutoDrive = false;};
  } Testing;

  double IterateLength;
  cGame() {
    randomize();
    StagingDistance = DefStagingDistance;
    StagingLength = DefStagingLength;
    TrackLength = DefTrackLength;
    StartLightsWholeLength = DefStartLightsWholeLength;
    CarLogic.speednum = 0;
  }
  void incspeed() {
      CarLogic.SetSpeedNum(CarLogic.speednum+1);
  }
  void decspeed() {
      CarLogic.SetSpeedNum(CarLogic.speednum-1);
  }

  void msgGameStart() {
    CarPhys.ResetValues();
    OppPhys.ResetValues();
    CarPhys.position = -StagingLength;
    CarLogic.msgGameStart();
    OppLogic.msgGameStart();
    Testing.Reset();
    CarPhys.iteratelength = IterateLength;
    OppPhys.iteratelength = IterateLength;
    YouWin = false;
    YouLose = false;
    Draw = false;
    Finished = false;
  }

  void ComputerLogic(sCarLogic &State,cCarPhys &Car) {
    if(State.Finished) {
        Car.throttle = 0;
        Car.Braking = true;
    } else {
     Car.Braking = false;
    }

    if(State.StartLights || State.Race || (State.StartLights && State.AI.dofalsestart && State.TimeFromFirstStartLight>2)) {
       // START DRIVING ONLY IF REACTION TIME OR FALSE START
       if(State.FalseStart
         ||(State.TimeFromFirstStartLight>=
            DefStartLightsWholeLength-COMPUTERCROSSLINE+State.AI.RandomReactionTime)
         ||(State.RaceTime>0) ) {
         // not changing speed
         if(Car.velocity-State.AI.prevvelocity<0.001)
           Car.incspeed();
         State.AI.prevvelocity = Car.velocity;
         // throttle not max increase it
         if(Car.throttle<State.AI.maxthrottle) {
             Car.throttle +=0.1;
             if(Car.throttle>State.AI.maxthrottle)
               Car.throttle = State.AI.maxthrottle;
         }
         // if lost traction pull down throttle
         if(Car.losttraction) {
             Car.throttle -= 0.2;
             if(Car.throttle<0) Car.throttle = 0;
         }

       }
    }
  }
  // return result is just for return YouLose=true;
  bool DetectWin() {
        // IF YOU ARE INITIATING CHALLENGE
        if(OppLogic.OffLine && !OppLogic.Finished)
          this->Finished = true;

        // IF RESULTS ARE NOT DETECTED AND OPPONENT FINISHED
        if(!YouLose && !YouWin && !Draw && OppLogic.Finished) {

             // CHECK FOULS
             if(CarLogic.Foul() && OppLogic.Foul())
                return Draw=true;
             if(CarLogic.Foul() && !OppLogic.Foul())
                return YouLose=true;
             if(!CarLogic.Foul() && OppLogic.Foul())
                return YouWin=true;

             // NOT BRACKET RACE
             if(OppLogic.BracketTime==0) {
               if(Dif(OppLogic.WholeTime(),CarLogic.WholeTime())<0.0001)
                 return Draw=true;
               return CarLogic.WholeTime()<OppLogic.WholeTime() ? YouWin=true: YouLose=true;
             }

             // BRACKET RACE
             if(OppLogic.BracketTime!=0) {
               if(Dif(OppLogic.LeftToBracket(),CarLogic.LeftToBracket())<0.0001)
                 return Draw=true;
               return CarLogic.LeftToBracket()<OppLogic.LeftToBracket() ? YouWin=true : YouLose=true;
             }

         }
     return false;
  }

  void Iterate() {
    if(!OppLogic.OffLine)
      ComputerLogic(OppLogic,OppPhys);

    if(Testing.CarAutoDrive) {
      ComputerLogic(CarLogic,CarPhys);
    }

    if(Testing.NoStaging) {
      if(CarLogic.Staging)
        CarPhys.position = 0;
    }

    OppPhys.Iterate();

    if(!Testing.CarAutoDrive) {
      CarPhys.speednum = CarLogic.speednum;
    }
    CarPhys.Iterate();

    // STAGING
    if(CarLogic.Staging) {
      // STAGING BORDERS
      if(CarPhys.position<-StagingLength) {
        CarPhys.position = -StagingLength;
        CarPhys.Stop();
        CarPhys.RPM = 0;
      }
      if(CarPhys.position>StagingLength) {
        CarPhys.position = StagingLength;
        CarPhys.Stop();
        CarPhys.RPM = 0;
      }

      if(CarPhys.position<StagingDistance && CarPhys.position>-StagingDistance) {
         // END STAGING
         if(CarPhys.velocity == 0) {
            CarLogic.Staging = false;
            CarLogic.EndStagingMessage = true;
            if(!OppLogic.OffLine) {
              CarLogic.Waiting = true;
              OppLogic.Waiting = true;
              OppLogic.WaitingTimeLeft = 0;
              CarLogic.WaitingTimeLeft = 0;

              if(CarLogic.BracketTime>OppLogic.BracketTime) {
                 OppLogic.WaitingTimeLeft = CarLogic.BracketTime - OppLogic.BracketTime;
              }

              if(CarLogic.BracketTime<OppLogic.BracketTime) {
                 CarLogic.WaitingTimeLeft = OppLogic.BracketTime - CarLogic.BracketTime;
              }
            } else {
               CarLogic.Waiting = true;
               CarLogic.WaitingTimeLeft = 0;

            }

         }
      }

    }

    // WAITING FOR OTHER CAR
    CarLogic.DoWaiting(IterateLength);
    OppLogic.DoWaiting(IterateLength);

    // STARTLIGHTS
    CarLogic.DoStartLights(IterateLength,StartLightsWholeLength,CarPhys.velocity,CarPhys.position);
    OppLogic.DoStartLights(IterateLength,StartLightsWholeLength,OppPhys.velocity,OppPhys.position);

    // RACE
    CarLogic.DoRace(IterateLength,CarPhys.position,TrackLength,CarPhys.velocity);
    OppLogic.DoRace(IterateLength,OppPhys.position,TrackLength,OppPhys.velocity);
//
      if(CarLogic.Finished) {
        DetectWin();
        if(YouWin || YouLose || Draw) Finished = true;
      }

  }
};

extern cGame Game;
#endif
