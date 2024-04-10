//---------------------------------------------------------------------------

#ifndef roomlistFormH
#define roomlistFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "chatserver.h"
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "sensor.h"
#include "sensortab.h"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class Trooms : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *list;
        TImage *Image8;
        TSensor *btnclose;
        TImage *Image26;
        TImage *Image1;
        TSensor *btncreate;
        TImage *Image2;
        TSensor *btnenter;
        TTimer *autoselect;
        TLabel *lbCurrentRoom;
        TSensorTab *SensorTab1;
        TSensorTab *SensorTab2;
        TSensorTab *SensorTab3;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall listDblClick(TObject *Sender);
        void __fastcall btncloseClick(TObject *Sender);
        void __fastcall listKeyPress(TObject *Sender, char &Key);
        void __fastcall btncreateClick(TObject *Sender);
        void __fastcall autoselectTimer(TObject *Sender);
        void __fastcall Image8MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall listDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
private:	// User declarations
public:		// User declarations
        __fastcall Trooms(TComponent* Owner);
        cChatServer* Server;
        TStringList* ids;
        AnsiString room;
        AnsiString roomname;
        bool Changed;
        bool testingAutoPass;
        AnsiString roomlist;
};
//---------------------------------------------------------------------------
extern PACKAGE Trooms *rooms;
//---------------------------------------------------------------------------
#endif
