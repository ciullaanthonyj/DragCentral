//---------------------------------------------------------------------------

#ifndef dc_offline_formH
#define dc_offline_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ShockwaveFlashObjects_OCX.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TLabel *lbChallengeInfo;
        TShockwaveFlash *flcarpanel;
        TShockwaveFlash *flupview;
        TShockwaveFlash *flstaging;
        TButton *btnSkipStaging;
        TShockwaveFlash *flroad;
        TTimer *autoexec;
        TTimer *gametimer;
        TButton *Restart;
        TImage *Image3;
        TLabel *lbenginedamage;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall autoexecTimer(TObject *Sender);
        void __fastcall gametimerTimer(TObject *Sender);
        void __fastcall btnSkipStagingClick(TObject *Sender);
        void __fastcall RestartClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
