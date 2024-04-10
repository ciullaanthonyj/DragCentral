//---------------------------------------------------------------------------

#ifndef mymathH
#define mymathH
#include <math.h>
//---------------------------------------------------------------------------
class cCurve {
public:
  void Add(float x, float y);
  float xtoy(float xval);
  cCurve();
  void Clear();
private:
  float x[100];
  float y[100];
  int count;
};

void rotate(int& x,int& y, int cx, int cy, float angle);
float mod(float a);
inline int sign(float a) {if(a<0) return -1; else return 1;}

#endif
