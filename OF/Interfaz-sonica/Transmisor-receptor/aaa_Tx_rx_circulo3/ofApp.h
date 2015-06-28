 
 // este programa funciona en paraleo comunicandose enre si con "aaa_Rx_tx_Circulo3"
 //en ambos el fondo dinámico es generativo
 // aqui  hay un cubo cuyo movimiento se controla dándole su dirección con el puntero del mouse
 // y la acelaración con la intensidad de sonido capturado en tiempo real.
 // La data de las coordenadas x,y resultantes correspondientes al cubo son envidadas a "aaa_Rx_tx_Circulo3" para que allí 
 // se grafique el mismo cubo.
 //
 //El otro porgrama envía aquí las oocrdenadas correspondiente a una esfera generada alli para ser graficada aqui.
 
 
 #pragma once

#include "ofMain.h"
#include "ofxNetwork.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
	 ofSoundStream soundStream;


        void audioIn(float * input, int bufferSize, int nChannels);

		vector <float> left;
		vector <float> right;	void draw();
		void dibujarCirculo(float x, float y, float d, int nivel);

		ofxUDPManager udpConnection;

		 ofSoundStream soundStream;


        void audioIn(float * input, int bufferSize, int nChannels);

		vector <float> left;
		vector <float> right;


};
