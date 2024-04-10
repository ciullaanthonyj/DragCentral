//---------------------------------------------------------------------------

#ifndef test_game_formH
#define test_game_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "race_car_engine.h"
#include <ExtCtrls.hpp>
#include "CGAUGES.h"
#include <jpeg.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <Menus.hpp>
#include "ShockwaveFlashObjects_OCX.h"
#include <OleCtrls.hpp>
#include <AppEvnts.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TTimer *Timer1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TPaintBox *pb;
        TButton *Button1;
        TPopupMenu *PopupMenu1;
        TMenuItem *SpeedUp1;
        TMenuItem *Down;
        TApplicationEvents *ApplicationEvents1;
        TButton *Start;
        TButton *Button2;
        TButton *Button3;
        TMemo *Memo1;
        TMainMenu *MainMenu1;
        TMenuItem *Test1;
        TMenuItem *Bracketrace1;
        TMenuItem *Youlose1;
        TMenuItem *Youwinsimulation1;
        TMenuItem *Youlosesimulation1;
        TMenuItem *WithracerYouwinsimulation1;
        TShockwaveFlash *flupview;
        TShockwaveFlash *flstaging;
        TShockwaveFlash *flroad;
        TShockwaveFlash *flcarpanel;
        TTabSheet *TabSheet3;
        TMemo *Memo2;
        TButton *Button4;
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall SpeedUp1Click(TObject *Sender);
        void __fastcall DownClick(TObject *Sender);
        void __fastcall ApplicationEvents1Message(tagMSG &Msg,
          bool &Handled);
        void __fastcall StartClick(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Bracketrace1Click(TObject *Sender);
        void __fastcall Youwinsimulation1Click(TObject *Sender);
        void __fastcall BreakOutCLick(TObject *Sender);
        void __fastcall Youlosesimulation1Click(TObject *Sender);
        void __fastcall WithracerYouwinsimulation1Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        void DetectWin();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
