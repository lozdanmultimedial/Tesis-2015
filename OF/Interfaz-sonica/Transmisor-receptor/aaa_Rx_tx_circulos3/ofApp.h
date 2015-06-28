//Este programa funciona en paralelo a "aaa_Tx_rx_circulos3"
// en ambos programas el fondo dinámico es generativo.
//
//Aquí se genera una animación autónoma de una esfera cuyas coordenadas x,y son
//transmitidas a "aaa_Tx_rx_circulos3" para que sea graficada allí.
//
//Recibe de "aaa_Tx_rx_circulos3" las coordenadas x,y del cubo controlado por el mouse y sonido
//para ser graficado aqui.

#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"

class ofApp : public ofBaseApp
{

	public:
		void setup();
		void update();
		void draw();
		void dibujarCirculo(float x, float y, float d, int nivel);

		void audioIn(float * input, int bufferSize, int nChannels);

		ofxUDPManager udpConnection;

		vector <float> left;
		vector <float> right;

		ofSoundStream soundStream;

		//animacion local de la esfera
		float x,y,vX, vY, dX, dY, r;

};
