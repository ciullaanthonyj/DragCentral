//---------------------------------------------------------------------------

#ifndef teammessage_postH
#define teammessage_postH
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
class Tteammessage : public TForm
{
__published:	// IDE-managed Components
        TGrButton *btnyes;
        TGrButton *Cancel;
        TLabel *Cap;
        TMemo *message;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnyesClick(TObject *Sender);
        void __fastcall CancelClick(TObject *Sender);
        void __fastcall messageKeyPress(TObject *Sender, char &Key);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall Tteammessage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tteammessage *teammessage;
//---------------------------------------------------------------------------
#endif
