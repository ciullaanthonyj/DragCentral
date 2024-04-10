//---------------------------------------------------------------------------

#ifndef form_challengeinfoH
#define form_challengeinfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "cgrbutton.h"
//---------------------------------------------------------------------------
class Tchallengeinfo : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *versus;
        TGrButton *btnaccept;
        TGrButton *btndecline;
        TGrButton *btnrace;
        TImage *Image3;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnacceptClick(TObject *Sender);
        void __fastcall btndeclineClick(TObject *Sender);
        void __fastcall btnraceClick(TObject *Sender);
        void __fastcall versusDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
private:	// User declarations
public:		// User declarations
        __fastcall Tchallengeinfo(TComponent* Owner);
        void Reset() {
          accept = false;
          decline = false;
          race = false;
        }
        bool accept;
        bool decline;
        bool race;
};
//---------------------------------------------------------------------------
extern PACKAGE Tchallengeinfo *challengeinfo;
//---------------------------------------------------------------------------
#endif
