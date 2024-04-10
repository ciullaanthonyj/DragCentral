//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "cgrbutton_test_form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgrbutton"
#pragma link "sensor"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GrButton1Click(TObject *Sender)
{
  ShowMessage("Click");        
}
//---------------------------------------------------------------------------

TGrButton* grbut;
TBevel* bevel;
void __fastcall TForm1::FormCreate(TObject *Sender)
{
  bevel = new TBevel(this);
  bevel->Parent = (TWinControl*)(this);
  bevel->Left = 100;
  bevel->Top = 230;
  bevel->Shape = bsFrame;

  
  grbut = new TGrButton(this);
  grbut->Parent = this;
  grbut->Caption = "corner";
//  grbut->Visible = false;
  grbut->Enabled = false;        
}
//---------------------------------------------------------------------------


void __fastcall TForm1::GrButton2Click(TObject *Sender)
{
  ShowMessage("Click");        
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Label1MouseLeave(TObject *Sender)
{
  this->Caption = "leave";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label1MouseEnter(TObject *Sender)
{
  this->Caption = "enter";
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  Timer1->Enabled = false;
  GrButton1->Enabled = false;        
}
//---------------------------------------------------------------------------


void __fastcall TForm1::GrButton4Enter(TObject *Sender)
{
  ShowMessage("here");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label1Click(TObject *Sender)
{
  ShowMessage("test");        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  ShowMessage("Test");        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GrButton3Click(TObject *Sender)
{
  ShowMessage("grbutton");        
}
//---------------------------------------------------------------------------

