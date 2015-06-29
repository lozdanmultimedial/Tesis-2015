#ifndef _OF_UNIVERSO
#define _OF_UNIVERSO

#include "ofMain.h"
//#include <math.h>

class universo
{
public:
   void iniciar();

   void dibujarCirculo(float x, float y, float d, int nivel);

   universo(); // constructor

   float rotRate ;
   float d,divisor,factor;

};


#endif // _OF_UNIVERSO
