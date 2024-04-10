//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "encoder_form.h"
#include "cencoder.h"
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
  Edit2->Text = encoder.encode(Edit1->Text);
  Edit3->Text = "encoder.decode(\""+Edit2->Text+"\") /*\""+Edit1->Text+"\"*/";        
}
//---------------------------------------------------------------------------


#include <clipbrd.hpp>
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  TClipboard* clip = Clipboard();

  static AnsiString clipboard = "";

  if(clip->AsText!=clipboard && clip->AsText!=Edit3->Text) {
     clipboard = clip->AsText;
     Edit1->Text = clipboard;
     Button1Click(0);
     clip->AsText = Edit3->Text;

  }


}
//---------------------------------------------------------------------------

bool over;
void __fastcall TForm1::Button2Click(TObject *Sender)
{
  int FoundAt, StartPos, ToEnd;

  int first;
  int second;
  while(1) {
     //first
  // begin the search after the current selection
  // if there is one
  // otherwise, begin at the start of the text
  if (RichEdit1->SelLength)
    StartPos = RichEdit1->SelStart + RichEdit1->SelLength;
  else
    StartPos = 0;
  RichEdit1->SetFocus();

  // ToEnd is the length from StartPos
  // to the end of the text in the rich edit control

  ToEnd = RichEdit1->Text.Length() - StartPos;

     FoundAt = RichEdit1->FindText("\"", StartPos, ToEnd, TSearchTypes()<< stMatchCase);
     if (FoundAt != -1)
     {
       first = FoundAt;
       if(
       RichEdit1->Lines->operator[](RichEdit1->Perform(EM_LINEFROMCHAR,first,0)).Pos("L\"")!=0 ||

       RichEdit1->Lines->operator[](RichEdit1->Perform(EM_LINEFROMCHAR,first,0)).SubString(1,1)=="#") {
          RichEdit1->SelStart = first + 1;
          RichEdit1->SelLength = 2;
          continue;
       }
     } else {
       over = true;
       return;
     }

     FoundAt = RichEdit1->FindText("\"", first+1, ToEnd, TSearchTypes()<< stMatchCase);

     if(FoundAt != -1) {
       second = FoundAt;
       if(
       RichEdit1->Lines->operator[](RichEdit1->Perform(EM_LINEFROMCHAR,first,0)).Pos("L\"")!=0 ||       
       RichEdit1->Lines->operator[](RichEdit1->Perform(EM_LINEFROMCHAR,second,0)).SubString(1,1)=="#") {
         RichEdit1->SelStart = second + 2;
         RichEdit1->SelLength = 2;
         continue;
       }

     } else {
       over = true;
       return;
     }
     break;
   }

   RichEdit1->SetFocus();
   RichEdit1->SelStart = first+1;
   RichEdit1->SelLength = 1+second-first-2;
   Edit1->Text =  RichEdit1->SelText;

   if(Edit1->Text!="") {
     Button1Click(0);

     RichEdit1->SelStart = first;
     RichEdit1->SelLength = 1+second-first;

     RichEdit1->SelText = Edit3->Text;
     RichEdit1->SelStart = first;
     RichEdit1->SelLength = Edit3->Text.Length();
   } else {
     RichEdit1->SelLength += 2;
   }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
  over = false;
  while(1) {
    Button2Click(0);
    if(over) break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
  TClipboard* clip = Clipboard();

  static AnsiString clipboard = "";

  if(clip->AsText!=clipboard && clip->AsText!=RichEdit1->Text) {
     clipboard = clip->AsText;
     RichEdit1->Text = clipboard;
     Button3Click(0);
     clip->AsText = RichEdit1->Text;
  }
  
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
  Timer2->Enabled = CheckBox1->Enabled;        
}
//---------------------------------------------------------------------------

