//---------------------------------------------------------------------------

#ifndef cprofilerH
#define cprofilerH
//---------------------------------------------------------------------------
#include <inifiles.hpp>
#pragma hdrstop

#define PSTARTP(Section,Parent) profiler.start(Section,Parent);
#define PSTART(Section) profiler.start(Section,"");
#define PEND(Section) profiler.stop(Section);
#define PSAVE profiler.save(ExtractFileDir(Application->ExeName)+"\\profile.txt");

//THIS IS SPECIFIC FOR EVERY PC
//668 internal ticks equal 1000 miliseconds
#define PCVALUE 2670

struct TimeStamp {
  unsigned int Hi;
  unsigned int Lo;
public:
  unsigned int MyStamp() {
    return (Hi<<12) + (Lo>>20);
  }
};

TimeStamp rdtsc(void) {
  TimeStamp res;
  DWORD h,l;
  asm {
    db 0x0F; db 0x31;
    mov l, eax
    mov h, edx
  }
  res.Hi = h;
  res.Lo = l;
  return res;
}


struct SectionRecord {
  AnsiString parent;
  TimeStamp start;
  DWORD whole;
  int count;
};

class cProfiler {
  TStringList* sections;
public:
  bool doprofile;
  cProfiler() {
    doprofile = true;
    sections = new TStringList;
    sections->CaseSensitive = true;
  }
  ~cProfiler() {
    for(int i=sections->Count-1;i>=0;i--)
      delete (SectionRecord*)sections->Objects[i];
    delete sections;
  }
  void start(AnsiString section,AnsiString parent) {
    if(!doprofile) return;
    int i;
    bool found = sections->Find(section,i);
    if(!found) {
       sections->Add(section);
       sections->Sort();
       sections->Find(section,i);
       sections->Objects[i] = (TObject*)new SectionRecord;
       ((SectionRecord*)sections->Objects[i])->whole = 0;
       ((SectionRecord*)sections->Objects[i])->count = 0;
       ((SectionRecord*)sections->Objects[i])->parent = parent;
    }
    ((SectionRecord*)sections->Objects[i])->start = rdtsc();
  }
  void stop(AnsiString section) {
    if(!doprofile) return;
    TimeStamp EndTime = rdtsc();
    int i;
    bool found = sections->Find(section,i);
    if(!found) {
      return;
    }
    SectionRecord* sr = (SectionRecord*)sections->Objects[i];
    sr->whole += EndTime.MyStamp() - sr->start.MyStamp();
    sr->count++;
  }
  void save(AnsiString filename) {
    if(!doprofile) return;
    TIniFile* ini = new TIniFile(filename);

    for(int i=0;i<sections->Count;i++) {
      SectionRecord* sr = (SectionRecord*)sections->Objects[i];
      ini->WriteInteger(sections->Strings[i],"Whole",
      sr->whole*1000/PCVALUE);
      ini->WriteString(sections->Strings[i],"Average",
       FloatToStrF(   ((float)sr->whole*1000/PCVALUE)/sr->count,ffFixed,10,3 )+"ms");
      ini->WriteInteger(sections->Strings[i],"Count",sr->count);
      if(sr->parent!="") {
        int pindex;
        bool found = sections->Find(sr->parent,pindex);
        if(!found) {
          ini->WriteString(sections->Strings[i],"ParentTime",sr->parent+":error (not found)");
        } else {
          SectionRecord* pr = (SectionRecord*)sections->Objects[pindex];
          ini->WriteString(sections->Strings[i],"ParentTime",
            sr->parent+":"+FloatToStrF( (float)sr->whole*100/pr->whole,ffFixed,10,2) + "%");
        }
      }
    }

    delete ini;
  }
};

extern cProfiler profiler;
#endif
