//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "chatserver_test_form.h"
#include "chatserver.h"
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
void mess(AnsiString msg) {
  Form1->Memo1->Lines->Add(msg);

}

void add(AnsiString msg) {
  msg = Form1->Memo1->Lines->Strings[Form1->Memo1->Lines->Count-1] + msg;
  Form1->Memo1->Lines->Strings[Form1->Memo1->Lines->Count-1] = msg;

}
#define log(cmd) mess(#cmd); cmd; add(" !");
cChatServer* server1;
cChatServer* server2;

void __fastcall TForm1::autoexecTimer(TObject *Sender)
{
  autoexec->Enabled = false;

  ban->OnClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::test1Click(TObject *Sender)
{

  log( server1->Login("nazar","nazar") );
  log( AnsiString RoomList = server1->RoomList() );
  mess( RoomList );
  log( server1->EnterRoom("1") );
  log( server1->SendMessage("message to room 1","") );

  server2 = new cChatServer;
  server2->Dir = server1->Dir;
  server2->ServerName = server1->ServerName;

  server2->Connect();
  log( server2->Login("another","another") );
  log( server2->EnterRoom("1") );
  log( server2->SendMessage("message to room 1","") );
  log( server2->SendMessage("message to nazar","nazar") );
  ShowMessage("message to room 1 from another");

  server2->QuitChat();
  server2->Disconnect();
  delete server2;


  ShowMessage("Wait for message");
  log( server1->EnterRoom("2") );
  log( server1->SendMessage("message to room 2","") );
  ShowMessage("Wait for message");
  log( server1->QuitChat() );



  mess( "Test Done" );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::eventRoomPeopleUpdate(TObject *Sender)
{
  Memo2->Lines->Clear();
  Memo2->Lines->Add(server1->chat->PeopleInRoom);
}
//---------------------------------------------------------------------------
bool Recieved = false;
void __fastcall TForm1::eventChatMessageClick(TObject *Sender)
{
  sRecieveMessage* msg = (sRecieveMessage*) (Sender);
  Memo3->Lines->Add(msg->nick+":"+msg->text);
  Recieved = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::changeroomClick(TObject *Sender)
{
  Memo2->Lines->Clear();
  Memo3->Lines->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::test2Click(TObject *Sender)
{
  Memo1->Clear();

  log( server1->Connect() );
  log( server1->Login("nazar","nazar") );
  log( AnsiString RoomList = server1->RoomList() );
  mess( RoomList );
  Recieved = false;
  log( server1->EnterRoom("1") );
  while(!Recieved) Application->ProcessMessages();

  Sleep(2000);
  Recieved = false;
  log( server1->SendMessage("message to room 1 - 1","") );
  while(!Recieved) Application->ProcessMessages();

  Sleep(2000);
  Recieved = false;
  log( server1->SendMessage("message to room 1 - 2","") );
  while(!Recieved) Application->ProcessMessages();

  Sleep(2000);
  Recieved = false;
  log( server1->SendMessage("message to room 1 - 3","") );
  while(!Recieved) Application->ProcessMessages();

  Sleep(2000);

  log( server1->QuitChat() );
  log( server1->Disconnect() );



  mess( "Test Done" );

        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::regClick(TObject *Sender)
{
  log( server1->Connect() );

//  try {
//    log( server1->CreateAccount("testnew","testnew","testnew"))
//  } catch (Exception &E) {
//    ShowMessage(E.Message);
//  }
  Sleep(2000);
  log(  server1->Login("testnew","testnew") );
  Sleep(2000);

  Recieved = false;
  log( server1->EnterRoom("1") );
  while(!Recieved) Application->ProcessMessages();

  Sleep(2000);
//  Recieved = false;
//  log( server1->SendMessage("message to room 1 - 1","") );
//  while(!Recieved) Application->ProcessMessages();

//  log( server1->Kick("testnew","no reason") );
//  Sleep(2000);

//  log( server1->DeleteAccount("testnew") );
//  try {
//    log( server1->Login("testnew","testnew") );
//  } catch (Exception &E) {
//    mess(E.Message);
//  }

  log( server1->QuitChat() );
//  log( server1->Disconnect() );
//  log( server1->Connect() );
//  log( server1->Login("testnew","testnew") );
  Recieved = false;
  log( server1->EnterRoom("1") );
  while(!Recieved) Application->ProcessMessages();

  log( server1->QuitChat() );
  log( server1->Disconnect() );



  mess( "Test Done" );




}
//---------------------------------------------------------------------------

void __fastcall TForm1::banClick(TObject *Sender)
{
  log( server1->Connect() );

  log( server1->Unban("borntobedead") );
  
  try {
    log( server1->CreateAccount("borntobedead","borntobedead","testnew@nowhere.com"))
  } catch (Exception &E) {
    ShowMessage(E.Message);
  }
  Sleep(2000);
  log(  server1->Login("borntobedead","borntobedead") );
  Sleep(2000);

  Recieved = false;
  log( server1->EnterRoom("1") );
  while(!Recieved) Application->ProcessMessages();

  Sleep(2000);
  Recieved = false;
  log( server1->SendMessage("message to room 1 - 1","") );
  while(!Recieved) Application->ProcessMessages();

  log( server1->Kick("borntobedead","no reason") );
  Sleep(2000);

  log( server1->Ban("borntobedead") );
  Sleep(2000);

  try {
    log( server1->Login("borntobedead","borntobedead") );
  } catch (Exception &E) {
    ShowMessage(E.Message);
  }

  log( server1->QuitChat() );
  log( server1->Disconnect() );



  mess( "Test Done" );




        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{

  log( server1->Connect() );
  try {
    log( server1->CreateAccount(Edit1->Text,Edit2->Text,Edit3->Text))
  } catch (Exception &E) {
    ShowMessage(E.Message);
  }
  Sleep(2000);

  log( server1->QuitChat() );
  log( server1->Disconnect() );



  mess( "Test Done" );






}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  server1 = new cChatServer;
  server1->Dir = "/cars/";
  server1->ServerName = "localhost";

  log( server1->Connect() );

  server1->chat->ShowQuitEnter = true;
  server1->chat->ShowMessageTime = true;
  server1->chat->OnRoomPeopleUpdate = eventRoomPeopleUpdate;
  server1->chat->OnRecieveMessage = eventChatMessageClick;
  server1->chat->OnChangeRoom = changeroomClick;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
  log( delete server1 );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
  log( server1->Connect() );

  log( server1->SendPass(Edit3->Text) );
  log( server1->Disconnect() );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
  server1->Connect();
  server1->Disconnect();
  server1->Connect();

  try {
    log( server1->CreateAccount("testnew","testnew","testnew"))
  } catch (Exception &E) {
    ShowMessage(E.Message);
  }
  Sleep(2000);
  log(  server1->Login("testnew","testnew") );
  Sleep(2000);

  Recieved = false;
  log( server1->EnterRoom("1") );
  while(!Recieved) Application->ProcessMessages();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
  log( server1->Connect());
  log( server1->Login("nazar","nazar") );
  log( server1->EnterRoom("1") );
  Sleep(3000);
  Application->ProcessMessages();
  log( server1->QuitChat() );
  log( server1->EnterRoom("1") );
  log( server1->QuitChat() );
  log( server1->EnterRoom("1") );
  log( server1->QuitChat() );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
  log( server1->chat->Resume() );
  log( server1->chat->Terminate() );
  log( server1->chat->WaitFor() );
  log( server1->chat->Resume() );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Logincreateprivateroomlogout1Click(TObject *Sender)
{
  server1->Connect();

  Sleep(2000);
  log(  server1->Login("builder","builder") );
  Sleep(2000);

  Recieved = false;
  log( server1->EnterRoom("1") );
  try {
    log( server1->CreateRoom("test","pass") );
  } catch (Exception &E) {
    ShowMessage(E.Message);
  }

  while(!Recieved) Application->ProcessMessages();

  log ( AnsiString roomlist = server1->RoomList() );
  mess( roomlist );

  AnsiString id;
  cStringTable tbl(roomlist);
  while(tbl.GetLine()) {
    id = tbl++;
    AnsiString name = tbl++;
    if(name=="test") break;
  }
  try {
    log( server1->EnterRoom(id,"wrongpass") );
  } catch (Exception &E) {
    ShowMessage(E.Message);
  }
  log( server1->EnterRoom(id,"pass") );
  log ( roomlist = server1->RoomList() );
  mess( roomlist );


  log( server1->Disconnect() );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Enterquit1Click(TObject *Sender)
{
  server1->chat->SleepInterval = 3000;
  server1->Connect();
  log(  server1->Login("member01","member01") );
  Sleep(2000);
  enterquit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::enterquitTimer(TObject *Sender)
{
  if (enterquit->Tag == 0) {
     log ( server1->EnterRoom(1); )
     log( server1->SendMessage("hello","") );
     Sleep(3000);
  }
  if (enterquit->Tag == 1) {
     log ( server1->EnterRoom(2); )
     Sleep(3000);
  }
  enterquit->Tag = 1 - enterquit->Tag;

}
//---------------------------------------------------------------------------

