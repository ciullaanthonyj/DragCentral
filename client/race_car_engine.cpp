//---------------------------------------------------------------------------


#pragma hdrstop

#include "race_car_engine.h"

//---------------------------------------------------------------------------
bool cCarPhys::donitro() {
  if (!Nitro.Installed) return false;
  if (Nitro.Left<=0) return false;
  Nitro.Left -=0.022;
  Nitro.TimeLeft = 1;
  return true;
}


  cCarPhys::cCarPhys() {
     prevspeednum = -2;
     maxspeednum = 5;
     mass = 1140;
     horses = 448;
     wheelradius = 0.33;
     differential = 3.07;
     transmission = 0.7;
     frontalarea = 2.2; // square meters
     airdensity = 1.29; // kg/m3
     cdrag = 0.42;
     crr = 12.8;
     velocity = 0;
     carvelocity = 0;
     iteratelength = 0.030;
     speednum = 0;
     throttle = 0;
     position = 0;
     reduceslip = 0;
     Nitro.Installed = false;
     Nitro.Left = 0;
     Braking = false;
     // this is curve for horses - 448
     TorqueCurve.Add(0,100);
     TorqueCurve.Add(2000,340);
     TorqueCurve.Add(2500,400);
     TorqueCurve.Add(3000,460);
     TorqueCurve.Add(3500,490);
     TorqueCurve.Add(4000,500);
     TorqueCurve.Add(4500,480);
     TorqueCurve.Add(5000,400);
     TorqueCurve.Add(6000,0);

     // Turbo charger (by SVT)
     TurboCharger.Add(0,0.70);		//Turbo-lag. Torque is decreased
     TurboCharger.Add(500,0.75);		//due to energy needed to spool
     TurboCharger.Add(1000,0.80);		//turbochargers.
     TurboCharger.Add(1500,0.90);
     TurboCharger.Add(2000,1.00);
     TurboCharger.Add(2500,1.05);
     TurboCharger.Add(3000,1.10);
     TurboCharger.Add(3500,1.15);
     TurboCharger.Add(4000,1.20);
     TurboCharger.Add(4500,1.25);
     TurboCharger.Add(5000,1.30);
     TurboCharger.Add(5500,1.35);
     TurboCharger.Add(6000,1.30);
     TurboCharger.Add(6500,1.20);
     TurboCharger.Add(7000,1.15);
     TurboCharger.Add(7500,1.10);
     TurboCharger.Add(8000,1);
     TurboCharger.Add(8500,1);
     TurboCharger.Add(9000,1);

     // Super Charger by SVT
     SuperCharger.Add(0,1.35);		//? No Lag J
     SuperCharger.Add(500,1.35);
     SuperCharger.Add(1000,1.35);
     SuperCharger.Add(1500,1.35);
     SuperCharger.Add(2000,1.40);
     SuperCharger.Add(2500,1.25);
     SuperCharger.Add(3000,1.25);
     SuperCharger.Add(3500,1.20);
     SuperCharger.Add(4000,1.20);
     SuperCharger.Add(4500,1.20);
     SuperCharger.Add(5000,1.20);
     SuperCharger.Add(5500,1.15);
     SuperCharger.Add(6000,1.15);
     SuperCharger.Add(6500,1.10);
     SuperCharger.Add(7000,1.10);
     SuperCharger.Add(7500,1.10);
     SuperCharger.Add(8000,1);
     SuperCharger.Add(8500,1);
     SuperCharger.Add(9000,1);

  }
  float cCarPhys::speedgearratio(int speed) {

    if(!MagicGear.Installed) {
      switch (speed) {
        case -1: return 3.5;
        case 0: return 0;
        case 1: return 3.5;
        case 2: return 2.2;
        case 3: return 1.5;
        case 4: return 1.1;
        case 5: return 0.94;
        case 6: return 0.763;
      }
    } else {
      switch (speed) {
        case -1: return 3.5;
        case 0: return 0;
        case 1: return MagicGear.g1;
        case 2: return MagicGear.g2;
        case 3: return MagicGear.g3;
        case 4: return MagicGear.g4;
        case 5: return MagicGear.g5;
        case 6: return MagicGear.g6;
      }
    }


    return 0;
  }
  float cCarPhys::torqueRPM(float RPM) {
     return TorqueCurve.xtoy(RPM);
  }
  void cCarPhys::incspeed() {
    if(speednum<maxspeednum) speednum++;
  }
  void cCarPhys::ResetValues() {
    throttle = 0;
    speednum = 0;
    velocity = 0;
    carvelocity = 0;
    RPM = 0;
    prevRPM = 0;
    position = 0;
    Nitro.TimeLeft = 0;
  }

  void cCarPhys::Stop() {
     velocity = 0;
     carvelocity = 0;
  }
  void cCarPhys::Iterate() {

    float FAir = cdrag * velocity * velocity;
    float FRubberRoad = crr * mod(velocity);
    float FEngine=0;


    // resistance must be negative to velocity
    FAir = -1*sign(velocity)*FAir;
    FRubberRoad = -1*sign(velocity)*FRubberRoad;

    float nitrohorses = 0;
    if(Nitro.Installed && Nitro.TimeLeft >0) {
      Nitro.TimeLeft -= iteratelength;
      Nitro.TimeLeft <= 0 ? Nitro.TimeLeft = 0 : 0;
      nitrohorses = Nitro.Horses;
    }

    float torque = throttle*torqueRPM(RPM)*(horses+nitrohorses)*4.48/448;
    // apply engine damage
    torque = torque * (130.-enginedamage)/ 130;

    if(Charger.turbocharger)
      torque = torque*TurboCharger.xtoy(RPM);
    if(Charger.supercharger)
      torque = torque*SuperCharger.xtoy(RPM);



    //START FROM ZERO SPEED
    if(prevspeednum==0 && speednum!=0) {
	  velocity = RPM / (20. *  3.6 * 60 / (2*M_PI)) * wheelradius;
	  if(speednum==-1) velocity = -velocity;
    }
    
    if(speednum!=0) {
      float gearratio = speedgearratio(speednum);
      float differentialratio = 3.6;
      float transmissioneffeciency = 0.7;
      float wheelangularvel = mod(velocity) / wheelradius;
      RPM = wheelangularvel * gearratio *
        differentialratio * 60 / (2*M_PI);
      float engineforce = torque * gearratio * differentialratio *
        transmissioneffeciency / wheelradius;
      FEngine = engineforce;
    }




    // NEUTRAL SPEED
    if(speednum==0) {
      float engineforce = RPM*1/5000;
      engineforce += torque*0.004*iteratelength;

      // resistance
      float ResistForce = 0.1*RPM*0.0005*iteratelength;
      engineforce -=ResistForce;
      RPM = engineforce * 5000;
    }

      float FBraking = 0;
      if(Braking && velocity!=0) {
         FBraking = -sign(velocity)*22000; // Newtons
      }

      if(Braking) FEngine = 0;

    //REVERSE SPEED
    if(speednum==-1) {
      //SPEED CHANGED TO REVERSE
      if(prevspeednum!=-1) {
         if(mod(velocity)>DAMAGEVELOCITY) {
            addenginedamage((mod(velocity)-DAMAGEVELOCITY)*(mod(velocity)-DAMAGEVELOCITY)*0.01);
         }
         if(velocity>0)
           velocity = 0;
      }
      FEngine = -FEngine;
    }

    if(speednum>0) {
      //SPEED CHANGED TO FORWARD FROM NEUTRAL OR REVERSE
      if(prevspeednum<=0) {
        if(mod(velocity)>DAMAGEVELOCITY) {
            addenginedamage((mod(velocity)-DAMAGEVELOCITY)*(mod(velocity)-DAMAGEVELOCITY)*0.01);
        }
        if(velocity<0)
          velocity = 0;
      }
    }

    // FAST RPM CHANGING DAMAGE
    float RPMDiff = mod(RPM-prevRPM);
    if(RPMDiff>RPMDAMAGEDIFF && prevspeednum!=0) {
       addenginedamage((RPMDiff-RPMDAMAGEDIFF)*(RPMDiff-RPMDAMAGEDIFF)/1000000);
    }

    prevRPM = RPM;

    prevspeednum = speednum;

    float Flong = FEngine + FAir + FRubberRoad + FBraking;

    a = Flong / mass;

    float startvel = velocity;

    losttraction = mod(Flong) > (mass * 7 * (100+reduceslip)/100);
    if(Braking) losttraction = false;

    if(losttraction)
      velocity += a * iteratelength / 25;
    else
      velocity += a * iteratelength;

    // change sign
    if(startvel*velocity<0)
      velocity = 0;

    carvelocity = velocity;
    position += carvelocity * iteratelength;

    KMH = velocity * 3600/1000;
    MPH = KMH / 1.609;
  }

#pragma package(smart_init)
