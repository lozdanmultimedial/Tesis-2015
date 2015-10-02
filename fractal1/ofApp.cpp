#include "ofApp.h"
#include <math.h>
#include <algorithm>

#define CANT 500

int maxIter ;
double valsX  [CANT];
double valsY  [CANT];
double xtemp;
double cx, cy;
double logg2;
int t;
double val;
double mx, my;
double xPressed, yPressed;
bool draggedd;
vector<double> arrai;
double iCam = 2.5; //initial zoom
 ofImage render;




//--------------------------------------------------------------
void ofApp::setup()
{
  logg2 = log2(2);
  maxIter = CANT;
  sizeRatio = (double)ofGetWidth()/ofGetHeight();

  render.allocate(ofGetWidth(),ofGetHeight(),OF_IMAGE_COLOR);
  resetCam();
  render.getPixels();

}

//--------------------------------------------------------------


//--------------------------------------------------------------
void ofApp::draw()
{
  //ofImage(render, 0, 0);
  render.draw(0,0);
  ofColor rgb = render.getColor(ofGetMouseX(), ofGetMouseY());
  int r = (rgb.getHex() & 0xFF0000) >> 16;
  int g = (rgb.getHex() & 0x00FF00) >> 8;
  int b = (rgb.getHex() & 0x0000FF);
  ofSetColor((255-r)*1.5, (255-g)*1.5, (255-b)*1.5);
  if(draggedd == true)
  {

    ofRectMode(OF_RECTMODE_CORNER);
    ofNoFill();
    ofSetColor(175, 200, 75);
    ofRect((float)xPressed, (float)yPressed, (float)ofGetMouseX(), (float)ofGetMouseY());
  }
  render.update();
}
ofColor ofApp::colorFor(double a) {
  return ofColor( (float)(196+(cos(6.4*a/25)*128)*(1-128.0/255)), (float)(64+64+(cos(6.4*a/12.5)*64)*(1-196/255)), (float)(64+196*sin(a/100)) );
}

void ofApp::resetCam() { setCam(-iCam, iCam, -iCam, iCam); }

void ofApp::setCam(double xmi, double xma, double ymi, double yma)
{
  ofImage temp;
  if(xma-xmi <= 0 | yma-ymi <= 0) return;
  xMinStack.push(xMin);
  xMaxStack.push(xMax);
  yMinStack.push(yMin);
  yMaxStack.push(yMax);
  temp.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
  temp.getPixels();
  temp.clone(render);
  temp.update();
  renderStack.push(temp);
  double dif = (xma-xmi)/sizeRatio/2;
  double midY = (yma+ymi)/2;
  setCamBoundaries(xmi, xma, midY - dif, midY + dif);
}// end setCam

void ofApp::setCamBoundaries(double xmi, double xma, double ymi, double yma) {
  setCamBoundaries(xmi, xma, ymi, yma, true);
}

void ofApp::setCamBoundaries(double xmi, double xma, double ymi, double yma, bool autoRender)
 {
  xMin = xmi;
  xMax = xma;
  xCamDif = (xMax-xMin);
  xIncr = xCamDif/ofGetWidth();
  xDecPrec = (int)log10(1 / xCamDif ) + 2;

  yMin = ymi;
  yMax = yma;
  yCamDif = (yMax-yMin);
  yIncr = yCamDif/ofGetHeight();
  yDecPrec = (int)log10( 1 / yCamDif ) + 2;

  if(autoRender) renderizar();
}

double x, y;
void ofApp::renderizar()
{
     //  int thisCount = ++renderCount;
     //  println("("+xMin+", "+xMax+"), ("+yMin+", "+yMax+")");
  render.draw( 0, 0);
  if(aboutToRender)
  {
    ofSetColor(255);
    ofFill();

    aboutToRender = false;
    return;
  }
  render.getPixels();
  for(x = xMin+xIncr/2; x < xMax; x += xIncr)
  {
    for(y = yMin+yIncr/2; y < yMax; y += yIncr)
    {
      solveFor(x, y);
//      if(thisCount != renderCount) { println("broke from render "+thisCount); return; }
    }
  }
  render.setUseTexture(false);
//  println("Finished render "+thisCount+" -- renderCount is now on "+renderCount);
}

