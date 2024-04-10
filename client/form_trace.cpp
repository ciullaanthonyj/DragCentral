//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "form_trace.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_trace *frm_trace;
bool command(AnsiString param) {

	for(int i=1;i<=ParamCount();i++) {
		if(ParamStr(i).LowerCase()==param.LowerCase())
		  return true;
	}
	return false;
}
//---------------------------------------------------------------------------
__fastcall Tfrm_trace::Tfrm_trace(TComponent* Owner)
	: TForm(Owner)
{
  if(command("debug")) this->Visible = true;
  if(command("debugmax")) {
	this->Visible = true;
    WindowState = wsMaximized;
  }
}

void Trace(AnsiString s) {
	static int count = 0;
	count++;
	if(count>40) {
	  count = 0;
	  Trace_Clear();
	}
	frm_trace->trace(s);
}

void Trace_Clear() {
    frm_trace->Memo1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_trace::FormShow(TObject *Sender)
{
  this->Left = Screen->Width/2;
}

//---------------------------------------------------------------------------





void __fastcall Tfrm_trace::FormCreate(TObject *Sender)
{
  ;        
}
//---------------------------------------------------------------------------

