#include "ofApp.h"
#include "ofxFFTLive.h"
#include <cmath>

//--------------------------------------------------------------
void ofApp::setup()
{
   ofBackground(0);

    fftLive.setMirrorData(false);
    fftLive.setup();
     int bufferSize = 256;


	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);

	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update()
{
   for ( int i = 0; i < CANT; i++)
   {
      planeta[i].actualizar();
   }

   fftLive.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
   for( int i = 0; i < CANT; i++)
       {

           planeta[i].dib();
       }

       fftLive.draw(30, 30);
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{


	for (int i = 0; i < bufferSize; i++)
    {
		left[i]		= input[i*2]*2;
		right[i]	= input[i*2+1]*2;
	}
}

