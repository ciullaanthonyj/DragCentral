//---------------------------------------------------------------------------

#ifndef PageSpeedFormH
#define PageSpeedFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TEdit *script;
        TButton *Button1;
        TMemo *Memo1;
        TEdit *params;
        TEdit *dir;
        TEdit *host;
        TMemo *Memo2;
        TEdit *script2;
        TButton *Button2;
        TEdit *params2;
        TEdit *dir2;
        TEdit *host2;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
