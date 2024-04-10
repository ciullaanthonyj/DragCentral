//---------------------------------------------------------------------------

#ifndef formHelpH
#define formHelpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class THelp : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
	__fastcall THelp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE THelp *Help;
//---------------------------------------------------------------------------
#endif
