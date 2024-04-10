//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "createroomForm.h"
#include "myvcl.h"
#include "messageform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma resource "*.dfm"
Tcreateroom *createroom;
//---------------------------------------------------------------------------
__fastcall Tcreateroom::Tcreateroom(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tcreateroom::btncloseClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall Tcreateroom::btncreateClick(TObject *Sender)
{
    try {
      Server->CreateRoom(createroom->edroom->Text,createroom->edpass->Text);
      // REFRESH ROOMS LIST
      ModalResult = mrOk;
    } catch (Exception &E) {
      message->ShowMessage(E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tcreateroom::FormActivate(TObject *Sender)
{
  edroom->Text = "";
  edpass->Text = "";
  edroom->SetFocus();          
}
//---------------------------------------------------------------------------

void __fastcall Tcreateroom::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    ReleaseCapture();
    SNDMSG(Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
  }
       
}
//---------------------------------------------------------------------------

void __fastcall Tcreateroom::FormCreate(TObject *Sender)
{
  RemoveImageBlink(this);
  SetRoundWindow(this,11);
}
//---------------------------------------------------------------------------

void __fastcall Tcreateroom::edpassKeyPress(TObject *Sender, char &Key)
{
  if(Key==13)
    btncreate->OnClick(0);        
}
//---------------------------------------------------------------------------

