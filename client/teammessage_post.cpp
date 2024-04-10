//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "teammessage_post.h"
#include "myvcl.h"
#include "common_stuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma link "cgrbutton"
#pragma resource "*.dfm"
Tteammessage *teammessage;
//---------------------------------------------------------------------------
__fastcall Tteammessage::Tteammessage(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall Tteammessage::FormShow(TObject *Sender)
{
    this->Top = (Screen->Height - this->Height) / 2 - 42;
    this->Left = (Screen->Width - this->Width) / 2;
  message->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall Tteammessage::btnyesClick(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall Tteammessage::CancelClick(TObject *Sender)
{
  message->Text = "";
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------


void __fastcall Tteammessage::messageKeyPress(TObject *Sender, char &Key)
{
  if(Key==13) {
    Key = 0;
    btnyesClick(0);
  }
  if(message->Lines->Text.Length()>=300) {
    int save = message->SelStart;
    message->Lines->Text =
      message->Lines->Text.SubString(1,300);
    message->SelStart = save;
  }
}
//---------------------------------------------------------------------------

void __fastcall Tteammessage::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Key==VK_ESCAPE) {
    CancelClick(0);
  }
}
//---------------------------------------------------------------------------

