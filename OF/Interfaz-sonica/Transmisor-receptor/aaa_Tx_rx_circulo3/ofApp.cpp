// este programa funciona en paraleo comunicandose enre si con "aaa_Rx_tx_Circulo3"
 //en ambos el fondo dinámico es generativo
 // aqui  hay un cubo cuyo movimiento se controla dándole su dirección con el puntero del mouse
 // y la acelaración con la intensidad de sonido capturado en tiempo real.
 // La data de las coordenadas x,y resultantes correspondientes al cubo son envidadas a "aaa_Rx_tx_Circulo3" para que allí 
 // se grafique el mismo cubo.
 //
 //El otro porgrama envía aquí las oocrdenadas correspondiente a una esfera generada alli para ser graficada aqui.
 
#include "ofApp.h"
#include <cmath>
#define RECONNECT_TIME 400

float x,y,r,vX, vY, dX, dY;
int rj, vd, az;

float rotRate = 0.01;
float d,divisor,factor;

float promedio;



//--------------------------------------------------------------
void ofApp::setup()
{
	// we don't want to be running to fast
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

    //create the socket and set to send to 127.0.0.1:11999
	udpConnection.Create();
        // envia
	udpConnection.Connect("127.0.0.1",11999);

	// recibo
	udpConnection.Bind(12000);
	//

	udpConnection.SetNonBlocking(true);
	r = ofRandom(10,30);
	x = ofRandom( r *2, ofGetWidth() - r *2);
	y = ofRandom( r * 2, ofGetHeight() - r* 2);

	rj = (int)ofRandom(255);
	vd = (int)ofRandom(80);
	az = (int)ofRandom(255);

	ofSetBackgroundAuto(false);
   d = sqrt((ofGetWidth() * ofGetWidth()) + (ofGetHeight() * ofGetHeight()))/2;
   ofEnableSmoothing();
   divisor = 2;
   factor = -1;

   //// sonido

   int bufferSize = 512;


	left.assign (bufferSize, 0.0);
	right.assign (bufferSize, 0.0);
	soundStream.setup (this, 0, 2, 44100, bufferSize, 4);

}

//--------------------------------------------------------------
void ofApp::update()
{
    // variables para el fondo de circulos
    divisor = fmod(divisor + 0.013,13);
    factor =  fmod(factor + 0.0013,13);

    promedio = 0;
}

//--------------------------------------------------------------
void ofApp::draw()
{
     ofSetColor(0,2);
    ofFill();
    ofRect(0,0,ofGetWidth(), ofGetHeight());

    dibujarCirculo( ofGetWidth()/2, ofGetHeight()/2, d - 20 ,10);

    float mX = ofGetMouseX();
    float mY = ofGetMouseY();


    float dir = atan2( mY - y, mX - x );
     //  aceleracion por intensidad de sonido
    vX = abs(promedio * 50);
    vY = abs(promedio * 50);
     //-------------------------------
    x += vX * cos(dir);
	y += vY * sin(dir);

    if(x < r) {x += ofGetWidth() - r;}

    if(x > ofGetWidth() - r){x = r; }

    if(y < r){ y += ofGetHeight() - r;}

    if(y > ofGetHeight() - r) {y = r;}

    // Objeto local

    ofFill();
	ofSetColor(rj,vd,az);

	ofPushMatrix();
	   ofTranslate(x,y);
	   ofBox(r);
	ofPopMatrix();

// envio de las coordenadas locales
	//cout<<"x = "<<x<<"   y = "<<y<<endl;
	string mensaje = "";

	mensaje += ofToString(x)+"|"+ofToString(y)+"|"+ofToString(rj)+"|"+ofToString(vd)+"|"+ofToString(az)+"[/p]";

	udpConnection.Send(mensaje.c_str(),mensaje.length());
	//cout<<"sms = "<<mensaje<<endl;
////////////////// fin envio


///// recibo y dibujo esfera

char udpMessage[100000];
	udpConnection.Receive(udpMessage,100000);
	string message=udpMessage;
	if(message!="")
	{
		cout<<"mensaje = "<<message<<endl;

        ofSetColor(255,55,90);

        float a,b,r;



    vector<string> strPoints = ofSplitString(message,"[/p]");
		for(unsigned int i=0;i<strPoints.size();i++)
        {
			vector<string> point = ofSplitString(strPoints[i],"|");
			if( point.size() == 3 )// vienen 3 datos
            {
				a = atof(point[0].c_str());
				b = atof(point[1].c_str());
                r = atof(point[2].c_str());

				ofPushMatrix();
				   ofTranslate(a,b);
				   ofSphere(r);
				ofPopMatrix();
			}
		}
    }// end if message != ""
}


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
		left[i]		= input[i*2]*2;
		right[i]	= input[i*2+1]*2;
		promedio += (left[i] + right[i])/2;

	}

	promedio /= bufferSize;


	//cout<<"Promedio = "<<promedio<< endl;

}
