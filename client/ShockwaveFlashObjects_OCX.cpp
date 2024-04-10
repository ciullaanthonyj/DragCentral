// ************************************************************************ //
// WARNING                                                                    
// -------                                                                    
// The types declared in this file were generated from data read from a       
// Type Library. If this type library is explicitly or indirectly (via        
// another type library referring to this type library) re-imported, or the   
// 'Refresh' command of the Type Library Editor activated while editing the   
// Type Library, the contents of this file will be regenerated and all        
// manual modifications will be lost.                                         
// ************************************************************************ //

// C++ TLBWRTR : $Revision:   1.151.1.0.1.27  $
// File generated on 12.04.2006 5:05:02 from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\WINXPNEW\system32\Macromed\Flash\Flash8.ocx (1)
// LIBID: {D27CDB6B-AE6D-11CF-96B8-444553540000}
// LCID: 0
// Helpfile: 
// HelpString: Shockwave Flash
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINXPNEW\System32\stdole2.tlb)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include <olectrls.hpp>
#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "ShockwaveFlashObjects_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Shockwaveflashobjects_tlb
{



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TShockwaveFlash which
// allows "Shockwave Flash" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
int   TShockwaveFlash::EventDispIDs[4] = {
    0xFFFFFD9F, 0x000007A6, 0x00000096, 0x000000C5};

TControlData TShockwaveFlash::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0xD27CDB6E, 0xAE6D, 0x11CF,{ 0x96, 0xB8, 0x44,0x45, 0x53, 0x54,0x00, 0x00} }, // CoClass
  {0xD27CDB6D, 0xAE6D, 0x11CF,{ 0x96, 0xB8, 0x44,0x45, 0x53, 0x54,0x00, 0x00} }, // Events

  // Count of Events and array of their DISPIDs
  4, &EventDispIDs,

  // Pointer to Runtime License string
  NULL,  // HRESULT(0x80004002)

  // Flags for OnChanged PropertyNotification
  0x00000000,
  300,// (IDE Version)

  // Count of Font Prop and array of their DISPIDs
  0, NULL,

  // Count of Pict Prop and array of their DISPIDs
  0, NULL,
  0, // Reserved
  0, // Instance count (used internally)
  0, // List of Enum descriptions (internal)
};

GUID     TShockwaveFlash::DEF_CTL_INTF = {0xD27CDB6C, 0xAE6D, 0x11CF,{ 0x96, 0xB8, 0x44,0x45, 0x53, 0x54,0x00, 0x00} };
TNoParam TShockwaveFlash::OptParam;

static inline void ValidCtrCheck(TShockwaveFlash *)
{
   delete new TShockwaveFlash((TComponent*)(0));
};

void __fastcall TShockwaveFlash::InitControlData()
{
  ControlData = &CControlData;
};

void __fastcall TShockwaveFlash::CreateControl()
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

TCOMIShockwaveFlash __fastcall TShockwaveFlash::GetDefaultInterface()
{
  CreateControl();
  return m_OCXIntf;
};

void __fastcall TShockwaveFlash::SetZoomRect(long left/*[in]*/, long top/*[in]*/, long right/*[in]*/, 
                                             long bottom/*[in]*/)
{
  GetDefaultInterface()->SetZoomRect(left/*[in]*/, top/*[in]*/, right/*[in]*/, bottom/*[in]*/);
}

void __fastcall TShockwaveFlash::Zoom(int factor/*[in]*/)
{
  GetDefaultInterface()->Zoom(factor/*[in]*/);
}

void __fastcall TShockwaveFlash::Pan(long x/*[in]*/, long y/*[in]*/, int mode/*[in]*/)
{
  GetDefaultInterface()->Pan(x/*[in]*/, y/*[in]*/, mode/*[in]*/);
}

void __fastcall TShockwaveFlash::Play(void)
{
  GetDefaultInterface()->Play();
}

void __fastcall TShockwaveFlash::Stop(void)
{
  GetDefaultInterface()->Stop();
}

void __fastcall TShockwaveFlash::Back(void)
{
  GetDefaultInterface()->Back();
}

void __fastcall TShockwaveFlash::Forward(void)
{
  GetDefaultInterface()->Forward();
}

void __fastcall TShockwaveFlash::Rewind(void)
{
  GetDefaultInterface()->Rewind();
}

void __fastcall TShockwaveFlash::StopPlay(void)
{
  GetDefaultInterface()->StopPlay();
}

void __fastcall TShockwaveFlash::GotoFrame(long FrameNum/*[in]*/)
{
  GetDefaultInterface()->GotoFrame(FrameNum/*[in]*/);
}

long __fastcall TShockwaveFlash::CurrentFrame(void)
{
  return GetDefaultInterface()->CurrentFrame();
}

TOLEBOOL __fastcall TShockwaveFlash::IsPlaying(void)
{
  return GetDefaultInterface()->IsPlaying();
}

long __fastcall TShockwaveFlash::PercentLoaded(void)
{
  return GetDefaultInterface()->PercentLoaded();
}

TOLEBOOL __fastcall TShockwaveFlash::FrameLoaded(long FrameNum/*[in]*/)
{
  return GetDefaultInterface()->FrameLoaded(FrameNum/*[in]*/);
}

