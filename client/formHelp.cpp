//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "formHelp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
THelp *Help;
//---------------------------------------------------------------------------
__fastcall THelp::THelp(TComponent* Owner)
	: TForm(Owner)         
{
}
//---------------------------------------------------------------------------


void __fastcall THelp::FormKeyPress(TObject *Sender, char &Key)
{
  if(Key==VK_ESCAPE) Close();  	
	
}
//---------------------------------------------------------------------------

