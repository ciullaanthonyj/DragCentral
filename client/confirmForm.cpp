//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "confirmForm.h"
#include "myvcl.h"
#include "common_stuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma link "cgrbutton"
#pragma resource "*.dfm"
Trequest *request;
//---------------------------------------------------------------------------
bool Trequest::confirm(AnsiString mess) {

  this->mess->Caption = mess;
  TModalResult res = ShowModal();
  //update screen
  Application->ProcessMessages();
  return (res==mrOk);


}
__fastcall Trequest::Trequest(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall Trequest::FormShow(TObject *Sender)
{
  this->Top = (Screen->Height - this->Height) / 2 - 42;
  this->Left = (Screen->Width - this->Width) / 2;
  if(mess->Caption.Pos("\r\n")==0)
    mess->Caption = "\r\n"+mess->Caption;
  btnyes->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall Trequest::btnyesClick(TObject *Sender)
{
  ModalResult = mrOk;        
}
//---------------------------------------------------------------------------

void __fastcall Trequest::Button2Click(TObject *Sender)
{
  ModalResult = mrCancel;        
}
//---------------------------------------------------------------------------

void __fastcall Trequest::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key==VK_ESCAPE) {
    ModalResult = mrCancel;
  }

}
//---------------------------------------------------------------------------

