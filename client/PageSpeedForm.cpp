//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PageSpeedForm.h"
#include "myinternet.h"
#include "myprofiler.h"
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
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  cServer Server;
  Server.ServerName = host->Text;
  Server.Dir = dir->Text;

  Server.Connect();
  cTimer Query;
  Query.Start();
  Memo2->Lines->Clear();
  Memo2->Lines->Add(Server.Query(script->Text,params->Text));
  Query.Stop();

  Memo1->Lines->Add(
  host->Text+dir->Text+script->Text+params->Text+" "+IntToStr(Query.Milliseconds())
  );
  Server.Disconnect();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
  cServer Server;
  Server.ServerName = host2->Text;
  Server.Dir = dir2->Text;

  Server.Connect();
  Memo2->Lines->Clear();
  cTimer Query;
  Query.Start();
  Memo2->Lines->Add(Server.Query(script2->Text,params2->Text));
  Query.Stop();

  Memo1->Lines->Add(
  host2->Text+dir2->Text+script2->Text+params2->Text+" "+IntToStr(Query.Milliseconds())
  );
  Server.Disconnect();

}
//---------------------------------------------------------------------------
