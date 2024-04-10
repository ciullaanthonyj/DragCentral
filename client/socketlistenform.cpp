//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "socketlistenform.h"
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
void __fastcall TForm1::ServerSocket1Accept(TObject *Sender,
      TCustomWinSocket *Socket)
{
  Memo1->Lines->Add("Connect to: " + Socket->RemoteAddress);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ServerSocket1ClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
  Memo1->Lines->Add(Socket->ReceiveText());          
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button1Click(TObject *Sender)
{
ServerSocket1->Socket->Connections[0]->SendText(
         Memo2->Lines->Text);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
ServerSocket1->Socket->Connections[0]->Close();
}
//---------------------------------------------------------------------------

