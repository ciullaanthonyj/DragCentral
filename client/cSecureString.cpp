//---------------------------------------------------------------------------


#pragma hdrstop

#include "cSecureString.h"
cSecureString::operator=(AnsiString newvalue) {
	  internal = newvalue;
	  for(int i=1;i<=internal.Length();i++) {
		  internal[i] = internal[i] ^ 0x7c;
	  }
	  crc = 0;
	  for(int i=1;i<=internal.Length();i++) {
		  crc += internal[i];
	  }

  }
AnsiString cSecureString::val() {
		  int checkcrc = 0;
	  for(int i=1;i<=internal.Length();i++) {
		  checkcrc += internal[i];
	  }
	  if (crc!=checkcrc) {
		  abort();
	  }
	  AnsiString res = internal;
	  for(int i=1;i<=internal.Length();i++) {
		  res[i] = internal[i] ^ 0x7c;
	  }

	  return res;
  
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
