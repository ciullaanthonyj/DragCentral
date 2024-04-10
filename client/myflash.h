//---------------------------------------------------------------------------

#ifndef myflashH
#define myflashH
//---------------------------------------------------------------------------
#include "ShockwaveFlashObjects_OCX.h"
#include "cprofiler.h"

wchar_t* wc(AnsiString c) {
   static wchar_t w[1024];
   c.WideChar(w,1024);
   return w;
}
wchar_t* wc2(AnsiString c) {
   static wchar_t w[1024];
   c.WideChar(w,1024);
   return w;
}
AnsiString f2s(float v);
float prop(float pmin,float pmax,float p,float vmin,float vmax);
void flvisible(TShockwaveFlash* fl,AnsiString name,bool value);
void flsetvar(TShockwaveFlash* fl,AnsiString varname, AnsiString value,bool cache=true);
// call this in case variable is changing in flash to parse it for set var
AnsiString flgetvar(TShockwaveFlash* fl,AnsiString varname);

struct MoviePos {
  float x;
  float y;
  float xscale;
  float yscale;
  float cx;
  float cy;
  float width;
  float height;
  float alpha;
  void getvars(TShockwaveFlash* fl,AnsiString name);
  void setvars(TShockwaveFlash* fl,AnsiString name);
  //same as setvars but sets name_pos value with all vars
  void setstringvars(TShockwaveFlash* fl,AnsiString name);
  void scale(float pmin,float pmax,float p,MoviePos &s, MoviePos &e);
};

struct FlashFlic {
  TShockwaveFlash* mv;
  void setvisible(AnsiString name, bool visible);
  void setvar(AnsiString name, AnsiString value);
};

#endif
