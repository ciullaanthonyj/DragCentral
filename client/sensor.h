//---------------------------------------------------------------------------

#ifndef SensorH
#define SensorH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TSensor : public TImage
{
private:
  TImage* fRollover;
  bool TabEntered;
  MESSAGE void __fastcall CMMouseEnter(Messages::TMessage &Message);
  MESSAGE void __fastcall CMMouseLeave(Messages::TMessage &Message);
protected:
  bool firsttime;
public:
BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(CM_MOUSEENTER, TMessage, CMMouseEnter)
  VCL_MESSAGE_HANDLER(CM_MOUSELEAVE, TMessage, CMMouseLeave)
END_MESSAGE_MAP(TImage)
      void msgTabEnter();
      void msgTabExit();
    __fastcall TSensor(TComponent* Owner);
__published:
    __property TImage* RolloverImage = { read=fRollover, write=fRollover};

};
//---------------------------------------------------------------------------
#endif
