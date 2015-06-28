//Este programa funciona en paralelo a "aaa_Tx_rx_circulos3"
// en ambos programas el fondo dinámico es generativo.
//
//Aquí se genera una animación autónoma de una esfera cuyas coordenadas x,y son
//transmitidas a "aaa_Tx_rx_circulos3" para que sea graficada allí.
//
//Recibe de "aaa_Tx_rx_circulos3" las coordenadas x,y del cubo controlado por el mouse y sonido
//para ser graficado aqui.


#include "ofApp.h"
#include <math.h>

float promedio;

//universo
float rotRate = 0.01;
float d,divisor,factor;




//--------------------------------------------------------------
void ofApp::setup()
{
  //we run at 60 fps!
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	 ofEnableSmoothing();
	 ofSetBackgroundAuto(false);

    //create the socket and bind to port 11999
	udpConnection.Create();
        // recibo
	udpConnection.Bind(11999);
	// transmito
	udpConnection.Connect("127.0.0.1",12000);

	udpConnection.SetNonBlocking(true);

	////Sonido

	int bufferSize = 512;


	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);

    promedio = 0;

    //// animacion local


    r = ofRandom(10,15);
	x = ofRandom(r, ofGetWidth() - r);
	y = ofRandom(r, ofGetHeight() - r);
    vX = ofRandom(0.5,0.99);
    vY = ofRandom(0.5,0.99);
	dX = ofRandom(360);
	dY = ofRandom(360);

	//universo

d = sqrt((ofGetWidth() * ofGetWidth()) + (ofGetHeight() * ofGetHeight()))/2;
 divisor = 2;
   factor = -1;


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{


    /// fondo
    ofSetColor(0,2);
    ofFill();
    ofRect(0,0,ofGetWidth(), ofGetHeight());

   ///universo
    dibujarCirculo( ofGetWidth()/2, ofGetHeight()/2, d - 20 ,10);

      divisor = fmod(divisor + 0.013,13);
      factor =  fmod(factor + 0.0013,13);

///Recibo data y dibujo
    char udpMessage[100000];
	udpConnection.Receive(udpMessage,100000);
	string message=udpMessage;
	if(message!="")
	{
		cout<<"mensaje = "<<message<<endl;
        ofSetColor(150,255,90);
        float a,b;
        int rj, vd, az;
		vector<string> strPoints = ofSplitString(message,"[/p]");
		for(unsigned int i=0;i<strPoints.size();i++)
        {
			vector<string> point = ofSplitString(strPoints[i],"|");
			if( point.size() == 5)
            {
				a = atof(point[0].c_str());
				b = atof(point[1].c_str());
				rj = atof(point[2].c_str());
				vd = atof(point[3].c_str());
				az = atof(point[4].c_str());
                 ofSetColor(rj,vd,az);
				ofPushMatrix();
				 ofTranslate(a,b);
				 ofBox(20);
				ofPopMatrix();
			}
		}

	}// end if message !=] ""


	// Animación autonoma local


	 vX = abs(promedio*100);
     vY = abs(promedio*100);
     //cout<<"vX = "<<vX<<"  vY = "<<vY<<endl;




	x += vX * cos(dX);
	y += vY * sin (dY);

	if(x < r) { x = ofGetWidth() - r;}
	if(x > ofGetWidth() - r ) { x = r;}
	if(y < r) { y = ofGetHeight() - r;}
	if(y > ofGetHeight() - r) { y = r;}
    cout<<"x = "<<x<<"  y = "<<y<< endl;
	ofPushMatrix();
	   ofTranslate(x,y);
	   ofSetColor(255,55,100);
	   ofFill();
	   ofSphere(r);
	 ofPopMatrix();

// transmitiendo

    string mensaje = "";

	mensaje += ofToString(x)+"|"+ofToString(y)+"|"+ofToString(r)+"[/p]";

	udpConnection.Send(mensaje.c_str(),mensaje.length());


}


//////// universo  fractal


void ofApp::dibujarCirculo(float x, float y, float d, int nivel)
{
    ofSetColor(x/2, y/2, d/2,20);
    ofNoFill();
    ofCircle(x,y,d);
    if( d > 2)
    {
        dibujarCirculo(x + d/divisor * cos(pow( -factor,nivel)* ofGetFrameNum()* rotRate), y + d/divisor * sin(pow( factor,nivel)* ofGetFrameNum()* rotRate),d/2,nivel -1 );
        dibujarCirculo(x - d/divisor * cos(pow( -factor,nivel)* ofGetFrameNum()* rotRate), y - d/divisor * sin(pow( factor,nivel)* ofGetFrameNum()* rotRate),d/2,nivel -1 );

    }
}




void ofApp::audioIn(float * input, int bufferSize, int nChannels){

	promedio = 0;

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++)
	{
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
		promedio += (left[i] + right[i])/2;

	}

	promedio /= bufferSize;



}
