//---------------------------------------------------------------------------

#ifndef form_traceH
#define form_traceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class Tfrm_trace : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TSpeedButton *PauseButton;
	void __fastcall FormShow(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Tfrm_trace(TComponent* Owner);
	void trace(AnsiString s,bool LogForUser = true) {
        if(this)
		if(!PauseButton->Down)
		  Memo1->Lines->Add(s);
    }


};
void Trace(AnsiString s);
void Trace_Clear();
#define TRACE(p) Trace(AnsiString(#p)+AnsiString("(")+AnsiString(__FILE__)+AnsiString("):")+AnsiString(p));
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_trace *frm_trace;
//---------------------------------------------------------------------------
#endif
