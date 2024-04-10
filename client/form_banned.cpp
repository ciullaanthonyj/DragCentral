//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "form_banned.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma resource "*.dfm"
Tbanned *banned;
//---------------------------------------------------------------------------
__fastcall Tbanned::Tbanned(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tbanned::btncloseClick(TObject *Sender)
{
  ModalResult = mrOk;        
}
//---------------------------------------------------------------------------
void __fastcall Tbanned::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    ReleaseCapture();
    SNDMSG(Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
  }
        
        
}
//---------------------------------------------------------------------------
void __fastcall Tbanned::memoKeyPress(TObject *Sender, char &Key)
{
  if(Key==13) {
    Key=0;
    ModalResult = mrOk;
  }

}
//---------------------------------------------------------------------------
