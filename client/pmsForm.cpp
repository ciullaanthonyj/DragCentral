//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "pmsForm.h"
#include "myvcl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma resource "*.dfm"
Tpms *pms;
//---------------------------------------------------------------------------
__fastcall Tpms::Tpms(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tpms::btncloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall Tpms::btnsendClick(TObject *Sender)
{
  AnsiString msg = mess->Lines->Text;
  Server->SendMessage(msg,withnick);
  mess->Lines->Clear();
  return;
}
//---------------------------------------------------------------------------
void __fastcall Tpms::messKeyPress(TObject *Sender, char &Key)
{
  if(Key==13) {
    btnsendClick(0);
    Key=0;
  }
}
//---------------------------------------------------------------------------

void __fastcall Tpms::FormActivate(TObject *Sender)
{
  if(PMSActivate!=NULL)
    PMSActivate(this);
  this->ActiveControl = mess;
}
//---------------------------------------------------------------------------


void __fastcall Tpms::Image1MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    ReleaseCapture();
    SNDMSG(Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
  }
        
}
//---------------------------------------------------------------------------

void __fastcall Tpms::FormCreate(TObject *Sender)
{
  RemoveImageBlink(this);
  SetRoundWindow(this,11);

}
//---------------------------------------------------------------------------

void __fastcall Tpms::smokemClick(TObject *Sender)
{
  if(this->OnSmokem!=NULL)
    this->OnSmokem(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tpms::blockClick(TObject *Sender)
{
  if(this->OnBlock!=NULL) {
    OnBlock(this);
    Close();
  }
}
//---------------------------------------------------------------------------

void __fastcall Tpms::btnreportClick(TObject *Sender)
{
  ShowMessage("Not working yet.");	
}
//---------------------------------------------------------------------------

