#pragma once

#include "ofMain.h"
#include "bolas.h"
#include "ofxFFTLive.h"
#define CANT 400

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


		bolas planeta[CANT];

		float centroX, centroY;
		int lado;
        float x; float y;
        float dirX, dirY;
		float velX, velY;

		 ofxFFTLive fftLive;
         ofSoundStream soundStream;


        void audioIn(float * input, int bufferSize, int nChannels);

		vector <float> left;
		vector <float> right;
};
