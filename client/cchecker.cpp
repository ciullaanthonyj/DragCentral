//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop

#include "cchecker.h"

long filesize(FILE *stream)
{
   long curpos, length;

   curpos = ftell(stream);
   fseek(stream, 0L, SEEK_END);
   length = ftell(stream);
   fseek(stream, curpos, SEEK_SET);
   return length;

}

int cChecker::CheckFile(AnsiString filename)
 {

      FILE* f = fopen(filename.c_str(),"rb");
      char* buf;

      int fsize = filesize(f);
      buf = new char[fsize];

      int bread = fread(buf,1,fsize,f);
      fclose(f);

      int res = 0;
      int key = 0;
      for(int i=0;i<bread;i++) {
        unsigned char c = buf[i];
        res += c ^ key + c ^ i + 1;
        key = key + c;
      }
      delete buf;
      return res;

  };
  
cChecker checker;

//---------------------------------------------------------------------------

#pragma package(smart_init)
