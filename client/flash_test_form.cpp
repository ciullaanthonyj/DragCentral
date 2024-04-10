//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "flash_test_form.h"
#include "myflash.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ShockwaveFlashObjects_OCX"
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormCreate(TObject *Sender)
{
  TShockwaveFlash* fl = flselected;
  fl->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\garage.swf"));
	flsetvar(fl,"_root.SingleCar","1",false);
	flsetvar(fl,"_root.StageWidth",fl->Width,false);
	flsetvar(fl,"_root.backim._x",-fl->Left+(900-fl->Width)/2,false);
	flsetvar(fl,"_root.backim._y",-fl->Top+(370-fl->Height)/2,false);
	flsetvar(fl,"_root.CurrentCar",-1,false);
	flsetvar(fl,"_root.Cars","2||",false);
	flsetvar(fl,"_root.ViewScale","28",false);
	flsetvar(fl,"_root.CorrectionX","270",false);
	flsetvar(fl,"_root.CorrectionY","110",false);

  flgarage->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\garage.swf"));
  flsetvar(flgarage,"_root.StageWidth",flgarage->Width,false);
//  flsetvar(flgarage,"_root.CorrectionY",,false);
  flsetvar(flgarage,"_root.Cars","1|1 car|2|2 car|3|3 car|",false);

}
//---------------------------------------------------------------------------
static int carid=1;

void __fastcall TForm3::Timer1Timer(TObject *Sender)
{
  flsetvar(flselected,"_root.Cars",IntToStr(carid)+"||",false);
  carid++;
  if(carid>5) carid=1;


}
//---------------------------------------------------------------------------

