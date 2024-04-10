//---------------------------------------------------------------------------

#ifndef encoder_formH
#define encoder_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TTimer *Timer1;
        TButton *Button2;
        TRichEdit *RichEdit1;
        TButton *Button3;
        TTimer *Timer2;
        TCheckBox *CheckBox1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
