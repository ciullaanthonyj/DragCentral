//---------------------------------------------------------------------------

#ifndef exceptFormH
#define exceptFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "sensor.h"
#include "sensortab.h"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class Texcept : public TForm
{
__published:	// IDE-managed Components
        TMemo *memo;
        TImage *Image1;
        TSensor *btnclose;
        TImage *Image3;
        TSensorTab *SensorTab1;
        TSensor *Sensor1;
        void __fastcall Image2Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall Sensor1Click(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        AnsiString WebSite;
        __fastcall Texcept(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Texcept *except;
//---------------------------------------------------------------------------
#endif
