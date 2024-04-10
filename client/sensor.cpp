//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "Sensor.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TSensor *)
{
        new TSensor(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSensor::TSensor(TComponent* Owner)
        : TImage(Owner)
{
  firsttime = true;
  TabEntered = false;
}

void TSensor::msgTabEnter() {
  TabEntered = true;
  TMessage dummy;
  CMMouseEnter(dummy);
};
void TSensor::msgTabExit() {
  TabEntered = false;
  TMessage dummy;
  CMMouseLeave(dummy);
};

void __fastcall TSensor::CMMouseEnter(Messages::TMessage &Message) {
  if(!ComponentState.Contains(csDesigning))
  if(RolloverImage!=NULL) {
    if(firsttime) {
      if(ParamStr(1)=="-nojumps")
        RolloverImage->Top += 1;
      firsttime = false;
    }
    RolloverImage->Visible = true;
  }
}
void __fastcall TSensor::CMMouseLeave(Messages::TMessage &Message) {
  if(!ComponentState.Contains(csDesigning)) {
    if(!TabEntered)
	  if(RolloverImage!=NULL) {
		RolloverImage->Visible = false;
      }
  }
}

//---------------------------------------------------------------------------
namespace Sensor
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TSensor)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------
 