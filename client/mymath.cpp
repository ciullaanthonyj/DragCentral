//---------------------------------------------------------------------------


#pragma hdrstop

#include "mymath.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


float mod(float a) {
  if(a>0) return a;
  else return -a;
}
int Round(float a) {
  float left = a - (int) a;
  if(a>0) {
    if (left>0.5) return ((int)a + 1);
    else return (int)a;
  } else
    return -Round(-a);

}

void rotate(int& x,int& y, int cx, int cy, float angle) {
  int mx = x-cx;
  int my = y-cy;
  int ox = mx;
  int oy = my;
  x = Round(cos(angle)*ox - sin(angle)*oy);
  y = Round(sin(angle)*ox + cos(angle)*oy);
  x = x+cx;
  y = y+cy;
}

void cCurve::Clear()
 { count = 0; }

cCurve::cCurve()
 { count = 0; }

float cCurve::xtoy(float xval) {
    if(count==0) return 0;
    if(xval<x[0])
      return y[0];
    int p = 0;
    while(true) {
      if(p>=count) break;
      if( (x[p]<=xval) && ( (p+1==count) || xval<=x[p+1] ) ) break;
      p++;
    }
    if(p+1==count)
      return this->y[p];

    float val = y[p] + ((xval - x[p]) / (x[p+1]-x[p])) * (y[p+1]-y[p]);
    return val;
  }

void cCurve::Add(float x, float y) {
    this->x[count] = x;
    this->y[count] = y;
    count++;
  }
