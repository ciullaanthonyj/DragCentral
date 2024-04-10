//---------------------------------------------------------------------------

#ifndef form_gearratiosH
#define form_gearratiosH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "cgrbutton.h"
//---------------------------------------------------------------------------
class Tgearratios : public TForm
{
__published:	// IDE-managed Components
        TGrButton *Button1;
        TGrButton *Button2;
        TLabel *Label1;
        TEdit *ed1;
        TEdit *ed2;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *ed3;
        TLabel *Label4;
        TEdit *ed4;
        TLabel *Label5;
        TEdit *ed5;
        TEdit *ed6;
        TLabel *lbed6;
        TSpeedButton *dec;
        TSpeedButton *inc;
        TImage *Image3;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ed1KeyPress(TObject *Sender, char &Key);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button1Enter(TObject *Sender);
        void __fastcall ed1Enter(TObject *Sender);
        void __fastcall decClick(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall Tgearratios(TComponent* Owner);
        int maxspeednum;
};
//---------------------------------------------------------------------------
extern PACKAGE Tgearratios *gearratios;
//---------------------------------------------------------------------------
#endif
