//---------------------------------------------------------------------------

#ifndef SensorTabH
#define SensorTabH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include "sensor.h"
//---------------------------------------------------------------------------
class PACKAGE TSensorTab : public TWinControl
{
private:
    TSensor* fSensor;
protected:
    DYNAMIC void __fastcall KeyPress(wchar_t &Key);
    DYNAMIC void __fastcall DoEnter(void);
    DYNAMIC void __fastcall DoExit(void);
public:
        __fastcall TSensorTab(TComponent* Owner);
    virtual void __fastcall SetBounds(int ALeft, int ATop, int AWidth, int AHeight);

__published:
   __property TabOrder;
   __property TabStop;
   __property TSensor* Sensor = { read=fSensor, write=fSensor};
};
//---------------------------------------------------------------------------
#endif
