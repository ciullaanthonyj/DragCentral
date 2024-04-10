//---------------------------------------------------------------------------

#ifndef cSecureStringH
#define cSecureStringH
//---------------------------------------------------------------------------
#include <vcl.h>

class cSecureString {
private:
  AnsiString internal;
  int crc;
public:
  operator=(AnsiString newvalue);
  AnsiString val();
};




#endif
