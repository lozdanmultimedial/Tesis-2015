#include "ofApp.h"
#include <math.h>

 Stack::Stack()
 {
   counter = 0;

 }

 void Stack::push(double f)
  {
    val[counter++] = f;
  }

 double Stack::pop()
  {
    val[counter--] = 0;
    return val[counter];
  }

  int Stack::size()
  {
    return counter;
  }

