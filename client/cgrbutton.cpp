//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop


#include "cgrbutton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TGrButton *)
{
   new TGrButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TGrButton::~TGrButton() {
  delete ATemplatePic;
  delete bmp1;
  delete bmp2;
  delete bmp3;
  delete bmp4;
}

__fastcall TGrButton::TGrButton(TComponent* Owner)
        : TPaintBox(Owner)
{
  AEnabled = true; //default
  AVisible = true; //default
  ATabMargin = 3;
  ColorUp = clBlue;
  bmp1 = new Graphics::TBitmap;
  bmp2 = new Graphics::TBitmap;
  bmp3 = new Graphics::TBitmap;
  bmp4 = new Graphics::TBitmap;
  ATemplatePic = new TPicture;
  ATemplatePic->OnChange = this->PicChange;
  //ATemplatePic->OnProgress
  Up = false;
//  this->Transparent = true;
  bevel = new TBevel(this);
//  bevel->Parent = (TWinControl*)(Owner);
  bevel->Visible = false;
//  bevel->Shape = bsFrame;
  KeyFocus = new TScrollBar(this); // seems the only for tab, and not up down
//  but->Parent = (TWinControl*)(Owner);
  KeyFocus->Width = 0;
  KeyFocus->Height = 0;
  KeyFocus->OnEnter = KeyFocusEnter;
  KeyFocus->OnExit = KeyFocusExit;
  KeyFocus->OnKeyDown = OnKeyFocusKeyDown;
  KeyFocus->OnKeyUp = OnKeyFocusKeyUp;
  KeyFocus->OnKeyPress = OnKeyFocusKeyPress;
  lbl = new TLabel(this);

//  lbl->Parent = this->Parent; //(TWinControl*)(Owner);
  lbl->Caption = "shit";
  lbl->Transparent = true;
  lbl->AutoSize = true;
  lbl->Alignment = taCenter;
//  lbl->OnMouseEnter = LabelMouseEnter;

  biglabel = new TMySensor(this);
  biglabel->Transparent = true;
  biglabel->OnMouseEnter = LabelMouseEnter;
  biglabel->OnMouseLeave = LabelMouseLeave;
  biglabel->OnClick = OnLabelClick;
  biglabel->OnMouseDown = OnLabelMouseDown;
  biglabel->OnMouseUp = OnLabelMouseUp;
  biglabel->OnMouseMove = OnLabelMouseMove;
  ordered = false;
}
//---------------------------------------------------------------------------
namespace Cgrbutton
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TGrButton)};
                 RegisterComponents("noizz", classes, 0);
        }
}
//---------------------------------------------------------------------------
