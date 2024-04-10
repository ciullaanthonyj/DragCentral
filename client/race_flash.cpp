//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop

#include "race_flash.h"
#include "cprofiler.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
MoviePos startmovie[5];
MoviePos curmovie[5];
MoviePos LineFar;
MoviePos LineNear;
MoviePos Line;
MoviePos CarNear;
MoviePos CarFar;
MoviePos CarP;
MoviePos LeftTreeFar;
MoviePos LeftTreeNear;
MoviePos RightTreeFar;
MoviePos RightTreeNear;
MoviePos TreeP;
double race_trackPos = 0;
double dh;  // screensize = focus * realsize/realdistance
double carDH;
double FarDist;
double NearDist = 10.0; // distance to near car from viewer
double MarksDist;
double ViewHeight = 4.0;
cRaceFlash RaceFlash;



MoviePos cursorup;
MoviePos cursordown;
MoviePos cursor;
void race_InitStaging(TShockwaveFlash* fl) {
  DecimalSeparator = '.';
  cursorup.getvars(fl,"cursor0");
  cursordown.getvars(fl,"cursor");
  flvisible(fl,"cursor0",false);
}




void cRaceFlash::ShowCar(double RoadDistance)
{  if(RoadDistance<0) {
      flroad.setvisible("leftcarnear",false);
   } else {
     flroad.setvisible("leftcarnear",true);
     double Distance = RoadDistance;
     if(Distance!=0) {
       double width = carDH/Distance;
       CarP.scale(CarNear.width,CarFar.width,width,CarNear,CarFar);
       CarP.setstringvars(flroad.mv,"leftcarnear");
     }
   }
}



  void cRaceFlash::HideCar(AnsiString prefix) {
    flroad.setvisible(prefix+"carfar",false);
    flroad.setvisible(prefix+"carnear",false);
  }

  void cRaceFlash::ShowInfo(AnsiString prefix, bool visible) {
    flroad.setvisible(prefix+"bracketcaption",visible);
    flroad.setvisible(prefix+"reactioncaption",visible);
    flroad.setvisible(prefix+"speedcaption",visible);
    flroad.setvisible(prefix+"timecaption",visible);
    flroad.setvisible(prefix+"bracket",visible);
    flroad.setvisible(prefix+"react",visible);
    flroad.setvisible(prefix+"speed",visible);
    flroad.setvisible(prefix+"time",visible);
  }

  void cRaceFlash::SelectCar(AnsiString prefix,AnsiString carid) {
    AnsiString carclip;
    if(prefix=="left")
      carclip="leftcarnear";
    if(prefix=="right")
      carclip="car";
    flroad.setvar("_root."+carclip+".carnum",carid);

  }

  void cRaceFlash::SetMinSpeed(AnsiString value) {
	flsetvar(this->flcarpanel,"_root.minspeednum",value);
  }

  void cRaceFlash::SetMaxSpeed(AnsiString value) {
	flsetvar(this->flcarpanel,"_root.maxspeednum",value);
  }

  void cRaceFlash::SetUpview(double pos,AnsiString prefix) {
    double y;
    y = prop(TrackLength,-StagingLength,pos,3,360);
    if(y<1) y = 1;
    if(y>400) y = 400;
//    flupview->SetVariable(wc2("car"+prefix+"._y"),wc(f2s(y)));
    flsetvar(flupview,"car"+prefix+"._y",f2s(y));
    flvisible(flupview,"car"+prefix,true);
  }
  void cRaceFlash::ShowBracketTime(double time,int track) {
    AnsiString prefix = "right";
    if(track==0) prefix = "left";
    AnsiString value;
    // IF NOT BRACKET RACE
    if(time==0)
      value = "-";
    else
      value = FloatToStrF(time,ffFixed,5,3);
    flroad.setvar(prefix+"bracket.text",value);
  }
  void cRaceFlash::ShowBreakOut(AnsiString prefix,bool visible) {
     flroad.setvisible(prefix+"breakout",visible);
  }

  bool cRaceFlash::GetBraking() {
	AnsiString s = flcarpanel->GetVariable(L"_root.braking");
	return (s!="0");
  }

  void cRaceFlash::FinishedInfo(double time,bool BreakOut,AnsiString prefix) {
    if(BreakOut) {
       ShowBreakOut(prefix,true);
    }
    if(time==-1) // UNDEFINED
      flroad.setvar(prefix+"time.text","-");
    else
      flroad.setvar(prefix+"time.text",FloatToStrF(time,ffFixed,5,3));

    if(time==-1) // UNDEFINED
      flroad.setvar(prefix+"speed.text","-");
    else {
      double speed;
      if(time==0)
        speed=0;
      else
        speed = TrackLength / time; // m/s
      speed = ((speed/1000)/1.609 * 3600);
      flroad.setvar(prefix+"speed.text",FloatToStrF(speed,ffFixed,5,3));
    }
  }

  double cRaceFlash::GetGas() {
	 double Gas = StrToFloat(flcarpanel->GetVariable(L"_root.gasvalue"));
     if(Gas>1) Gas = 1;
     if(Gas<0) Gas = 0;
     return Gas;
  }

  void cRaceFlash::SetSpeedNum(int newspeednum) {
	 //flcarpanel->SetVariable(L"_root.speednum",wc(IntToStr(newspeednum)));
	 flsetvar(flcarpanel,"_root.speednum",IntToStr(newspeednum));
  }
  int cRaceFlash::GetSpeedNum() {
     PSTART("getspeednum");
	 AnsiString res=flgetvar(flcarpanel,"_root.speednum");
	 PEND("getspeednum");
     return StrToInt(res);
  }


  void cRaceFlash::showLostTraction(bool visible) {
	 flvisible(flcarpanel,"losttraction",visible);
	 if(visible)
	   flsetvar(flcarpanel,"_root.dashboard.lampon._visible",1);
	 else
	   flsetvar(flcarpanel,"_root.dashboard.lampon._visible",0);
  }
  bool cRaceFlash::SkipStaging() {
	 bool res = StrToInt(flgetvar(flcarpanel,"_root.skipstaging"));
	 return res;
  }
  void cRaceFlash::HideSkipStagingButton() {
	flvisible(RaceFlash.flcarpanel,"skipbtn",false);
  }
  void cRaceFlash::msgGameStart() {
      ReactionTime(-1,false,"right");
      ReactionTime(-1,false,"left");
      FinishedInfo(-1,false,"left");
      FinishedInfo(-1,false,"right");
      ShowBreakOut("left",false);
      ShowBreakOut("right",false);
      ShowEndMessage("");
      SetSpeedNum(0);
      PrestageLightsVisible(false,0);
      flvisible(flupview,"carright",false);
	  flvisible(flupview,"carleft",false);
	  flvisible(flcarpanel,"skipbtn",true);
	  flsetvar(flcarpanel,"_root.skipbtn._alpha",100);
	  //because it is set to 0, on press
	  flsetvar(flcarpanel,"_root.skipstaging",0);
      showLostTraction(false);
      LightsTurned[0] = false;
      LightsTurned[1] = false;
      LightsTurned[2] = false;
      LightsTurned[3] = false;
      racertrack = "right";
      ChangeTerrain(-30,true);
  }

  void cRaceFlash::ShowEndMessage(AnsiString mess) {
     flroad.setvar("endmessage.text",mess);
  }

  void cRaceFlash::PrestageLightsVisible(bool visible,double Position) {
      if(!visible) {
        flroad.setvisible("prestaged",false);
        flroad.setvisible("staged",false);
        flroad.setvisible("prestagedlight",false);
        flroad.setvisible("stagedlight",false);
        return;
      }
      flroad.setvisible("prestaged",true);
      flroad.setvisible("staged",true);

      if(-StagingDistance*2<=Position && Position<=StagingDistance*2) {
        flroad.setvisible("prestagedlight",true);
      } else {
        flroad.setvisible("prestagedlight",false);
      }
      if(-StagingDistance<=Position && Position<=StagingDistance) {
        flroad.setvisible("stagedlight",true);
      } else {
        flroad.setvisible("stagedlight",false);
      }

  }

  // track 0 or 1
  void cRaceFlash::StartLights(double time,bool FalseStart, AnsiString prefix) {
    if(time==0) {// || len>10) {
      flroad.setvisible(prefix+"lamp1light",false);
      flroad.setvisible(prefix+"lamp2light",false);
      flroad.setvisible(prefix+"lamp3light",false);
      flroad.setvisible(prefix+"lamp4light",false);
      flroad.setvisible(prefix+"lamp5light",false);
      flroad.setvisible(prefix+"lamp1",false);
      flroad.setvisible(prefix+"lamp2",false);
      flroad.setvisible(prefix+"lamp3",false);
      flroad.setvisible(prefix+"lamp4",false);
      flroad.setvisible(prefix+"lamp5",false);
      return;
    }
    if(0<time) {
      flroad.setvisible(prefix+"lamp1",true);
      flroad.setvisible(prefix+"lamp2",true);
      flroad.setvisible(prefix+"lamp3",true);
      flroad.setvisible(prefix+"lamp4",true);
      flroad.setvisible(prefix+"lamp5",true);
    }
    if(!FalseStart) {
      double Period = StartLightsWholeLength / 4;
      if(Period<time && time<=Period*2) {
        flroad.setvisible(prefix+"lamp1light",true);
        if(prefix==racertrack) LightsTurned[0] = true;
      }
      if(Period*2<=time && time<=Period*3) {
        flroad.setvisible(prefix+"lamp2light",true);
        if(prefix==racertrack) LightsTurned[1] = true;
      }
      if(Period*3<=time && time<=Period*4) {
        flroad.setvisible(prefix+"lamp3light",true);
        if(prefix==racertrack) LightsTurned[2] = true;
      }
      if(Period*4<=time && time<=Period*5) {
        flroad.setvisible(prefix+"lamp4light",true);
        if(prefix==racertrack) LightsTurned[3] = true;
      }
    }
  }

  void cRaceFlash::ReactionTime(double time, bool FalseStart, AnsiString track) {
    AnsiString Prefix = track;
    if(FalseStart) {
       flroad.setvar(Prefix+"react.text","FOUL");
    } else {
      if(time!=-1)
        flroad.setvar(Prefix+"react.text",FloatToStrF(time,ffFixed,7,3));
      else
        flroad.setvar(Prefix+"react.text","-");
    }
  }

  void cRaceFlash::LightFalseStart() {
    flroad.setvisible("rightlamp5light",true);
  }

  void cRaceFlash::LoadFlashMovies() {
    flroad.mv->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\race.swf"));
    flupview->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\track.swf"));
	flupview->SetVariable(L"_root.carright_color",L"0xff0000");
//	flcarpanel->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\car-panel.swf"));
	flstaging->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\staging.swf"));

	// make invisible
	this->StartLights(0,false,"left");
	this->StartLights(0,false,"right");

	InitStaging();
	InitTerrain();
  }



