//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop

#include "myvcl.h"

//---------------------------------------------------------------------------
void FixStringGrid(TStringGrid* sg) {
  sg->FixedRows = 0;
  for(int i=0;i<sg->ColCount;i++)
    if(sg->ColWidths[i]<12) sg->ColWidths[i]=0;
  sg->RowCount = 0;
}

AnsiString replchar(char what, char onwhat, AnsiString in) {
  AnsiString out = in;
  for(int i=1;i<=out.Length();i++)
    if(out[i]==what) out[i]=onwhat;

  return out;
}
bool GridCellLike(TStringGrid* sg,int col,AnsiString like) {
  return sg->Cells[col][sg->Row].LowerCase().Pos(like.LowerCase())!=0;
}

AnsiString GridCell(TStringGrid* sg,int col) {
  return sg->Cells[col][sg->Row];
}

void ClearGrid(TStringGrid* sg) {
  sg->FixedRows = 0;
  for(int j=0;j<sg->RowCount;j++)
  for(int i=0;i<sg->ColCount;i++)
    sg->Cells[i][j] = "";
}

bool KeyPressed(int KeyCode)
{
  if (!Application->Active) return false;
  if((GetKeyState(KeyCode) & 0xFF00)) return true;
  return false;
}

TColor WebColor(AnsiString color) {
  AnsiString converted;
  if(color.Length()==0) return (TColor)0;
  converted = color;
  if(color[1]=='#')
    converted = "0x"+color.SubString(6,2)+
      color.SubString(4,2)+color.SubString(2,2);;

  return (TColor)StrToInt(converted);

}

void SetRoundWindow(TForm* f,int radius) {
  POINT point[10];
 point[0].x=radius;
 point[0].y=0;
 point[1].x=f->Width-radius;
 point[1].y=0;
 point[2].x=f->Width;
 point[2].y=radius;
 point[3].x=f->Width;
 point[3].y=f->Height-radius;
 point[4].x=f->Width-radius;
 point[4].y=f->Height;
 point[5].x=radius;
 point[5].y=f->Height;
 point[6].x=0;
 point[6].y=f->Height-radius;
 point[7].x=0;
 point[7].y=radius;

 HRGN FullWindow=CreatePolygonRgn(point,8,WINDING);
 HRGN Hole=CreateEllipticRgn(0,0,radius*2,radius*2);
 CombineRgn(FullWindow,FullWindow,Hole,RGN_OR);
 HRGN Hole2=CreateEllipticRgn(f->Width-radius*2+1,0,f->Width+1,radius*2);
 CombineRgn(FullWindow,FullWindow,Hole2,RGN_OR);
 HRGN Hole3=CreateEllipticRgn(f->Width-radius*2+1,f->Height-radius*2+1,f->Width+1,f->Height+1);
 CombineRgn(FullWindow,FullWindow,Hole3,RGN_OR);
 HRGN Hole4=CreateEllipticRgn(0,f->Height-radius*2+1,radius*2,f->Height+1);
 CombineRgn(FullWindow,FullWindow,Hole4,RGN_OR);

 SetWindowRgn(f->Handle,FullWindow,false);
}

void MoveForm(TForm* f) {
  {
    ReleaseCapture();
    SNDMSG(f->Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
  }
}
void RemoveImageBlink(TComponent* c) {
  for(int i=0;i<c->ComponentCount;i++)
    if(dynamic_cast<TImage*>(c->Components[i])!=NULL) {
      TImage* im = (TImage*)(c->Components[i]);
      if(im->Picture->Graphic!=NULL)
        im->Parent->DoubleBuffered = true;
    }
}
void RemoveLabelCaptions(TComponent* c) {
  for(int i=0;i<c->ComponentCount;i++)
	if(dynamic_cast<TLabel*>(c->Components[i])!=NULL) {
	  TLabel* lb = (TLabel*)(c->Components[i]);
	  if(lb->Caption.SubString(0,2)=="lb")
		lb->Caption = "";
	}
}

void ReplaceEditsColor(TComponent* c,int OriginalColor, int SetColor) {
  for(int i=0;i<c->ComponentCount;i++)
    if(dynamic_cast<TEdit*>(c->Components[i])!=NULL) {
      TEdit* lb = (TEdit*)(c->Components[i]);
      if(lb->Color == OriginalColor)
		lb->Color = SetColor;
	}
}

void FixStringGrids(TComponent* c) {
  for(int i=0;i<c->ComponentCount;i++)
	if(dynamic_cast<TStringGrid*>(c->Components[i])!=NULL) {
	  TStringGrid* gr = (TStringGrid*)(c->Components[i]);
	  FixStringGrid(gr);
	}
}
void addline(TRichEdit* re,
        AnsiString first, TColor color,bool Bold,
        AnsiString second, TColor color2,bool Bold2 ) {

//  LockWindowUpdate(re->Handle);
  int savecaret = re->SelStart;
  int savelen = re->SelLength;

  int caret = re->Text.Length();
  if(first+second=="") return;

  re->Lines->Add(first+second);
  // FIRST ATTRIBUTES
  re->SelStart = caret;
  re->SelLength = first.Length();
  if(color!=-1)
    re->SelAttributes->Color = color;

  if(Bold)
    re->SelAttributes->Style =
      re->SelAttributes->Style << fsBold;
  else
    re->SelAttributes->Style =
      re->SelAttributes->Style >> fsBold;
  // SECOND ATTRIBUTES
  re->SelStart = caret+first.Length();
  re->SelLength = second.Length();
  if(color2!=-1)
    re->SelAttributes->Color = color2;
  if(Bold2)
    re->SelAttributes->Style =
      re->SelAttributes->Style << fsBold;
  else
    re->SelAttributes->Style =
      re->SelAttributes->Style >> fsBold;

  re->SelStart = savecaret;
  re->SelLength = savelen;
//  LockWindowUpdate(NULL);

}
   void cGridCell::operator=(AnsiString s) {
     grid->Cells[cell][row] = s;
   }
  cGridFiller::cGridFiller(TStringGrid* newgrid, bool SaveRow) {
     this->SaveRow = -1;
     if(SaveRow) {
       this->SaveRow = newgrid->Row;
     }
     grid = newgrid;
     grid->RowCount = 1;
     for(int i=0;i<grid->ColCount;i++)
       grid->Cells[i][0] = "";
     for(int i=0;i<grid->ColCount;i++)
       if(grid->ColWidths[i] < 12)
         grid->ColWidths[i] = 0 ;
  }
  cGridFiller::~cGridFiller() {
    grid->RowCount--;
    if(SaveRow!=-1) {
      if(SaveRow<grid->RowCount)
        grid->Row = SaveRow;
      else
        grid->Row = grid->RowCount-1;
    }
  }
  void cGridFiller::setcell(int i, AnsiString value) {
     grid->Cells[i][grid->RowCount-1] = value;
  }
  cGridCell& cGridFiller::operator[](int i) {
    cell.cell = i;
    cell.row = grid->RowCount-1;
    cell.grid = grid;
    return cell;
  }
  void cGridFiller::nextrow() {
    grid->RowCount++;
  }
  int cGridFiller::operator++(int) {
    nextrow();
    return grid->RowCount;
  }


TColor GridSelectColor = 0xab5a09;
#pragma package(smart_init)
