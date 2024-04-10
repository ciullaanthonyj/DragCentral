//---------------------------------------------------------------------------

#ifndef myvclH
#define myvclH
//---------------------------------------------------------------------------

TColor WebColor(AnsiString color);
//in selected row - case insensitive (lowercasing both)
AnsiString GridCell(TStringGrid* sg,int col);
AnsiString replchar(char what, char onwhat, AnsiString in);
void FixStringGrids(TComponent* c);
void RemoveLabelCaptions(TComponent* c); // if started from 'lb'
bool KeyPressed(int KeyCode);
bool GridCellLike(TStringGrid* sg,int col,AnsiString like);
void FixStringGrid(TStringGrid* sg);
void ClearGrid(TStringGrid* sg);
void MoveForm(TForm* f);
void SetRoundWindow(TForm* f,int radius);
void RemoveImageBlink(TComponent* c);
void ReplaceEditsColor(TComponent* c,int OriginalColor, int SetColor);
void addline(TRichEdit* re,
        AnsiString first, TColor color,bool Bold,
        AnsiString second, TColor color2,bool Bold2 );

class cGridCell {
public:
   TStringGrid* grid;
   int cell;
   int row;
   void operator=(AnsiString s);
   AnsiString val() {return grid->Cells[cell][row];}
   bool like(AnsiString s) {
      return (val().LowerCase().Pos(s.LowerCase())!=0);
   }
};

class cGridFiller {
  TStringGrid* grid;
  cGridCell cell;
  int SaveRow;
public:
  cGridFiller(TStringGrid* newgrid,bool SaveRow = false);
  ~cGridFiller();
  void setcell(int i, AnsiString value);
  cGridCell& operator[](int i);
  void nextrow();
  int operator++(int);
};

extern TColor GridSelectColor;
#endif
