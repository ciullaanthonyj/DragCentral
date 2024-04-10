//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "inputForm.h"
#include "myvcl.h"
#include "common_stuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma link "cgrbutton"
#pragma resource "*.dfm"
Tinput *input;
//---------------------------------------------------------------------------
__fastcall Tinput::Tinput(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall Tinput::FormShow(TObject *Sender)
{
  this->Top = (Screen->Height - this->Height) / 2 - 42;
  this->Left = (Screen->Width - this->Width) / 2;

  Result->SetFocus();
  Result->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall Tinput::btnyesClick(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall Tinput::CancelClick(TObject *Sender)
{
  Result->Text = "";
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------


void __fastcall Tinput::ResultKeyPress(TObject *Sender, char &Key)
{
  if(Key==13)
	btnyesClick(0);
}
//---------------------------------------------------------------------------

void __fastcall Tinput::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Key==VK_ESCAPE) {
    CancelClick(0);
  }
}
//---------------------------------------------------------------------------


