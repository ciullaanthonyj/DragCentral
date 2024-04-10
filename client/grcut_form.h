//---------------------------------------------------------------------------

#ifndef grcut_formH
#define grcut_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <inifiles.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ExtDlgs.hpp>
//---------------------------------------------------------------------------
class Tfm : public TForm
{
__published:	// IDE-managed Components
        TImage *im;
        TButton *open;
        TEdit *filename;
        TImage *sel;
        TButton *save;
        TSaveDialog *sd;
        TOpenDialog *od2;
        TOpenPictureDialog *od;
        TSavePictureDialog *sd3;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall openClick(TObject *Sender);
        void __fastcall imMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall imMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall saveClick(TObject *Sender);
        void __fastcall imMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
private:	// User declarations
public:		// User declarations
        __fastcall Tfm(TComponent* Owner);
        TIniFile* ini;
        void CopySelection();
};
//---------------------------------------------------------------------------
extern PACKAGE Tfm *fm;
//---------------------------------------------------------------------------
#endif
