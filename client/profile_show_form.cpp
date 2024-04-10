//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "profile_show_form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::openClick(TObject *Sender)
{
  if(Sender==0 || od->Execute()) {
    memo->Lines->LoadFromFile(od->FileName);
    filename->Text = od->FileName;
    tm->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::tmTimer(TObject *Sender)
{
  if(!memo->Focused())
  if(this->ActiveControl!=memo)
  memo->Lines->LoadFromFile(filename->Text);        
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  memo->Lines->SaveToFile(filename->Text);        
}
//---------------------------------------------------------------------------
void __fastcall TForm1::autoexecTimer(TObject *Sender)
{
  autoexec->Enabled = false;
  od->FileName = ExtractFileDir(Application->ExeName) + "\\profile.txt";
  openClick(0);        
}
//---------------------------------------------------------------------------

