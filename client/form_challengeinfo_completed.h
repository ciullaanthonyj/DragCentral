//---------------------------------------------------------------------------

#ifndef form_challengeinfo_completedH
#define form_challengeinfo_completedH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class Tchallengeinfo_completed : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *versus;
        TLabel *lbcar;
        TLabel *lbop;
        TLabel *lbname;
        TLabel *lbopname;
        TImage *Image3;
        void __fastcall versusClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall versusDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
private:	// User declarations
public:		// User declarations
        __fastcall Tchallengeinfo_completed(TComponent* Owner);
        bool Bracket;
};
//---------------------------------------------------------------------------
extern PACKAGE Tchallengeinfo_completed *challengeinfo_completed;
//---------------------------------------------------------------------------
#endif
