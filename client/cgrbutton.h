//---------------------------------------------------------------------------

#ifndef cgrbuttonH
#define cgrbuttonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#define min(a,b) a<b?a:b
#define max(a,b) a>b?a:b
class TMySensor : public TImage {
  MESSAGE void __fastcall CMMouseEnter(Messages::TMessage &Message) {
	if(OnMouseEnter!=0)
	  OnMouseEnter(this);
  }
  MESSAGE void __fastcall CMMouseLeave(Messages::TMessage &Message) {
	if(OnMouseLeave!=0)
	  OnMouseLeave(this);
  }
public:
  TNotifyEvent OnMouseEnter;
  TNotifyEvent OnMouseLeave;
BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(CM_MOUSEENTER, TMessage, CMMouseEnter)
  VCL_MESSAGE_HANDLER(CM_MOUSELEAVE, TMessage, CMMouseLeave)
END_MESSAGE_MAP(TImage)
  __fastcall TMySensor(TComponent* Owner);
};

__fastcall TMySensor::TMySensor(TComponent* Owner)
		: TImage(Owner) {}

class PACKAGE TGrButton : public TPaintBox
{
private:
  bool ordered;
  TScrollBar* KeyFocus;
  TBevel* bevel;
  AnsiString ACaption;
  TMySensor* biglabel; // label for mouse
  TLabel* lbl; // caption label
  TPicture* ATemplatePic;
  TImage* AExtImage;
  Graphics::TBitmap* bmp1;
  Graphics::TBitmap* bmp2;
  Graphics::TBitmap* bmp3;
  Graphics::TBitmap* bmp4;
  TColor FUpColor;
  TColor FDownColor;
  TKeyEvent FOnKeyDown; // should be assigned to focus button
  TKeyEvent FOnKeyUp;
  TKeyPressEvent FOnKeyPress;
  TNotifyEvent FOnEnter;
protected:
public:
	void __fastcall SetDownColor(TColor value) {
	  FDownColor = value;
	  lbl->Font->Color = value;
    }
	void __fastcall Paint(void) {
	  Graphics::TBitmap* bmp = bmp1;
	  Canvas->Draw(0,0,bmp1);
	  Canvas->Draw(Width-bmp->Width,0,bmp2);
	  Canvas->Draw(Width-bmp->Width,Height-bmp->Height,bmp3);
	  Canvas->Draw(0,Height-bmp->Height,bmp4);
	  if(this->Name!="" && KeyFocus->Name=="")
		KeyFocus->Name = this->Name + "_taborder";
	};

	void __fastcall SetParent(TWinControl* Parent) {
	  TPaintBox::SetParent(Parent);
	}
   void __fastcall PicChange(TObject*Sender) {
	  if(!bmp1) return;
	  if(ExtImage==0  && ATemplatePic==0) return;
	  if(ExtImage) {
		if(ExtImage->Picture->Width==0 || ExtImage->Picture->Height==0)
		   return;
	  }
	  else
	  if(ATemplatePic) {
		if(ATemplatePic->Width==0 || ATemplatePic->Height==0) return;
	  }

	  TPicture* pic = ATemplatePic;
	  if(ExtImage) pic=ExtImage->Picture;
	  Graphics::TBitmap* bmp = bmp1;
	  int copywidth = min(Width/2+1,pic->Width/2);
	  int copyheight = min(Height/2+1,pic->Height/2);
	  bmp->Width = Width/2+1;
	  bmp->Height = Height/2+1;
	  bmp2->Width = bmp->Width;
	  bmp2->Height = bmp->Height;
	  bmp3->Width = bmp->Width;
	  bmp3->Height = bmp->Height;
	  bmp4->Width = bmp->Width;
	  bmp4->Height = bmp->Height;

	  bmp1->Canvas->CopyRect(
		TRect(0,0,bmp->Width,bmp->Height),
		pic->Bitmap->Canvas,
		TRect(0,0,bmp->Width,bmp->Height));

	  bmp2->Canvas->CopyRect(
		TRect(0,0,bmp->Width,bmp->Height),
		pic->Bitmap->Canvas,
		TRect(pic->Bitmap->Width-bmp->Width,0,
		pic->Bitmap->Width,bmp->Height));

	  bmp3->Canvas->CopyRect(
		TRect(0,0,bmp->Width,bmp->Height),
		pic->Bitmap->Canvas,
		TRect(pic->Bitmap->Width-bmp->Width,
		  pic->Bitmap->Height-bmp->Height,
		  pic->Bitmap->Width,
		  pic->Bitmap->Height));

	  bmp4->Canvas->CopyRect(
		TRect(0,0,bmp->Width,bmp->Height),
		pic->Bitmap->Canvas,
		TRect(0,
		  pic->Bitmap->Height-bmp->Height,
		  bmp->Width,
		  pic->Bitmap->Height));

      bmp1->Transparent = true;
      bmp1->TransparentColor = bmp->Canvas->Pixels[0][0];
      bmp2->Transparent = true;
      bmp2->TransparentColor = bmp1->TransparentColor;
      bmp3->Transparent = true;
      bmp3->TransparentColor = bmp1->TransparentColor;
      bmp4->Transparent = true;
      bmp4->TransparentColor = bmp1->TransparentColor;


   }

