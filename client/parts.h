//---------------------------------------------------------------------------

#ifndef partsH
#define partsH
//---------------------------------------------------------------------------
#include "chatserver.h"

class cParts {
public:
  cChatServer* s;

  AnsiString GetList(AnsiString usercarid) {
     AnsiString res = s->Query(encoder.decode("8E512DBE889F487A10") /*"parts.php"*/,
       encoder.decode("9972BE02FAFE4C0FBA941E6080D148DAA5") /*"getlist=1&userid="*/+s->userid+
       encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid);
     return res;
  }

  AnsiString GetCarParts(AnsiString usercarid) {
    return
      s->Query(encoder.decode("8E512DBE889F487A10") /*"parts.php"*/,
        encoder.decode("9972BE0D1B275E7E455198474C7E4779465AE46E6A") /*"getcarparts=1&userid="*/+s->userid+
        encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid);
  }
  // returns balance
  void BuyPart(AnsiString usercarid, AnsiString partid) {
    AnsiString res = s->Query(encoder.decode("8E512DBE889F487A10") /*"parts.php"*/,
       encoder.decode("9FB8D1C404501AFC61B741B8F0032983FA89BD") /*"act=buypart&userid="*/+s->userid+
       encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid+
       encoder.decode("D8EF653E6FA9C342") /*"&partid="*/+partid);
    if(s->Stack.Stacking) return;
    if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res)) throw Exception(res);
  }
  void InstallPart(AnsiString usercarpartid,int install) {
    AnsiString res = s->Query(encoder.decode("8E512DBE889F487A10") /*"parts.php"*/,
       encoder.decode("97B814CDAF2612B3F1C54C556F6981CCF0013E023E") /*"installpart=1&userid="*/+s->userid+
       encoder.decode("D8EA92A7419A267745883724923806") /*"&usercarpartid="*/+usercarpartid+
       encoder.decode("D8F606B5C20C1DB016") /*"&install="*/+IntToStr(install));
    if(s->Stack.Stacking) return;
    if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res)) throw Exception(res);
  }
};


extern cParts Parts;


#endif
