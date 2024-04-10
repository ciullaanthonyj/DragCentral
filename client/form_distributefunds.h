//---------------------------------------------------------------------------

#ifndef form_distributefundsH
#define form_distributefundsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "cgrbutton.h"
//---------------------------------------------------------------------------
class Tdistributefunds : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *members;
        TGrButton *btntransfer;
        TGrButton *Button2;
        TEdit *edAmount;
        TLabel *Label1;
        TLabel *Label2;
        TImage *Image3;
        void __fastcall btntransferClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall edAmountKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall membersDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall Tdistributefunds(TComponent* Owner);
        int TeamAccount;
};
//---------------------------------------------------------------------------
extern PACKAGE Tdistributefunds *distributefunds;
//---------------------------------------------------------------------------
#endif
