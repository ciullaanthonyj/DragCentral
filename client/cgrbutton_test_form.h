//---------------------------------------------------------------------------

#ifndef cgrbutton_test_formH
#define cgrbutton_test_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cgrbutton.h"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <jpeg.hpp>
#include "sensor.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TImage *_ButtonsBack;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTimer *Timer1;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TPanel *Panel1;
        TEdit *Edit1;
        TGrButton *GrButton1;
        TGrButton *GrButton2;
        TGrButton *GrButton3;
        TGrButton *GrButton4;
        TLabel *Label1;
        TButton *Button1;
        void __fastcall GrButton1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall GrButton2Click(TObject *Sender);
        void __fastcall Label1MouseLeave(TObject *Sender);
        void __fastcall Label1MouseEnter(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall GrButton4Enter(TObject *Sender);
        void __fastcall Label1Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall GrButton3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
