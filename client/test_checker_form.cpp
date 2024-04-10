//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "test_checker_form.h"
#include "cchecker.h"
#include <inifiles.hpp>
#include <stdio.h>
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
void __fastcall TForm1::OpenClick(TObject *Sender)
{
  if(od->Execute()) {
    Edit1->Text = IntToStr(checker.CheckFile(od->FileName));
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  Timer1->Enabled = false;
  SetCurrentDir(ExtractFileDir(Application->ExeName));
  if(ParamStr(1)!="") {      
    TIniFile* ini;
    int i = checker.CheckFile(ParamStr(1));
    Edit1->Text = IntToStr(i);

    if(ParamStr(2)!="") {
      ini = new TIniFile(ParamStr(2));
      ini->WriteInteger(ParamStr(3),ParamStr(4),i);
      delete ini;
      Close();
    }
  }
}
//---------------------------------------------------------------------------