// THIS SHOWS OBJECT IF WE KNOW WHAT WIDTH IT WILL BE ON SOME DISTANCE
// AND KNOW TWO POINTS OF IT'S POSITIONS LINE
void ShowTree(MoviePos& TreeNear, MoviePos& TreeFar, AnsiString name, double RoadPosition) {
    double Distance = sqrt(RoadPosition*RoadPosition+6*6);
    double NearTreeDistance = 9;
    double DH = TreeNear.width*NearTreeDistance;
    if(Distance!=0) {
       double width = DH/Distance;
       TreeP.scale(TreeNear.width,TreeFar.width,width,TreeNear,TreeFar);
       TreeP.setstringvars(RaceFlash.flroad.mv,name);
    }
}

//     }
void cRaceFlash::ChangeTerrain(double Position, bool directset) {
  TShockwaveFlash* fl = flroad.mv;
  bool Vis = false;
  // START LINE

      if(Position>=-60 && Position<60) {
    
    	 double RoadDistance = -Position + NearDist;
    
    	 if(RoadDistance>0) {
    
    	   double Distance = sqrt(RoadDistance*RoadDistance + ViewHeight*ViewHeight);
    	   double width = dh/Distance;
    	   Line.scale(LineNear.width,LineFar.width,width,LineNear,LineFar);
    	   if(directset)
    		 Line.setvars(fl,"line");
    	   else
    		 Line.setstringvars(fl,"line");
    	   Vis = true;
    	 }
    
    
	  }else
      // FINISH LINE
      if(Position>RaceFlash.TrackLength-60 && Position<RaceFlash.TrackLength+60) {

    	 double RoadDistance = RaceFlash.TrackLength-Position + NearDist;
    
    	 if(RoadDistance>0) {
    
    	   double Distance = sqrt(RoadDistance*RoadDistance + ViewHeight*ViewHeight);
    	   double width = dh/Distance;
    	   Line.scale(LineNear.width,LineFar.width,width,LineNear,LineFar);
    	   Line.setstringvars(fl,"line");
    	   Vis = true;
    	 }
    
      } else {
    	 Vis = false;
      }
    
      flvisible(fl,"line",Vis);
        

  if(Position>=0)
	race_trackPos = Position - MarksDist*((int)(Position/MarksDist));
  else
	race_trackPos = MarksDist - (mod(Position) - MarksDist*((int)(mod(Position)/MarksDist)));


	  // MARKS
	  //removed because of flash invalid floating point operation
//	  curmovie[4].scale(0,MarksDist,race_trackPos,startmovie[4],startmovie[3]);
//	  curmovie[4].setstringvars(fl,"mark4");
	  curmovie[3].scale(0,MarksDist,race_trackPos,startmovie[3],startmovie[2]);
	  curmovie[3].setstringvars(fl,"mark3");
	  curmovie[2].scale(0,MarksDist,race_trackPos,startmovie[2],startmovie[1]);
	  curmovie[2].setstringvars(fl,"mark2");
	  curmovie[1].scale(0,MarksDist,race_trackPos,startmovie[1],startmovie[0]);
	  curmovie[1].setstringvars(fl,"mark1");


// position 0
// trees distance - 5,10,15,20

   // car position 0 = viewer distance 20
   double Distance = 20 - Position;

   PSTARTP("trees","terrain");

	   // on zero calculations bugs
	   Distance -=TreesDistance*100+8;
	   double Offset = Distance-int(Distance/TreesDistance)*TreesDistance;
	   ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_01",Offset+TreesDistance*1);
	   ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_02",Offset+TreesDistance*2);
	   ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_03",Offset+TreesDistance*3);
	   ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_04",Offset+TreesDistance*4);
       ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_05",Offset+TreesDistance*5);
       ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_06",Offset+TreesDistance*6);
    
    //   ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_07",Offset+TreesDistance*7);
    //   ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_08",Offset+TreesDistance*8);
    //   ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_09",Offset+TreesDistance*9);
    //   ShowTree(LeftTreeNear,LeftTreeFar,"left_tree_10",Offset+TreesDistance*10);
    
    
       ShowTree(RightTreeNear,RightTreeFar,"right_tree_01",Offset+TreesDistance*1);
       ShowTree(RightTreeNear,RightTreeFar,"right_tree_02",Offset+TreesDistance*2);
       ShowTree(RightTreeNear,RightTreeFar,"right_tree_03",Offset+TreesDistance*3);
       ShowTree(RightTreeNear,RightTreeFar,"right_tree_04",Offset+TreesDistance*4);
       ShowTree(RightTreeNear,RightTreeFar,"right_tree_05",Offset+TreesDistance*5);
       ShowTree(RightTreeNear,RightTreeFar,"right_tree_06",Offset+TreesDistance*6);
    //   ShowTree(RightTreeNear,RightTreeFar,"right_tree_07",Offset+TreesDistance*7);
    //   ShowTree(RightTreeNear,RightTreeFar,"right_tree_08",Offset+TreesDistance*8);
    //   ShowTree(RightTreeNear,RightTreeFar,"right_tree_09",Offset+TreesDistance*9);
    //   ShowTree(RightTreeNear,RightTreeFar,"right_tree_10",Offset+TreesDistance*10);

   PEND("trees");
}

