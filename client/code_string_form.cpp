//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "code_string_form.h"
#include "cencoder.h"
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
void __fastcall TForm1::Edit1Change(TObject *Sender)
{
  Edit2->Text = encoder.encode(Edit1->Text);
  Edit3->Text = encoder.decode(Edit2->Text);
  if(Edit3->Text!=Edit1->Text)
    ShowMessage("Error in encoder/decoder");        
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit2Change(TObject *Sender)
{
  Edit3->Text = encoder.decode(Edit2->Text);
}
//---------------------------------------------------------------------------

