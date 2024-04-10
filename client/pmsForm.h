//---------------------------------------------------------------------------

#ifndef pmsFormH
#define pmsFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "chatserver.h"
#include <ComCtrls.hpp>
#include "sensor.h"
#include "sensortab.h"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
#include "cencoder.h"
class Tpms : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TMemo *mess;
        TRichEdit *chat;
        TLabel *lbwithnick;
        TSensor *btnclose;
        TImage *Image22;
	TSensor *btnreport;
        TImage *Image2;
        TSensorTab *SensorTab1;
        TSensorTab *SensorTab2;
        TSensor *smokem;
        TSensorTab *SensorTab3;
        TImage *Image3;
        TImage *Image4;
        TSensor *block;
        TSensorTab *SensorTab4;
        void __fastcall btncloseClick(TObject *Sender);
        void __fastcall btnsendClick(TObject *Sender);
        void __fastcall messKeyPress(TObject *Sender, char &Key);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall smokemClick(TObject *Sender);
        void __fastcall blockClick(TObject *Sender);
	void __fastcall btnreportClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall Tpms(TComponent* Owner);
        cChatServer* Server;
        SecString withnick;
        TNotifyEvent PMSActivate;
        TNotifyEvent OnSmokem;
        TNotifyEvent OnBlock;
};
//---------------------------------------------------------------------------
extern PACKAGE Tpms *pms;
//---------------------------------------------------------------------------
#endif
