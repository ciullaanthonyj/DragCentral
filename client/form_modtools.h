//---------------------------------------------------------------------------

#ifndef form_modtoolsH
#define form_modtoolsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "sensor.h"
#include "sensortab.h"
#include "chatserver.h"
#include <Grids.hpp>
#include <Menus.hpp>
#include "cgrbutton.h"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class Tmodtools : public TForm
{
__published:	// IDE-managed Components
        TMemo *reason;
        TEdit *nick;
        TComboBox *duration;
        TGrButton *btnclose;
        TGrButton *btnprocess;
        TGrButton *btnlookup;
        TStringGrid *users;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TComboBox *cbaction;
        TEdit *edLength;
        TPopupMenu *PopupMenu1;
        TMenuItem *Close1;
        TImage *Image3;
	TTimer *julik_detect;
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnenterClick(TObject *Sender);
        void __fastcall Sensor1Click(TObject *Sender);
        void __fastcall durationClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnlookupClick(TObject *Sender);
        void __fastcall Close1Click(TObject *Sender);
        void __fastcall usersDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
        void __fastcall cbactionClick(TObject *Sender);
	void __fastcall julik_detectTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        cChatServer* s;
        AnsiString userid;
        int length;
        __fastcall Tmodtools(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tmodtools *modtools;
//---------------------------------------------------------------------------
#endif