long __fastcall TShockwaveFlash::FlashVersion(void)
{
  return GetDefaultInterface()->FlashVersion();
}

void __fastcall TShockwaveFlash::LoadMovie(int layer/*[in]*/, BSTR url/*[in]*/)
{
  GetDefaultInterface()->LoadMovie(layer/*[in]*/, url/*[in]*/);
}

void __fastcall TShockwaveFlash::TGotoFrame(BSTR target/*[in]*/, long FrameNum/*[in]*/)
{
  GetDefaultInterface()->TGotoFrame(target/*[in]*/, FrameNum/*[in]*/);
}

void __fastcall TShockwaveFlash::TGotoLabel(BSTR target/*[in]*/, BSTR label/*[in]*/)
{
  GetDefaultInterface()->TGotoLabel(target/*[in]*/, label/*[in]*/);
}

long __fastcall TShockwaveFlash::TCurrentFrame(BSTR target/*[in]*/)
{
  return GetDefaultInterface()->TCurrentFrame(target/*[in]*/);
}

BSTR __fastcall TShockwaveFlash::TCurrentLabel(BSTR target/*[in]*/)
{
  return GetDefaultInterface()->TCurrentLabel(target/*[in]*/);
}

void __fastcall TShockwaveFlash::TPlay(BSTR target/*[in]*/)
{
  GetDefaultInterface()->TPlay(target/*[in]*/);
}

void __fastcall TShockwaveFlash::TStopPlay(BSTR target/*[in]*/)
{
  GetDefaultInterface()->TStopPlay(target/*[in]*/);
}

void __fastcall TShockwaveFlash::SetVariable(BSTR name/*[in]*/, BSTR value/*[in]*/)
{
  GetDefaultInterface()->SetVariable(name/*[in]*/, value/*[in]*/);
}

BSTR __fastcall TShockwaveFlash::GetVariable(BSTR name/*[in]*/)
{
  return GetDefaultInterface()->GetVariable(name/*[in]*/);
}

void __fastcall TShockwaveFlash::TSetProperty(BSTR target/*[in]*/, int property/*[in]*/, 
                                              BSTR value/*[in]*/)
{
  GetDefaultInterface()->TSetProperty(target/*[in]*/, property/*[in]*/, value/*[in]*/);
}

BSTR __fastcall TShockwaveFlash::TGetProperty(BSTR target/*[in]*/, int property/*[in]*/)
{
  return GetDefaultInterface()->TGetProperty(target/*[in]*/, property/*[in]*/);
}

void __fastcall TShockwaveFlash::TCallFrame(BSTR target/*[in]*/, int FrameNum/*[in]*/)
{
  GetDefaultInterface()->TCallFrame(target/*[in]*/, FrameNum/*[in]*/);
}

void __fastcall TShockwaveFlash::TCallLabel(BSTR target/*[in]*/, BSTR label/*[in]*/)
{
  GetDefaultInterface()->TCallLabel(target/*[in]*/, label/*[in]*/);
}

void __fastcall TShockwaveFlash::TSetPropertyNum(BSTR target/*[in]*/, int property/*[in]*/, 
                                                 double value/*[in]*/)
{
  GetDefaultInterface()->TSetPropertyNum(target/*[in]*/, property/*[in]*/, value/*[in]*/);
}

double __fastcall TShockwaveFlash::TGetPropertyNum(BSTR target/*[in]*/, int property/*[in]*/)
{
  return GetDefaultInterface()->TGetPropertyNum(target/*[in]*/, property/*[in]*/);
}

double __fastcall TShockwaveFlash::TGetPropertyAsNumber(BSTR target/*[in]*/, int property/*[in]*/)
{
  return GetDefaultInterface()->TGetPropertyAsNumber(target/*[in]*/, property/*[in]*/);
}

void __fastcall TShockwaveFlash::EnforceLocalSecurity(void)
{
  GetDefaultInterface()->EnforceLocalSecurity();
}

BSTR __fastcall TShockwaveFlash::CallFunction(BSTR request/*[in]*/)
{
  return GetDefaultInterface()->CallFunction(request/*[in]*/);
}

void __fastcall TShockwaveFlash::SetReturnValue(BSTR returnValue/*[in]*/)
{
  GetDefaultInterface()->SetReturnValue(returnValue/*[in]*/);
}

void __fastcall TShockwaveFlash::DisableLocalSecurity(void)
{
  GetDefaultInterface()->DisableLocalSecurity();
}

LPUNKNOWN __fastcall TShockwaveFlash::get_InlineData(void)
{
  return GetDefaultInterface()->get_InlineData();
}

void __fastcall TShockwaveFlash::set_InlineData(LPUNKNOWN ppIUnknown/*[in]*/)
{
  GetDefaultInterface()->set_InlineData(ppIUnknown/*[in]*/);
}


};     // namespace Shockwaveflashobjects_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Shockwaveflashobjects_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  TComponentClass cls_ocx[] = {
                              __classid(Shockwaveflashobjects_tlb::TShockwaveFlash)
                           };
  RegisterComponents("ActiveX", cls_ocx,
                     sizeof(cls_ocx)/sizeof(cls_ocx[0])-1);
}

};     // namespace Shockwaveflashobjects_ocx
