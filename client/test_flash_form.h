//---------------------------------------------------------------------------

#ifndef test_flash_formH
#define test_flash_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cgrbutton.h"
#include "ShockwaveFlashObjects_OCX.h"
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
class Tmainform : public TForm
{
__published:	// IDE-managed Components
        TGrButton *btngotomain;
        TShockwaveFlash *flcarpanel;
        TShockwaveFlash *flupview;
        TShockwaveFlash *flstaging;
        TShockwaveFlash *flroad;
        TTimer *autoexec;
        TButton *Button1;
        TTimer *race;
        TScrollBar *ScrollBar1;
        void __fastcall autoexecTimer(TObject *Sender);
        void __fastcall raceTimer(TObject *Sender);
        void __fastcall ScrollBar1Change(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall Tmainform(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tmainform *mainform;
//---------------------------------------------------------------------------
#endif
