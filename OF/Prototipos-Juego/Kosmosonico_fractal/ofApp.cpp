#include "ofApp.h"
#include "ofxFFTLive.h"
#include <cmath>

#define CANT 2

float factor[CANT] = {0.7, 0.3};
//float x[CANT] = { 0.4 , 0.8 };
//float y[CANT] = { 0.5 , 0.6 };
float ang[CANT] = { -10, 50 };

float x1 = 0.4;
float x2 = 0.8;
float yy1 = 0.5;
float yy2 = 0.6;
int cont = 1;

//--------------------------------------------------------------
void ofApp::setup()
{
   ofEnableSmoothing();
   //setLineWidth(1);




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
   for ( int i = 0; i < CANT_P; i++)
   {
      planeta[i].actualizar();
   }

   fftLive.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
   ofBackground(0);



   for( int i = 0; i < CANT_P; i++)
       {

           planeta[i].dib();
       }



       fftLive.draw(30, 30);

       fractal(5,400,300);

}

void ofApp::fractal(int nivel, float ancho, float alto)
{

    if ( nivel > 0 )
    {
        float xx = (float) ofMap(ofGetMouseX(),0,ofGetWidth(),0.13, 0.91,true);
        float yy = (float) ofMap(ofGetMouseY(),0, ofGetHeight(), 0.13, 0.91, true);
        ofFill();

      for ( int i = 0 ; i < 5; i++ )
      {
         ofPushMatrix();
         if( cont/ 2 == 0)
         {
            ofTranslate( ancho * x1 *-xx, alto *yy1 *-yy );
         }
         else{ofTranslate( ancho * x2 *-xx, alto *yy2 *-yy ); }
         cont++;

         ofRotateZ( ang[i] );
          ofSetColor(0,0,255,4);
         ofCircle( 0, 0, ancho*factor[i], alto*factor[i] );
         ofTranslate( ancho*factor[i]*-0.31, alto*factor[i]*-0.31 );
         fractal( nivel-1, ancho*factor[i], alto*factor[i] );
         ofPopMatrix();
        }
        cont = 0;
    }
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{


	for (int i = 0; i < bufferSize; i++)
    {
		left[i]		= input[i*2]*2;
		right[i]	= input[i*2+1]*2;
	}
}

