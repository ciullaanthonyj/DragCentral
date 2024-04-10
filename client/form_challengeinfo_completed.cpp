//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "form_challengeinfo_completed.h"
#include "myvcl.h"
#include "challenge.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tchallengeinfo_completed *challengeinfo_completed;
//---------------------------------------------------------------------------
__fastcall Tchallengeinfo_completed::Tchallengeinfo_completed(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tchallengeinfo_completed::versusClick(TObject *Sender)
{
  AnsiString init = GridCell(versus,7);
  AnsiString ans = GridCell(versus,8);

  init = replchar('x','|',init);
  ans = replchar('x','|',ans);

{
  cStringTable tbl(init);

  Challenge.Car.ReactionTime = StrToFloat(tbl++);
  Challenge.Car.RaceTime = StrToFloat(tbl++);
  Challenge.Car.BracketTime = StrToFloat(tbl++);
  Challenge.Car.FalseStart = (tbl++==S_1);
}

{

  cStringTable tbl(ans);

  Challenge.Opponent.ReactionTime = StrToFloat(tbl++);
  Challenge.Opponent.RaceTime = StrToFloat(tbl++);
  Challenge.Opponent.BracketTime = StrToFloat(tbl++);
  Challenge.Opponent.FalseStart = (tbl++==S_1);
}

  lbname->Caption = GridCell(versus,2);
  lbcar->Caption =  Challenge.Car.ResultStr(Bracket);
  lbopname->Caption = GridCell(versus,5);
  lbop->Caption = Challenge.Opponent.ResultStr(Bracket);

}
//---------------------------------------------------------------------------

void __fastcall Tchallengeinfo_completed::FormShow(TObject *Sender)
{
  versus->OnClick(0);        
}
//---------------------------------------------------------------------------

void __fastcall Tchallengeinfo_completed::versusDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
  TStringGrid* g = (TStringGrid*)Sender;
  g->Canvas->Font = g->Font;
  if(State.Contains(gdSelected))
     g->Canvas->Brush->Color = GridSelectColor;
  else
     g->Canvas->Brush->Color = clBlack;
  g->Canvas->FillRect(Rect);
//   SetBkMode(inroom->Canvas->Handle,TRANSPARENT);
  g->Canvas->TextOut(Rect.Left+2,Rect.Top+2,g->Cells[ACol][ARow]);
//  if(State.Contains(gdFocused))
//    g->Canvas->DrawFocusRect(Rect);
        
}
//---------------------------------------------------------------------------

