//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "test_findwindow_form.h"
#include "cfindwindow.h"
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
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  if(DesktopWindows.Find("cheat","engine")) ShowMessage("got it");
}
//---------------------------------------------------------------------------
