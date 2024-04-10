//---------------------------------------------------------------------------

#ifndef common_stuffH
#define common_stuffH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class T_common : public TForm
{
__published:	// IDE-managed Components
	TImage *_ButtonsBack;
        TImage *im;
	TImage *_ButtonsBack1;
private:	// User declarations
public:		// User declarations
        __fastcall T_common(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE T_common *_common;
//---------------------------------------------------------------------------
#endif
