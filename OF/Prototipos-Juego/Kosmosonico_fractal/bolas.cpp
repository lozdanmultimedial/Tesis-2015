#include "bolas.h"

bolas::bolas()
{
    r = ofRandom(0.1,1);
    xx = ofGetWidth()/2;
    yy = ofGetHeight()/2;
    vX = ofRandom(1,3);
    vY = ofRandom(1,3);
    dirX = ofRandom(360);
    dirY = ofRandom(360);
    ofSetColor(255);
}// end constructor

void bolas::actualizar()
{
    xx+= vX * cos(dirX);
    yy+= vY * sin(dirY);
    r+= 0.0001;

    if( xx < 0 || xx > ofGetWidth() || yy < 0 || yy > ofGetHeight())
    {
        r = ofRandom(0.1,0.5);
        xx = ofGetWidth()/2;
        yy = ofGetHeight()/2;
    }
}

    void bolas::dib()
    {
        ofSetColor(255);
        ofFill();
        ofCircle(xx,yy,r);
    }



