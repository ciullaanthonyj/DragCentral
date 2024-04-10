//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "form_challengeinfo.h"
#include "myvcl.h"
#include "common_stuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgrbutton"
#pragma resource "*.dfm"
Tchallengeinfo *challengeinfo;
//---------------------------------------------------------------------------
__fastcall Tchallengeinfo::Tchallengeinfo(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tchallengeinfo::FormShow(TObject *Sender)
{
  this->Top = (Screen->Height - this->Height) / 2 - 42;
  this->Left = (Screen->Width - this->Width) / 2;
  Reset();
}
//---------------------------------------------------------------------------
void __fastcall Tchallengeinfo::btnacceptClick(TObject *Sender)
{
  accept = true;
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall Tchallengeinfo::btndeclineClick(TObject *Sender)
{
  decline = true;
  ModalResult = mrOk;        
}
//---------------------------------------------------------------------------
void __fastcall Tchallengeinfo::btnraceClick(TObject *Sender)
{
  race = true;
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall Tchallengeinfo::versusDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
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


