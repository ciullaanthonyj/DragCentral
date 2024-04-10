//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "form_getpass.h"
#include "myvcl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma resource "*.dfm"
Tgetpass *getpass;
//---------------------------------------------------------------------------
__fastcall Tgetpass::Tgetpass(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tgetpass::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    ReleaseCapture();
    SNDMSG(Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
  }
        

}
//---------------------------------------------------------------------------
void __fastcall Tgetpass::btncreateClick(TObject *Sender)
{
  ModalResult = mrCancel;        
}
//---------------------------------------------------------------------------
void __fastcall Tgetpass::registerbackClick(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall Tgetpass::edforgotemailKeyPress(TObject *Sender, char &Key)
{
  // enter key
  if(Key==13) {
    Key=0;
    ModalResult = mrOk;
  }

  // escape key
  if(Key==0x1b) {
    Key=0;
    ModalResult = mrCancel;
  }

}
//---------------------------------------------------------------------------
void __fastcall Tgetpass::FormCreate(TObject *Sender)
{
  this->DoubleBuffered = true;
  SetRoundWindow(this,11);
}
//---------------------------------------------------------------------------

