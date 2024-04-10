//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop

#include "cencoder.h"

//---------------------------------------------------------------------------
cEncoder encoder;

//this will be specific DRAGCENTRAL settings
//only for EncString
unsigned short this_C1 = 41123;
unsigned short this_C2 = 21890;



AnsiString enc_Decrypt(AnsiString in, unsigned short Key)
{
  AnsiString out;
  out=in;
  int l = in.Length();
  for(int i=1;i<=l;i++) {
	out[i] = in[i] ^ (Key >> 8);
	Key = (Key+(unsigned char)(in[i])) * this_C1 + this_C2;
  }
  return out;
}


AnsiString enc_Encrypt(AnsiString in, unsigned short Key)
{
  AnsiString out;
  out=in;
  int l = in.Length();
  for(int i=1;i<=l;i++) {
	out[i] = in[i] ^ (Key >> 8);
	Key = (Key+(unsigned char)(out[i])) * this_C1 + this_C2;
  }
  return out;
}

inline char toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

AnsiString HexStr(AnsiString in) {
  AnsiString out = "";

  int p=1;
  int l=in.Length();
  while(true) {
    if(p>l) break;

    unsigned char c = in[p];
    out += toHex(c>>4);
    out += toHex(c%16);
    p++;
  }
  return out;
}

AnsiString HexStrToStr(AnsiString in) {
  AnsiString res = "";
  for(int i=0;i<in.Length()/2;i++) {
	AnsiString asci = "0xDD";
	asci[3] = in[i*2+1];
	asci[4] = in[i*2+2];
	res = res + char(StrToInt(asci));
  }
  return res;
}
AnsiString cEncoder::encode(AnsiString value) {
  return HexStr(enc_Encrypt(value,Key));
}

AnsiString cEncoder::decode(AnsiString value) {
  return enc_Decrypt(HexStrToStr(value),Key);
}

#pragma package(smart_init)
