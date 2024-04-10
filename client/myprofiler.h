//---------------------------------------------------------------------------

#ifndef myprofilerH
#define myprofilerH
//---------------------------------------------------------------------------

class cTimer {
public:
  void Start();
  void Stop();
  int Milliseconds();
private:
  struct time start;
  struct time end;
};

#endif
