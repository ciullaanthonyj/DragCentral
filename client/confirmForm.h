//---------------------------------------------------------------------------

#ifndef confirmFormH
#define confirmFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Graphics.hpp>
#include "cgrbutton.h"
//---------------------------------------------------------------------------
class Trequest : public TForm
{
__published:	// IDE-managed Components
        TGrButton *btnyes;
        TGrButton *Button2;
	TLabel *mess;
	TImage *Image1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnyesClick(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        __fastcall Trequest(TComponent* Owner);
        bool confirm(AnsiString mess);
};
//---------------------------------------------------------------------------
extern PACKAGE Trequest *request;
//---------------------------------------------------------------------------
#endif
