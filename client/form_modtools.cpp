//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "form_modtools.h"
#include "myvcl.h"
#include "chatserver.h"
#include "messageform.h"
#include "common_stuff.h"
#include "cfindwindow.h"
#include "cencoder.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sensor"
#pragma link "sensortab"
#pragma link "cgrbutton"
#pragma resource "*.dfm"
Tmodtools *modtools;
//---------------------------------------------------------------------------
__fastcall Tmodtools::Tmodtools(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tmodtools::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

  if (Button == mbLeft)
    MoveForm(this);

}
//---------------------------------------------------------------------------
void __fastcall Tmodtools::FormCreate(TObject *Sender)
{
  RemoveImageBlink(this);
  SetRoundWindow(this,12);
}
//---------------------------------------------------------------------------
void __fastcall Tmodtools::btnenterClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall Tmodtools::Sensor1Click(TObject *Sender)
{
  if(reason->Text.Trim().Length()==0) {
    message->ShowMessage(encoder.decode("BB0318EB869D3D0C7376ED86E3F355510FD6") /*"Enter reason text."*/);
    return;
  }
  if(cbaction->ItemIndex!=0 && duration->Visible && StrToIntDef(edLength->Text,0)==0) {
    message->ShowMessage(encoder.decode("ADD40157173273B82D6986984A656F9744") /*"Specify duration."*/);
    return;
  }

  length = StrToIntDef(edLength->Text,0);
  AnsiString userid = GridCell(users,0);
  switch(cbaction->ItemIndex) {
    case 0:
      s->Kick(userid,reason->Text);
      break;
    case 1:
      s->ChatBan(userid,reason->Text,length);
      break;
    case 2:
      s->GameBan(userid,reason->Text,length);
      break;
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall Tmodtools::durationClick(TObject *Sender)
{
  AnsiString d = duration->Items->Strings[duration->ItemIndex];
  if(d==encoder.decode("CFD9BD1528AABA17") /*"1 minute"*/)
     length = 60;
  if(d==encoder.decode("CFCC8858CD9E924BA512") /*"15 minutes"*/)
	 length = 60*15;
  if(d==encoder.decode("CFD9B8364195") /*"1 hour"*/)
     length = 60*60;
  if(d==encoder.decode("CD98DFA10868EC") /*"3 hours"*/)
     length = 60*60*3;
  if(d==encoder.decode("CFCB2758F4CFA45C") /*"12 hours"*/)
     length = 60*60*12;
  if(d==encoder.decode("CFD9B4B69E") /*"1 day"*/)
     length = 60*60*24;
  if(d==encoder.decode("CD98D3276BB4") /*"3 days"*/)
     length = 60*60*24*3;
  if(d==encoder.decode("CFD9A7CB6AEE") /*"1 week"*/)
     length = 60*60*24*7;
  edLength->Text = IntToStr(length);
}
//---------------------------------------------------------------------------

void __fastcall Tmodtools::FormShow(TObject *Sender)
{
  duration->ItemIndex = 0;
  length = 0;
  edLength->Text = "";
  if(nick->Text!="")
    btnlookup->OnClick(0);
  nick->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall Tmodtools::btnlookupClick(TObject *Sender)
{
  AnsiString res =
  s->Query(encoder.decode("8B3D3E08EA518598F1AE5DD11E") /*"user_list.php"*/,
    encoder.decode("9FB8D1C400CD41FC5E5C96C76F6F02629FE790E4") /*"act=forban&nicklike="*/+nick->Text);

      cStringTable tbl(res);
      cGridFiller gridline(users);
      while(tbl.GetLine()) {
        gridline[0] = tbl++;
		gridline[1] = tbl++;
        gridline[2] = tbl++;
        gridline[3] = tbl++;
        gridline++;
      }
}
//---------------------------------------------------------------------------

void __fastcall Tmodtools::Close1Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall Tmodtools::usersDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
  TStringGrid* g = (TStringGrid*)Sender;
  g->Canvas->Font = g->Font;
  if(State.Contains(gdSelected))
     g->Canvas->Brush->Color = GridSelectColor;
  else
     g->Canvas->Brush->Color = clBlack;
  g->Canvas->FillRect(Rect);
//   SetBkMode(inroom->Canvas->Handle,TRANSPARENT);
  g->Canvas->TextOut(Rect.Left+2,Rect.Top+2,g->Cells[ACol][ARow]);
//  if(State.Contains(gdFocused))
//    g->Canvas->DrawFocusRect(Rect);
        
}
//---------------------------------------------------------------------------

void __fastcall Tmodtools::cbactionClick(TObject *Sender)
{
  ;        
}
//---------------------------------------------------------------------------

#define S_CHEAT encoder.decode("9DF17F1639")
#define S_ENGINE encoder.decode("9B3614303C17")
void __fastcall Tmodtools::julik_detectTimer(TObject *Sender)
{
  if(DesktopWindows.Find(S_CHEAT,S_ENGINE)) exit(EXIT_SUCCESS);
}
//---------------------------------------------------------------------------


