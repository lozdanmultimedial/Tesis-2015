#pragma once

#include "ofMain.h"
#include "bolas.h"
#include "ofxFFTLive.h"
#define CANT_P 400


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void fractal(int nivel, float ancho, float alto);

        bolas planeta[CANT_P];

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
