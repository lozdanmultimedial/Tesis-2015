 #include "universo.h"
 #include <math.h>
 float transparencia;
 bool flag = true;

 universo::universo()
{
   rotRate = 0.01;
   //d = sqrt((ofGetWidth() * ofGetWidth()) + (ofGetHeight() * ofGetHeight()))/2;
   divisor = 2;
   factor = -1;
   transparencia = 30;
}

void universo::dibujarCirculo(float x, float y, float d, int nivel)
{
    //transparencia = (int)abs(x - y);
    ofSetColor(x/2, y/2, d/2,transparencia);
    ofFill();
    ofCircle(x,y,d);

    if( d > 2)
    {
        dibujarCirculo(x + d/divisor * cos(pow( -factor,nivel)* ofGetFrameNum()* rotRate), y + d/divisor * sin(pow( factor,nivel)* ofGetFrameNum()* rotRate),d/2,nivel -1 );
        dibujarCirculo(x - d/divisor * cos(pow( -factor,nivel)* ofGetFrameNum()* rotRate), y - d/divisor * sin(pow( factor,nivel)* ofGetFrameNum()* rotRate),d/2,nivel -1 );
        divisor = fmod(divisor + 0.013,1.3);
        factor =  fmod(factor + 0.013,1.3);


    }


}
