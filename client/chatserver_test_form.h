//---------------------------------------------------------------------------

#ifndef chatserver_test_formH
#define chatserver_test_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TTimer *autoexec;
        TButton *Button1;
        TMemo *Memo2;
        TButton *test1;
        TButton *eventChatMessage;
        TMemo *Memo3;
        TButton *changeroom;
        TButton *test2;
        TButton *reg;
        TButton *ban;
        TButton *Button2;
        TButton *Button3;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TButton *Button4;
        TButton *Button5;
        TButton *Button6;
        TButton *Button7;
        TMainMenu *MainMenu1;
        TMenuItem *Test3;
        TMenuItem *Logincreateprivateroomlogout1;
        TMenuItem *Enterquit1;
        TTimer *enterquit;
        void __fastcall autoexecTimer(TObject *Sender);
        void __fastcall eventRoomPeopleUpdate(TObject *Sender);
        void __fastcall eventChatMessageClick(TObject *Sender);
        void __fastcall changeroomClick(TObject *Sender);
        void __fastcall test2Click(TObject *Sender);
        void __fastcall test1Click(TObject *Sender);
        void __fastcall regClick(TObject *Sender);
        void __fastcall banClick(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Logincreateprivateroomlogout1Click(
          TObject *Sender);
        void __fastcall Enterquit1Click(TObject *Sender);
        void __fastcall enterquitTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
