//---------------------------------------------------------------------------

#ifndef form_getpassH
#define form_getpassH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sensor.h"
#include "sensortab.h"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class Tgetpass : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TImage *Image22;
        TImage *Image23;
        TSensor *btncreate;
        TSensor *registerback;
        TSensorTab *SensorTab4;
        TSensorTab *SensorTab5;
        TEdit *edforgotemail;
	TLabel *Label1;
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btncreateClick(TObject *Sender);
        void __fastcall registerbackClick(TObject *Sender);
        void __fastcall edforgotemailKeyPress(TObject *Sender, char &Key);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall Tgetpass(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tgetpass *getpass;
//---------------------------------------------------------------------------
#endif