void ofApp::setPixAt(double x, double y, int c)
{

      //render.pixels[yNot(y)*ofGetWidth()+xNot(x)] = c; :: processing
       //render.getPixelsRef()[yNot(y)*ofGetWidth()+xNot(x)] = c;
       //render.reloadTexture();
       ofPixels px =render.getPixelsRef();
       px.setColor(x,y,c);
}

double ofApp::round(double val, int digit) {
  return floor(val * pow(10, digit) + .5)/pow(10, digit);
}

int ofApp::yNot(double y) {
  return (int)((y-yMin)/yIncr);
}

double ofApp::xCoord(double x) {
  return x*xIncr+xMin;
}

double ofApp::yCoord(double y) {
  return y*yIncr+yMin;
}

int ofApp::xNot(double x) {
  return (int)((x-xMin)/xIncr);
}


void ofApp::solveFor(double x, double y)
{
  iter = 0;
  cx = x;
  cy = y;
  while(x*x + y*y <= 4 & iter < maxIter)
  {
    xtemp = x*x - y*y + cx;
    y = 2*x*y + cy;
    x = xtemp;
    valsX[iter] = x*x;
    valsY[iter] = y*y;
    iter++;
  }
  if(iter >= maxIter)
  {
    setPixAt(cx, cy, 000000);
  }
  else
  {
    //Comment out this code if you change how val is set
    for(t = 0 ; t < 3; t++)
    {
      xtemp = x*x - y*y + cx;
      y = 2*x*y + cy;
      x = xtemp;
    }
    val = iter+1-(log2(log2(sqrt(x*x+y*y))))/logg2;
    //setPixAt(cx, cy, colorFor(val)); processing
    setPixAt(cx, cy, val);
  }
}

double ofApp::mean(vector<double> arrai, int endd) {
  return sum(arrai, endd)/endd;
}

double ofApp::sum(vector<double> arrai, int endd) {
  if(endd > arrai.size()) endd = arrai.size();
  double k = 0;
  for(int a =0 ; a < endd; a++) {
    k += arrai[a];
  }
  return k;
}

double ofApp::deviation(vector<double> arrai, int endd) {
  double k = mean(arrai, endd);
  for(int a = 0; a < endd; a++) {
    arrai[a] -= k;
  }
  for(int a = 0; a < endd; a++) {
    arrai[a] *= arrai[a];
  }
  return sqrt(mean(arrai, endd));
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
   if(button == 0)//LEFT
  {
    xPressed = ofGetMouseX();
    yPressed = ofGetMouseY();
    draggedd = true;
  }
  else if(button == 1)//RIGTH
  {
    if(xMinStack.size() > 1)
    {
      setCamBoundaries(xMinStack.pop(), xMaxStack.pop(), yMinStack.pop(), yMaxStack.pop(), false);
      render = renderStack.pop();
    }
  }
  if(button == 0 && draggedd == true)
  {
    draggedd = false;
    setCam(xCoord(ofGetMouseX())-xCamDif*.5, xCoord(ofGetMouseX())+xCamDif*.5, yCoord(ofGetMouseY())-yCamDif*.5, yCoord(ofGetMouseY())+yCamDif*.5);
  }


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
   if(button == 0 && xPressed-x != 0 && yPressed-y != 0)
  {
    draggedd = false;
    int xmn;
    int xmx;
    int ymn;
    int ymx;

    if(xPressed < x)
    {
        xmn = xPressed;
        xmx = x;
    }
    else
    {
        xmn = x;
        xmx = xPressed;
    }

    if(yPressed < y)
    {
        ymn = yPressed;
        ymx = y;
    }
    else
    {
      ymn = y;
      ymx = yPressed;
    }

    setCam(xCoord(xmn), xCoord(xmx), yCoord(ymn), yCoord(ymx));
  }
}
