//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop

#include "myprofiler.h"

//---------------------------------------------------------------------------
  void cTimer::Start() {
   gettime(&start);              
  }
  void cTimer::Stop() {
   gettime(&end);
  };
  int cTimer::Milliseconds() {
    return end.ti_hour*60*60*1000+end.ti_min*60*1000+end.ti_sec*1000+end.ti_hund * 10 -
             (start.ti_hour*60*60*1000+start.ti_min*60*1000+start.ti_sec*1000+start.ti_hund * 10);
  };

#pragma package(smart_init)

