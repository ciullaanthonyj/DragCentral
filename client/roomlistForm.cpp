//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "roomlistForm.h"
#include "createroomForm.h"
#include "myvcl.h"
#include "inputForm.h"
#include "messageform.h"
#include "frmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma resource "*.dfm"
Trooms *rooms;
//---------------------------------------------------------------------------
__fastcall Trooms::Trooms(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Trooms::FormShow(TObject *Sender)
{
  ids->Clear();
  cGridFiller gridline(list);
  AnsiString id;
  AnsiString room;
  AnsiString people;
  AnsiString passworded;
  cStringTable tbl(roomlist);
  while(tbl.GetLine()) {
    id = tbl++;
    room = tbl++;
	people = tbl++;
	passworded = tbl++;

	ids->Add(id);
	gridline[0] = room;
	gridline[3] = people+" racer(s)";
	gridline[2] = passworded; // PASSWORDED
	gridline[4] = encoder.encode(tbl++); //max non members allowed
	gridline[5] = encoder.encode(tbl++);//non members count
	AnsiString membersonly = tbl++;
	gridline[6] = encoder.encode(membersonly);//member flag
	if(passworded==S_1)
	  gridline[1] = "Private";
	else
	if(membersonly=="1")
      gridline[1] = "Members Only";
	else
	  gridline[1] = "Public";
	gridline.nextrow();
  }

  if(Tag==1) {
	 Application->ProcessMessages();
     btncreateClick(0);
     Tag=0;
  }

  Changed = false;
}
//---------------------------------------------------------------------------
void __fastcall Trooms::FormCreate(TObject *Sender)
{
  RemoveImageBlink(this);
  ids = new TStringList;
  SetRoundWindow(this,12);
}
//---------------------------------------------------------------------------
void __fastcall Trooms::FormDestroy(TObject *Sender)
{
  delete ids;
}
//---------------------------------------------------------------------------
void __fastcall Trooms::listDblClick(TObject *Sender)
{
  AnsiString newroom = ids->Strings[list->Row];
  roomname = list->Cells[0][list->Row];
  AnsiString pass="";
  bool Private = false;
  int limit = StrToInt(encoder.decode(list->Cells[4][list->Row]));
  int nonusers = StrToInt(encoder.decode(list->Cells[5][list->Row]));

  if(testingAutoPass==true) { // for testing
	pass = "pass";
	testingAutoPass=false;
  }


  if(!main->Server.member && encoder.decode(list->Cells[6][list->Row])=="1") {
	 message->ShowMessage("Members only room.");
	 goto quit;
  }

  if(!main->Server.member && nonusers>=limit) {
	 message->ShowMessage("Only "+IntToStr(limit)+ " non member(s) allowed in this room.");
	 goto quit;
  }


  if(list->Cells[2][list->Row]==S_1) {
	Private = true;
	pass=input->InputBox("Room is private","Enter password:","");
	if(pass=="") goto quit; //cancel pressed
  }
	Changed = (room!=newroom);
	if(Changed) {
		Screen->Cursor = crAppStart;
		Server->EnterRoom(newroom,pass);
		Screen->Cursor = crDefault;
	}
	room = newroom;
  ModalResult = mrOk;
quit:
}
//---------------------------------------------------------------------------
void __fastcall Trooms::btncloseClick(TObject *Sender)
{
  if(room=="") {
	list->Row = 0;
	listDblClick(0);
  } else {
	ModalResult = mrCancel;
  }

}
//---------------------------------------------------------------------------

void __fastcall Trooms::listKeyPress(TObject *Sender, char &Key)
{
  if(Key==13)
    list->OnDblClick(0);
}
//---------------------------------------------------------------------------


void __fastcall Trooms::btncreateClick(TObject *Sender)
{
  createroom->Top = this->Top + 40;
  createroom->Left = this->Left + 40;
  createroom->Server = this->Server;
  if(mrOk==createroom->ShowModal()) {
      this->roomlist = Server->RoomList();
      FormShow(0);
  }
}
//---------------------------------------------------------------------------



void __fastcall Trooms::autoselectTimer(TObject *Sender)
{
  autoselect->Enabled = false;
  if(this->Visible)
    listDblClick(0);
}
//---------------------------------------------------------------------------

void __fastcall Trooms::Image8MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    ReleaseCapture();
    SNDMSG(Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
  }
        
}
//---------------------------------------------------------------------------

void __fastcall Trooms::FormActivate(TObject *Sender)
{
  list->SetFocus();        
}
//---------------------------------------------------------------------------

void __fastcall Trooms::listDrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
 if(State.Contains(gdSelected)) {
	 list->Canvas->Brush->Color = 0xa8a8a8;
	 list->Canvas->FillRect(Rect);
  }
  else {
	 list->Canvas->Brush->Color = clBlack;
	 list->Canvas->FillRect(Rect);
  }

  TStringGrid* g = (TStringGrid*)Sender;
  g->Canvas->Font = g->Font;
  if(State.Contains(gdSelected))
     g->Canvas->Brush->Color = GridSelectColor;
  else
     g->Canvas->Brush->Color = g->Color;
  g->Canvas->FillRect(Rect);
/*  if(g->Cells[6][ARow]==encoder.encode("1") && ACol==0) {
	  g->Canvas->Font->Color = WebColor(main->ini->ReadString("Chat Colors","member_text","#00ff1e"));
  } else {
	  g->Canvas->Font->Color = clWhite;
  }*/

  g->Canvas->TextOut(Rect.Left+2,Rect.Top+2,g->Cells[ACol][ARow]);
        
}
//---------------------------------------------------------------------------


