//---------------------------------------------------------------------------

#ifndef race_car_engineH
#define race_car_engineH
//---------------------------------------------------------------------------
#include <math.h>
#include "mymath.h"
#include  <assert.h>

// THIS IS FILE FOR CAR SPEED
// AND MODIFICATIONS

// from this value of speed changing engine damage starts
#define DAMAGEVELOCITY 10
// for fast speed changing
#define RPMDAMAGEDIFF 3000

class cCarPhys {
private:
  int prevspeednum;
  float prevRPM;
  void addenginedamage(float add) {
    enginedamage += add;
    if(enginedamage>100) enginedamage = 100;
  }
public:
  float enginedamage; //from 0 to 100% reduces horse power
  float mass; //kg
  float wheelradius; //m
  float horses;
  float differential;
  float transmission;
  float frontalarea;
  float airdensity;
  float cdrag;
  float crr; //coefficient rubber resistance
  float velocity; //m/s // wheel velocity
  float carvelocity;
  float KMH;
  float MPH;
  float iteratelength;
  float RPM;
  float throttle;
  float position; //m
  float a;
  float reduceslip; // in percents
  int speednum;
  int maxspeednum;
  bool losttraction;
  cCurve TorqueCurve;
  cCurve TurboCharger;
  cCurve SuperCharger;
  bool Braking;
  struct sNitro {
    bool Installed;  // installed or not
    float Left; //0-100
    float Horses;
    float TimeLeft; // in seconds - when pressed nitro it sets to NITROTIME and NitrousLeft - 5
  } Nitro;
  struct sCharger {
    bool turbocharger;
    bool supercharger;
  } Charger;
  struct sMagicGear {
    bool Installed;
    float g1;
    float g2;
    float g3;
    float g4;
    float g5;
    float g6;
  } MagicGear;
  cCarPhys();
  float speedgearratio(int speed);
  float torqueRPM(float RPM);
  bool donitro(); // return true if nitrous left
  void incspeed();
  void ResetValues();
  void Iterate();
  void Stop();
};


#endif
