#include "ofApp.h"
#define CANT 2

float factor[CANT] = {0.7, 0.3};
float x[CANT] = { 0.4 , 0.8 };
float y[CANT] = { 0.5 , 0.6 };
float ang[CANT] = { -10, 50 };

//--------------------------------------------------------------
void ofApp::setup()
{
   ofEnableSmoothing();
   //setLineWidth(1);
   ofSetColor(0,0,255,50);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
   ofBackground(0);
   fractal(7,400,300);
}

void ofApp::fractal(int nivel, float ancho, float alto)
{
    if ( nivel > 0 )
    {
        float xx = (float) ofMap(ofGetMouseX(),0,ofGetWidth(),0.13, 0.91,true);
        float yy = (float) ofMap(ofGetMouseY(),0, ofGetHeight(), 0.13, 0.91, true);

      for ( int i = 0 ; i < 5; i++ )
      {
         ofPushMatrix();
         ofTranslate( ancho*x[i]*-xx, alto*y[i]*-yy );
         ofRotateZ( ang[i] );
         ofCircle( 0, 0, ancho*factor[i], alto*factor[i] );
         ofTranslate( ancho*factor[i]*-0.5, alto*factor[i]*-0.5 );
         fractal( nivel-1, ancho*factor[i], alto*factor[i] );
         ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
