#pragma once


#include "ofMain.h"
#include "ofxFFTBase.h"


class ofxFFTLive : public ofxFFTBase
{

public:

    ofxFFTLive();
    ~ofxFFTLive();

    void setup();
    void audioIn(float * input, int bufferSize, int nChannels);

    ofSoundStream * soundStream;
    float xx,yy,r,vX,vY,dir;
    //float dataMaxVal;
};
