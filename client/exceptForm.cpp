//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "exceptForm.h"
#include "myvcl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma resource "*.dfm"
Texcept *except;
//---------------------------------------------------------------------------
__fastcall Texcept::Texcept(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Texcept::Image2Click(TObject *Sender)
{
  ModalResult = mrOk;        
}
//---------------------------------------------------------------------------


void __fastcall Texcept::FormCreate(TObject *Sender)
{
  RemoveImageBlink(this);
}
//---------------------------------------------------------------------------

void __fastcall Texcept::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    ReleaseCapture();
    SNDMSG(Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
  }
        
}
//---------------------------------------------------------------------------

void __fastcall Texcept::FormActivate(TObject *Sender)
{
  this->memo->SetFocus();        
}
//---------------------------------------------------------------------------


void __fastcall Texcept::Sensor1Click(TObject *Sender)
{
//  ShellExecute(NULL,"open",WebSite.c_str(),NULL,NULL,SW_NORMAL);
}
//---------------------------------------------------------------------------


void __fastcall Texcept::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key==0x1b)
    ModalResult = mrCancel;        
}
//---------------------------------------------------------------------------



