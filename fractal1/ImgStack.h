#ifndef _OF_IMGSTACK
#define _OF_IMGSTACK

#include "ofMain.h"#

class ImgStack
{
  public:
  void push(ofImage f);
  ofImage pop();
  int size();

  ImgStack();
};
#endif
