//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "form_teamchallenge.h"
#include "myinternet.h"
#include "myvcl.h"
#include "common_stuff.h"
#include "confirmForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgrbutton"
#pragma resource "*.dfm"
Tcreatechallenge *createchallenge;
//---------------------------------------------------------------------------
__fastcall Tcreatechallenge::Tcreatechallenge(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tcreatechallenge::FormShow(TObject *Sender)
{
  static bool firsttime=true;
  if(firsttime) {
    this->Top = (Screen->Height - this->Height) / 2 - 42;
    this->Left = (Screen->Width - this->Width) / 2;
    firsttime = false;
  }
  
  Panel1->Color = this->Color;
  reset->OnClick(0);
  lbTeamAccount->Caption = teamaccount;
  btnadd->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall Tcreatechallenge::btnaddClick(TObject *Sender)
{
  if(yourmember->ItemIndex==-1 || othermember->ItemIndex==-1)
  if(!(yourmember->Items->Count==0 || othermember->Items->Count==0))
    throw Exception("Select team member.");
  AnsiString your = yourmember->Items->Strings[yourmember->ItemIndex];
  AnsiString other = othermember->Items->Strings[othermember->ItemIndex];
  AnsiString yourid;
  AnsiString otherid;
  {
    cStringTable tbl(this->yourteam);
    while(tbl.GetLine()) {
      yourid = tbl++;
      AnsiString nick = tbl++;
      tbl++;
      tbl++;
      AnsiString car = tbl++;
      if(your==nick + " - " + car) break;
    }
  }

  {
    cStringTable tbl(this->otherteam);
    while(tbl.GetLine()) {
      otherid = tbl++;
      AnsiString nick = tbl++;
      tbl++;
      tbl++;
      AnsiString car = tbl++;
      if(other==nick + " - " + car) break;
    }
  }

  if(!(versus->RowCount==1 && versus->Cells[0][0]==""))
    versus->RowCount += 1;

  versus->Cells[0][versus->RowCount-1] = yourid;
  versus->Cells[1][versus->RowCount-1] = your;
  versus->Cells[2][versus->RowCount-1] = otherid;
  versus->Cells[3][versus->RowCount-1] = other;

  yourmember->Items->Delete(yourmember->ItemIndex);
  othermember->Items->Delete(othermember->ItemIndex);

//  start->Enabled = (yourmember->Items->Count==0) || (othermember->Items->Count==0);
  start->Enabled = true;
  btnadd->Enabled = yourmember->Items->Count!=0 && othermember->Items->Count!=0;

}
//---------------------------------------------------------------------------
void __fastcall Tcreatechallenge::resetClick(TObject *Sender)
{
  AnsiString s;
  {
    cStringTable tbl(this->yourteam);
    yourmember->Items->Clear();
    while(tbl.GetLine()) {
      tbl++;
      s = tbl++; //racer name
      tbl++; // leader
      tbl++; // carid
      s = s + " - " + tbl++; // car name
      yourmember->Items->Add(s);
    }
    yourmember->ItemIndex = 0;
  }
  {
    cStringTable tbl(this->otherteam);
    othermember->Items->Clear();
    while(tbl.GetLine()) {
      tbl++;
      s = tbl++; //racer name
      tbl++; // leader
      tbl++; // carid
      s = s + " - " + tbl++; // car name
      othermember->Items->Add(s);
    }
    othermember->ItemIndex = 0;
  }

  versus->RowCount = 0;

  for(int i=0;i<versus->ColCount;i++) {
    if(versus->ColWidths[i]<8)
      versus->ColWidths[i] = 0;
    versus->Cells[i][0] = "";

  }
  start->Enabled = false;
  btnadd->Enabled = yourmember->Items->Count!=0 && othermember->Items->Count!=0;
}
//---------------------------------------------------------------------------
void __fastcall Tcreatechallenge::formoneyClick(TObject *Sender)
{
  edWager->Enabled = formoney->Checked;
  lbwager->Enabled = formoney->Checked;
  if(forfun->Checked)
    edWager->Text = "";        
}
//---------------------------------------------------------------------------
void __fastcall Tcreatechallenge::edWagerExit(TObject *Sender)
{
  int value;
  try {
    value = StrToInt(edWager->Text);
  } catch (Exception &exception) {
	edWager->Text = S_0;
	value = 0;
  }
  if(value<=0)
    edWager->Text = S_0;
  if(value>teamaccount)
    edWager->Text = teamaccount;

}
//---------------------------------------------------------------------------
void __fastcall Tcreatechallenge::startClick(TObject *Sender)
{
  if(!request->confirm("Create team challenge?"))
    return;
  challengers = "";
  for(int i=0;i<versus->RowCount;i++) {
    challengers += "u"+versus->Cells[0][i]+"c0";
    challengers += "u"+versus->Cells[2][i]+"c0";
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall Tcreatechallenge::versusDrawCell(TObject *Sender, int ACol,
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


