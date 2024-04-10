//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop

#include "cfindwindow.h"

bool found = false;
AnsiString word1 = "";
AnsiString word2 = "";
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam) {
 char caption[255];
 GetWindowText(hwnd,caption,sizeof(caption)-1);
 AnsiString cap = caption;
 if((word1=="" || cap.LowerCase().Pos(word1)!=0) &&
	(word2=="" || cap.LowerCase().Pos(word2)!=0))
   found = true;
 if(found) return false; //stop searching
 return true;
};
//---------------------------------------------------------------------------
cDesktopWindows DesktopWindows;
bool cDesktopWindows::Find(AnsiString lowerword1,AnsiString lowerword2){
  found = false;
  word1 = lowerword1;
  word2 = lowerword2;
  EnumWindows(WNDENUMPROC(EnumWindowsProc),0);
  return found;
}




//---------------------------------------------------------------------------

#pragma package(smart_init)
