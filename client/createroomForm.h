//---------------------------------------------------------------------------

#ifndef createroomFormH
#define createroomFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "chatserver.h"
#include "sensor.h"
#include "sensortab.h"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class Tcreateroom : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TEdit *edroom;
        TEdit *edpass;
        TImage *Image2;
        TSensor *btnclose;
        TImage *Image3;
        TSensor *btncreate;
        TSensorTab *SensorTab1;
        TSensorTab *SensorTab2;
	TLabel *Label62;
	TLabel *Label1;
        void __fastcall btncloseClick(TObject *Sender);
        void __fastcall btncreateClick(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall edpassKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        __fastcall Tcreateroom(TComponent* Owner);
        cChatServer* Server;
};
//---------------------------------------------------------------------------
extern PACKAGE Tcreateroom *createroom;
//---------------------------------------------------------------------------
#endif
