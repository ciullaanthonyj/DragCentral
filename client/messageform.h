//---------------------------------------------------------------------------

#ifndef messageformH
#define messageformH
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
class Tmessage : public TForm
{
__published:	// IDE-managed Components
        TGrButton *btnyes;
        TLabel *mess;
        TImage *Image3;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnyesClick(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        __fastcall Tmessage(TComponent* Owner);
        void ShowMessage(AnsiString message) {
           mess->Caption = message;
           this->ShowModal();
        }
};
//---------------------------------------------------------------------------
extern PACKAGE Tmessage *message;
//---------------------------------------------------------------------------
#endif
