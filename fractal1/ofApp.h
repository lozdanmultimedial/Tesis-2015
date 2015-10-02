#pragma once

#include "ofMain.h"
#include "Stack.h"
#include "ImgStack.h"

class ofApp : public ofBaseApp{

	public:
		void setup();

		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		ofColor colorFor(double a);
		void resetCam();
		void setCam(double xmi, double xma, double ymi, double yma);
		void setCamBoundaries(double xmi, double xma, double ymi, double yma);
        void setCamBoundaries(double xmi, double xma, double ymi, double yma, bool autoRender);
        void renderizar();
        void setPixAt(double x, double y, int c);
        double round(double val, int digit);
        int xNot(double x);
        int yNot(double y);
        double xCoord(double x);
        double yCoord(double y);
        void solveFor(double x, double y) ;
        double mean(vector<double> arrai, int endd);
        double sum(vector<double> arrai, int endd);
        double deviation(vector<double> arrai, int endd);

		double xMin, xMax, xCamDif, xIncr;
        int     xDecPrec;
        double  yMin,         yMax, yCamDif, yIncr;
        int     yDecPrec;

        bool    aboutToRender =     false;
        float camZoom = 10; //zoom ratio

double sizeRatio;
int iter;

int xalign ;
int yalign ;

Stack xMinStack;
Stack xMaxStack;
Stack yMinStack;
Stack yMaxStack;

ImgStack renderStack;
};
