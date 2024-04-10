//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "form_gearratios.h"
#include "myinternet.h"
#include "common_stuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgrbutton"
#pragma resource "*.dfm"
Tgearratios *gearratios;
//---------------------------------------------------------------------------
__fastcall Tgearratios::Tgearratios(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tgearratios::FormShow(TObject *Sender)
{
  static bool firsttime=true;
  if(firsttime) {
    this->Top = (Screen->Height - this->Height) / 2 - 42;
    this->Left = (Screen->Width - this->Width) / 2;
    firsttime = false;
  }
  lbed6->Visible = maxspeednum==6;
  ed6->Visible = maxspeednum==6;
  if(maxspeednum!=6)
    ed6->Text = "0.7"; //for checking valid values
}
//---------------------------------------------------------------------------
void __fastcall Tgearratios::ed1KeyPress(TObject *Sender, char &Key)
{
  if(Key=='0' ||
     Key=='1' ||
     Key=='2' ||
     Key=='3' ||
     Key=='4' ||
     Key=='5' ||
     Key=='6' ||
     Key=='7' ||
     Key=='8' ||
     Key=='9' ||
     Key=='0' ||
     Key=='.' ||
     Key==VK_ESCAPE ||
     Key==VK_TAB ||
     Key==VK_BACK ||
     Key==VK_HOME ||
     Key==VK_LEFT ||
     Key==VK_RIGHT ) {

  } else Key=0;

}
//---------------------------------------------------------------------------



void __fastcall Tgearratios::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key==VK_ESCAPE) ModalResult = mrCancel;        
}
//---------------------------------------------------------------------------


bool GoodValue(AnsiString s) {
  try {
    float f = StrToFloat(s);
    if(f<0.02 || f>10.0)
      return false;
  } catch (...) {
    return false;
  }
  return true;
}
void __fastcall Tgearratios::Button1Click(TObject *Sender)
{
  if(!GoodValue(ed1->Text)) {
    ed1->SetFocus();
    throw Exception("Bad value.");
  }
  if(!GoodValue(ed2->Text)) {
    ed2->SetFocus();
    throw Exception("Bad value.");
  }
  if(!GoodValue(ed3->Text)) {
    ed3->SetFocus();
    throw Exception("Bad value.");
  }
  if(!GoodValue(ed4->Text)) {
    ed4->SetFocus();
    throw Exception("Bad value.");
  }
  if(!GoodValue(ed5->Text)) {
    ed5->SetFocus();
    throw Exception("Bad value.");
  }
  if(!GoodValue(ed6->Text)) {
    ed6->SetFocus();
    throw Exception("Bad value.");
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall Tgearratios::Button1Enter(TObject *Sender)
{
  inc->Enabled = false;
  dec->Enabled = false;        
}
//---------------------------------------------------------------------------

void __fastcall Tgearratios::ed1Enter(TObject *Sender)
{
  inc->Enabled = true;
  dec->Enabled = true;        
}
//---------------------------------------------------------------------------

void __fastcall Tgearratios::decClick(TObject *Sender)
{
  TEdit* ed = (TEdit*)(this->ActiveControl);
  try {
     float a  = StrToFloat(ed->Text);
     if(Sender==dec)
       a -= 0.01;
     if(Sender==inc)
       a += 0.01;
     ed->Text = FloatToStrF(a,ffFixed,10,3);
  } catch (...) {};
}
//---------------------------------------------------------------------------

void __fastcall Tgearratios::Button2Click(TObject *Sender)
{
  ModalResult = mrCancel;        
}
//---------------------------------------------------------------------------



