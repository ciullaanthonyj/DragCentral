//---------------------------------------------------------------------------

#ifndef form_bannedH
#define form_bannedH
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
class Tbanned : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TImage *banned;
        TSensor *btnclose;
        TSensorTab *SensorTab1;
        TMemo *memo;
        void __fastcall btncloseClick(TObject *Sender);
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall memoKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        __fastcall Tbanned(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tbanned *banned;
//---------------------------------------------------------------------------
#endif
