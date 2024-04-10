//---------------------------------------------------------------------------

#include <vcl.h>
#include <algorithm.h>
#pragma hdrstop

#include "grcut_form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfm *fm;
//---------------------------------------------------------------------------
__fastcall Tfm::Tfm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tfm::FormCreate(TObject *Sender)
{
  ini = new TIniFile(ChangeFileExt( Application->ExeName, ".ini" ));
  od->InitialDir = ini->ReadString("Settings","LastDir",".");
  od->FileName = ini->ReadString("Settings","LastFile","");
  openClick(0);
}
//---------------------------------------------------------------------------
void __fastcall Tfm::FormDestroy(TObject *Sender)
{
  ini->WriteString("Settings","LastFile",filename->Text);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall Tfm::openClick(TObject *Sender)
{
  if(Sender==0 || od->Execute())
  if(FileExists(od->FileName))
  {
     im->Picture->LoadFromFile(od->FileName);
     ini->WriteString("Settings","LastFile",od->FileName);
     filename->Text = od->FileName;
     ini->WriteString("Settings","LastDir",ExtractFileDir(od->FileName));
  }
}

TRect selection;
bool selected = false;
bool selecting = false;
//---------------------------------------------------------------------------
void __fastcall Tfm::imMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if(Shift.Contains(ssCtrl)) {
    selection.Left = X;
    selection.top = Y;
    CopySelection();
  } else
  if(Shift.Contains(ssAlt)) {
    selection.right = X;
    selection.Bottom = Y;
    selected = true;
    CopySelection();
  } else {
    selecting = true;
    selection.Left = X;
    selection.top = Y;
  }
}
//---------------------------------------------------------------------------
  void Tfm::CopySelection() {
    TRect source;
    source.left = min(selection.right,selection.left);
    source.right = max(selection.right,selection.left);
    source.top = min(selection.top,selection.Bottom);
    source.bottom = max(selection.top,selection.Bottom);

    sel->Picture->Bitmap->Width = source.right-source.left;
    sel->Picture->Bitmap->Height = source.Bottom - source.top;
    sel->Picture->Bitmap->Canvas->CopyRect(
      TRect(0,0,sel->Picture->Width,sel->Picture->Height),
      im->Picture->Bitmap->Canvas,
     source);
  }

void __fastcall Tfm::imMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if(Shift.Contains(ssCtrl) || Shift.Contains(ssAlt)) return;
  selected = true;
  selection.right = X;
  selection.Bottom = Y;
  selecting = false;
  CopySelection();

}
//---------------------------------------------------------------------------
void __fastcall Tfm::saveClick(TObject *Sender)
{
  sd->InitialDir = ini->ReadString("Settings","LastSaveDir",".");
  if(sd->Execute()) {
    ini->WriteString("Settings","LastSaveDir",
      ExtractFileDir(sd->FileName));
    sel->Picture->SaveToFile(sd->FileName);
  }
}
//---------------------------------------------------------------------------
void __fastcall Tfm::imMouseMove(TObject *Sender, TShiftState Shift, int X,
      int Y)
{
  if(Shift.Contains(ssCtrl)) {
     selection.left = X;
     selection.top = Y;
     CopySelection();
  } else

  if(Shift.Contains(ssAlt)) {
     selection.right = X;
     selection.bottom = Y;
     CopySelection();
  } else

  if(selecting) {
     selection.right = X;
     selection.bottom = Y;
     CopySelection();
  }



}
//---------------------------------------------------------------------------
