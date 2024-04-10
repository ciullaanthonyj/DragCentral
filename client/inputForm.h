//---------------------------------------------------------------------------

#ifndef inputFormH
#define inputFormH
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
class Tinput : public TForm
{
__published:	// IDE-managed Components
        TGrButton *btnyes;
        TGrButton *Cancel;
        TLabel *Message;
        TImage *Image3;
        TEdit *Result;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnyesClick(TObject *Sender);
        void __fastcall CancelClick(TObject *Sender);
        void __fastcall ResultKeyPress(TObject *Sender, char &Key);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall Tinput(TComponent* Owner);
        AnsiString InputBox(AnsiString caption, AnsiString message, AnsiString DefValue) {
           this->Caption = caption;
           this->Message->Caption = message;
           this->Result->Text = DefValue;
           this->ShowModal();
           AnsiString res = this->Result->Text;
           //update screen
           Application->ProcessMessages();
           return res;
        }
};
//---------------------------------------------------------------------------
extern PACKAGE Tinput *input;
//---------------------------------------------------------------------------
#endif