void cRaceFlash::InitTerrain() {
  TShockwaveFlash* fl = flroad.mv;
  startmovie[0].getvars(fl,"mark0");
  startmovie[1].getvars(fl,"mark1");
  startmovie[2].getvars(fl,"mark2");
  startmovie[3].getvars(fl,"mark3");
  startmovie[4].getvars(fl,"mark4");

  LineFar.getvars(fl,"linefar");
  LineNear.getvars(fl,"line");

  LeftTreeNear.getvars(fl,"left_tree_near");
  LeftTreeFar.getvars(fl,"left_tree_far");
  RightTreeNear.getvars(fl,"right_tree_near");
  RightTreeFar.getvars(fl,"right_tree_far");

  flvisible(fl,"mark0",false);
  flvisible(fl,"linefar",false);
  flvisible(fl,"left_tree_near",false);
  flvisible(fl,"left_tree_far",false);
  flvisible(fl,"right_tree_near",false);
  flvisible(fl,"right_tree_far",false);



  dh = LineNear.width * NearDist;
  FarDist= dh / LineFar.width;
  MarksDist = 6;

  CarNear.getvars(fl,"leftcarnear");
  CarFar.getvars(fl,"leftcarfar");
  carDH = CarNear.width * NearDist;
  flvisible(fl,"leftcarfar",false);

}
void cRaceFlash::InitStaging() {
    race_InitStaging(flstaging);
  }
void cRaceFlash::StagingChange(double Position) {
    cursor.scale(-RaceFlash.StagingLength,RaceFlash.StagingLength,Position,cursordown,cursorup);
    cursor.setvars(flstaging,"cursor");
  }

