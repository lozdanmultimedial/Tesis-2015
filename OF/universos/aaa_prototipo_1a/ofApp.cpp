#include "ofApp.h"

//provisoriamente:
float d,a,x,y;

//--------------------------------------------------------------
void ofApp::setup()
{
   ofSetBackgroundAuto(false);
   d = sqrt((ofGetWidth() * ofGetWidth()) + (ofGetHeight() * ofGetHeight()))/128;
   a = 0;
   x = ofGetWidth()/2;
   y = ofGetHeight()/2;

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(0,5);
    ofFill();
    ofRect(0,0,ofGetWidth(), ofGetHeight());


    u.dibujarCirculo( x  + 200* cos(a), y + 150 *sin(a), d ,10);
    x += 0.01;
    y += 0.01;

    a += 0.0001;


}
