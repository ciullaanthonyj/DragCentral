
//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "form_distributefunds.h"
#include "myvcl.h"
#include "common_stuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgrbutton"
#pragma resource "*.dfm"
Tdistributefunds *distributefunds;
//---------------------------------------------------------------------------
__fastcall Tdistributefunds::Tdistributefunds(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tdistributefunds::btntransferClick(TObject *Sender)
{
  int value=StrToInt(edAmount->Text);
  if(value<=0)
    throw Exception("Enter positive value.");
  if(value>TeamAccount)
    throw Exception("Team doesn't have enough money.");

  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall Tdistributefunds::FormShow(TObject *Sender)
{
  edAmount->Text = "";        
}
//---------------------------------------------------------------------------
void __fastcall Tdistributefunds::edAmountKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Key==13)
    btntransfer->OnClick(0);
}
//---------------------------------------------------------------------------
void __fastcall Tdistributefunds::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Key==27)
    ModalResult = mrCancel;        
}
//---------------------------------------------------------------------------
void __fastcall Tdistributefunds::membersDrawCell(TObject *Sender,
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

void __fastcall Tdistributefunds::Button2Click(TObject *Sender)
{
  ModalResult = mrCancel;        
}
//---------------------------------------------------------------------------

