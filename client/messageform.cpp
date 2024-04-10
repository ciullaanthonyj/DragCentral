//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "messageform.h"
#include "myvcl.h"
#include "common_stuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma link "cgrbutton"
#pragma resource "*.dfm"
Tmessage *message;
//---------------------------------------------------------------------------
__fastcall Tmessage::Tmessage(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall Tmessage::FormShow(TObject *Sender)
{
  this->Top = (Screen->Height - this->Height) / 2 - 42;
  this->Left = (Screen->Width - this->Width) / 2;
  if(mess->Caption.Pos("\r\n")==0)
    mess->Caption = "\r\n"+mess->Caption;
  btnyes->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall Tmessage::btnyesClick(TObject *Sender)
{
  ModalResult = mrOk;        
}
//---------------------------------------------------------------------------

void __fastcall Tmessage::Button2Click(TObject *Sender)
{
  ModalResult = mrCancel;        
}
//---------------------------------------------------------------------------

void __fastcall Tmessage::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key==VK_ESCAPE) {
    ModalResult = mrCancel;
  }

}
//---------------------------------------------------------------------------

