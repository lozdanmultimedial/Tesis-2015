#ifndef _OF_STACK
#define _OF_STACK

#include "ofMain.h"
#define CT 1000

class Stack
{
   public:

       void push(double f);

       double val[CT];
       double pop();

       int size();
        int counter;


       Stack();
};

#endif
