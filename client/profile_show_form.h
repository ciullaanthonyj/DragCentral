//---------------------------------------------------------------------------

#ifndef profile_show_formH
#define profile_show_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMemo *memo;
        TOpenDialog *od;
        TTimer *tm;
        TPanel *Panel1;
        TEdit *filename;
        TButton *open;
        TButton *Button1;
        TTimer *autoexec;
        void __fastcall openClick(TObject *Sender);
        void __fastcall tmTimer(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall autoexecTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
