#include "ofApp.h"
#include <math.h>


 //ofImage val[CTD];
 vector<ofImage> vl;
 int cont;

ImgStack::ImgStack()
{
  cont = 0;
}

void ImgStack::push(ofImage f) {
    vl.push_back(f);
    cont++;
  }

  ofImage ImgStack:: pop() {
      cont--;
    vl.pop_back();
    return vl[cont];
  }

  int ImgStack::size() {
    return cont;
  }