   void __fastcall SetTemplatePic(TPicture* value) {
      ATemplatePic->Assign(value);
   }
   void __fastcall OnKeyFocusKeyDown(TObject* Sender, Word &Key, TShiftState Shift) {
     if(Key==VK_SPACE || Key==VK_RETURN) {
	   if(this->OnClick)
         OnClick(this);
     }
	 if(this->OnKeyDown)
	   OnKeyDown(this,Key,Shift);
   }

   void __fastcall OnKeyFocusKeyUp(TObject* Sender, Word &Key, TShiftState Shift) {
	 if(this->OnKeyUp)
	   OnKeyUp(this,Key,Shift);
   }

   void __fastcall OnKeyFocusKeyPress(TObject* Sender, wchar_t &Key) {
	 if(this->OnKeyPress) {
	   OnKeyPress(this,Key);
	 }
   }

   void __fastcall KeyFocusEnter(TObject *Sender) {
	 bevel->Visible = true;
	 if(FOnEnter!=0)
	   FOnEnter(this);
   };
   void __fastcall KeyFocusExit(TObject *Sender) {
	 bevel->Visible = false;
   };

   void __fastcall LabelMouseEnter(TObject*Sender) {
	   Up = true;
   }

   void __fastcall LabelMouseLeave(TObject*Sender) {
	   Up = false;
   }

   void __fastcall OnLabelClick(TObject*Sender) {
	 if(OnClick!=0) OnClick(this);
   }

   void __fastcall SetFocus() {
	 try {
	   if(KeyFocus->Parent==0)
		 KeyFocus->Parent = this->Parent;
	   if(KeyFocus->Visible && KeyFocus->Enabled)
		 KeyFocus->SetFocus();
	 }
	 catch(...) {}
   };

  void __fastcall OnLabelMouseDown(TObject* Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y) {

	 if(KeyFocus->Visible && KeyFocus->Enabled)
	   KeyFocus->SetFocus();
	 if(this->OnMouseDown)
	   OnMouseDown(this,Button,Shift,X,Y);
   }
  void __fastcall OnLabelMouseUp(TObject* Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y) {
	 if(this->OnMouseUp)
	   OnMouseUp(this,Button,Shift,X,Y);
   }
  void __fastcall OnLabelMouseMove(TObject* Sender,
	TShiftState Shift, int X, int Y) {
	if(this->OnMouseMove)
	  OnMouseMove(this,Shift,X,Y);
  }



