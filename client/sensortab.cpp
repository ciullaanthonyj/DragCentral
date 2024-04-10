//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SensorTab.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TSensorTab *)
{
		new TSensorTab(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSensorTab::TSensorTab(TComponent* Owner)
        : TWinControl(Owner)
{
  TabStop = true;
}
void __fastcall TSensorTab::SetBounds(int ALeft, int ATop, int AWidth, int AHeight) {
  if(!ComponentState.Contains(csDesigning))
     AWidth = 0;
  TWinControl::SetBounds(ALeft,ATop,AWidth,AHeight);
}

void __fastcall TSensorTab::KeyPress(wchar_t &Key) {
  if(ComponentState.Contains(csDesigning)) return;
  if(Key==32 || Key==13)
  if(fSensor!=NULL)
  if(fSensor->OnClick!=NULL)
	fSensor->OnClick(fSensor);
}

void __fastcall TSensorTab::DoEnter(void) {
  if(ComponentState.Contains(csDesigning)) return;
  if(fSensor!=NULL)
	fSensor->msgTabEnter();
  TWinControl::DoEnter();
}
void __fastcall TSensorTab::DoExit(void) {
  if(ComponentState.Contains(csDesigning)) return;
  if(fSensor!=NULL)
    fSensor->msgTabExit();
  TWinControl::DoExit();
}

//---------------------------------------------------------------------------
namespace Sensortab
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TSensorTab)};
                 RegisterComponents("Samples", classes, 0);
        }
}
//---------------------------------------------------------------------------
 