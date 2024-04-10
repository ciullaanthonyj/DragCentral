//---------------------------------------------------------------------------

#ifndef form_teamchallengeH
#define form_teamchallengeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <jpeg.hpp>
#include "cgrbutton.h"
//---------------------------------------------------------------------------
class Tcreatechallenge : public TForm
{
__published:	// IDE-managed Components
        TRadioButton *forfun;
        TPanel *Panel1;
        TRadioButton *bracket;
        TRadioButton *headsup;
        TRadioButton *formoney;
        TEdit *edWager;
        TStringGrid *versus;
        TGrButton *start;
        TLabel *Label1;
        TLabel *lbwager;
        TGrButton *btnadd;
        TGrButton *reset;
        TListBox *yourmember;
        TListBox *othermember;
        TLabel *Label2;
        TLabel *lbTeamAccount;
        TImage *Image3;
        TImage *Image2;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnaddClick(TObject *Sender);
        void __fastcall resetClick(TObject *Sender);
        void __fastcall formoneyClick(TObject *Sender);
        void __fastcall edWagerExit(TObject *Sender);
        void __fastcall startClick(TObject *Sender);
        void __fastcall versusDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
private:	// User declarations
public:		// User declarations
        __fastcall Tcreatechallenge(TComponent* Owner);
        AnsiString yourteam;
        AnsiString otherteam;
        AnsiString teamaccount;
        AnsiString challengers;
};
//---------------------------------------------------------------------------
extern PACKAGE Tcreatechallenge *createchallenge;
//---------------------------------------------------------------------------
#endif