   void __fastcall SetCaption(AnsiString value) {
	 this->ACaption = value;
	 lbl->Caption = value;
	 //move label
	 SetBounds(Left,Top,Width,Height);
   }
   void __fastcall KeyFocusClick(TObject *Sender) {
	 if(OnClick!=0)
	   OnClick(this);
   };
   void __fastcall SetBounds(int ALeft, int ATop, int AWidth, int AHeight) {
	 TPaintBox::SetBounds(ALeft,ATop,AWidth,AHeight);
	 this->PicChange(this);
	 if(bevel!=0 && lbl!=0 && biglabel!=0 && KeyFocus!=0) {
	   lbl->Top = ATop + (AHeight-lbl->Height)/2;
	   lbl->Left = ALeft + (AWidth-lbl->Width)/2;
       SetTabMargin(TabMargin);
       biglabel->SetBounds(ALeft,ATop,AWidth,AHeight);
	   if(!ordered && this->Parent!=0) {
		 lbl->Parent = this->Parent;
		 bevel->Parent = this->Parent;
		 KeyFocus->Parent = this->Parent;
		 biglabel->Parent = this->Parent;
		 ordered = true;
         if(!this->ComponentState.Contains(csDesigning)) {
           bevel->BringToFront();
           lbl->BringToFront();
           biglabel->BringToFront();
         } else {
           this->BringToFront();
           lbl->BringToFront();
         }

		 if(this->Parent!=0) {
		   if(!this->Parent->DoubleBuffered)
			 this->Parent->DoubleBuffered = true;
         }
       }
     }
   }
   __fastcall TGrButton(TComponent* Owner);
   __fastcall ~TGrButton();
   bool AEnabled;
   void __fastcall SetEnabled(bool value) {
	  TPaintBox::Enabled = value;
	  KeyFocus->Enabled = value;
	  if(!value) {
		if(Up) Up = false;
		KeyFocus->TabStop = false;
	  }
	  else
		KeyFocus->TabStop = true;
	  AEnabled = value;
	  lbl->Enabled = value;
	  biglabel->Enabled = value;
   }
   bool AVisible;
   void __fastcall SetVisible(bool value) {
	 AVisible = value;
	 TPaintBox::Visible = value;
	 KeyFocus->Visible = value;
	 lbl->Visible = value;
	 biglabel->Visible = value;
   }
   bool AUp;
   void __fastcall SetUp(bool value) {
	 if(value && !AUp) {
	   if(Enabled) {
//		 lbl->Top--;
		 lbl->Font->Color = FUpColor;
	   }
	 }
	 if(!value && AUp) {
	   if(Enabled) {
//		 lbl->Top++;
		 lbl->Font->Color = FDownColor;
	   }
	 }
	 AUp = value;
   }
   int ATabMargin;
   void __fastcall SetTabMargin(int value) {
	  ATabMargin = value;
	  // resize tab
	  if(bevel!=0)
		bevel->SetBounds(Left+TabMargin,Top+TabMargin,Width-TabMargin*2,Height-TabMargin*2);
   };
   void __fastcall SetExtImage(TImage* value) {
	 AExtImage = value;
	 this->PicChange(this);
   }

   int __fastcall GetTabOrder() {
	 return KeyFocus->TabOrder;
   }
   void __fastcall SetTabOrder(int value) {
	 KeyFocus->TabOrder = value;
   }
__published:
   __property AnsiString Caption = {read=ACaption,write=SetCaption,stored=true};
   __property bool Enabled = {read=AEnabled,write=SetEnabled };
   __property bool Visible = {read=AVisible,write=SetVisible,stored=true };
   __property bool Up = {read=AUp,write=SetUp,stored=true};
   __property TPicture* TemplatePic= {read=ATemplatePic,write=SetTemplatePic,stored=true };
   __property int TabMargin = {read=ATabMargin,write=SetTabMargin,stored=true};
   __property TImage* ExtImage = {read=AExtImage, write=SetExtImage, stored=true};
   __property int TabOrder = {read=GetTabOrder, write=SetTabOrder,stored=true};
   __property TColor ColorUp = {read=FUpColor,write=FUpColor};
   __property TColor ColorDown = {read=FDownColor, write=SetDownColor};
   __property TKeyEvent OnKeyDown = {read=FOnKeyDown,write=FOnKeyDown,stored=true};
   __property TKeyEvent OnKeyUp = {read=FOnKeyUp,write=FOnKeyUp,stored=true};
   __property TKeyPressEvent OnKeyPress = {read=FOnKeyPress,write=FOnKeyPress,stored=true};
   __property TNotifyEvent OnEnter = {read=FOnEnter, write=FOnEnter};

};
//---------------------------------------------------------------------------
#endif
