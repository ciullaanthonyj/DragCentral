//---------------------------------------------------------------------------

#ifndef test_computer_racer_formH
#define test_computer_racer_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *rt;
        TLabel *Label2;
        TEdit *mt;
        TButton *go;
        TTimer *Timer1;
        TLabel *Label3;
        TEdit *carpos;
        TMemo *Memo1;
        void __fastcall goClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall mtKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
