//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "myinternettestform.h"
#include "myinternet.h"
#include "server.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
cServer Server;
TServerThread* thread;
void __fastcall TForm1::Button1Click(TObject *Sender)
{
//  Memo1->Lines->Add(Server.Query("maxmess.php",""));
//  Server.Disconnect();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  Timer1->Enabled =false;
  Server.ServerName = "localhost";
  Server.Dir = "/cars/";
  Server.Connect();
  thread = new TServerThread(true);
  thread->Server = &Server;
  thread->Memo = Memo1;
  thread->Resume();
//  refresh->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ApplicationEvents1Exception(TObject *Sender,
      Exception *E)
{
  ShowMessage(E->Message);
}
//---------------------------------------------------------------------------
