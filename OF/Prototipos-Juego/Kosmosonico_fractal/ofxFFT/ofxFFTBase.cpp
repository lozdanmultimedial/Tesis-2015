/*
 *  AudioAbstract.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 
 /*Este archivo es una modificación del código homonimo original. Las modificaciones grafican en pantalla
 *un cubo que se desplaza, su dirección está determinada por las coordenadas x,y del cursor del mouse
 *y la aceleración depende de la intensidad del sonido captado en tiempo real.
 *LAs líneas comentadas del código original no cunplen función aquí y en un futuro serán eliminadas.
 *El código original pertenece a la librería para openFramework: ofxFFT https://github.com/julapy/ofxFFT
 */
 // Lozdan Claudio me fecit in 16-Junio-2015 

#include "ofxFFTBase.h"
#include "fft.h"
#include "ofApp.h"
#include "ofMain.h"
#include <cmath>


ofxFFTBase::ofxFFTBase()
{
    _fft = NULL;
    buffer = NULL;
    magnitudes = NULL;
    magnitudesDB = NULL;
    phases = NULL;
    window = NULL;

    setMaxDecay(0.995);
    setPeakDecay(0.96);
    setThreshold(0.5);
    setMirrorData(false);

    renderBorder = 1;

    bufferSize = 512; // default.
    binSize = (int)(bufferSize * 0.5);

    r = ofRandom(5,15);
    xx = ofRandom(r * 4, ofGetWidth() - r * 4);
    yy = ofRandom(r * 4, ofGetHeight() -r * 4);

	dirX = ofRandom(-3,3);
	dirY = ofRandom(-3,3);


    initFFT();
    initAudioData(fftData, binSize);
}

ofxFFTBase::~ofxFFTBase() {
    killFFT();
}

void ofxFFTBase::setup()
{
   /*
   r = ofRandom(5,15);
   xx = ofRandom(r * 4, ofGetWidth() - r * 4);
   yy = ofRandom(r * 4, ofGetHeight() -r * 4);
   vX = ofRandom(1,5);
	vY = ofRandom(1,5);
	dir = ofRandom(360);
 */
}

void ofxFFTBase::audioIn(float * data) {
    memcpy(buffer, data, sizeof(float) * bufferSize);
}

void ofxFFTBase::update()
{

#if defined(__APPLE_CC__) && !defined(_NO_VDSP)
    _fft->powerSpectrum_vdsp(0, buffer, window, magnitudes, phases);
//    _fft->convToDB_vdsp(magnitudes, magnitudesDB);
#else
    _fft->powerSpectrum(0, buffer, window, magnitudes, phases);
//    _fft->convToDB(magnitudes, magnitudesDB);
#endif

    updateAudioData(fftData, magnitudes);

    if(bMirrorData)
    {
        mirrorAudioData(fftData);
    }
}

//////////////////////////////////////////////////////
// FFT.
//////////////////////////////////////////////////////

void ofxFFTBase::initFFT() {
    killFFT();

    _fft = new fft(bufferSize);

    buffer = (float *)malloc(bufferSize * sizeof(float));
    memset(buffer, 0, bufferSize * sizeof(float));

    magnitudes = (float *)malloc(binSize * sizeof(float));
    memset(magnitudes, 0, binSize * sizeof(float));

    magnitudesDB = (float *)malloc(binSize * sizeof(float));
    memset(magnitudesDB, 0, binSize * sizeof(float));

    phases = (float *)malloc(binSize * sizeof(float));
	memset(phases, 0, binSize * sizeof(float));

    window = (float *)malloc(bufferSize * sizeof(float));
    memset(window, 0, bufferSize * sizeof(float));
    fft::genWindow(3, bufferSize, window);
}

void ofxFFTBase::killFFT() {
    if(_fft) {
        delete _fft;
        _fft = NULL;
    }

    if(buffer) {
        delete[] buffer;
        buffer = NULL;
    }

    if(magnitudes) {
        delete[] magnitudes;
        magnitudes = NULL;
    }

    if(magnitudesDB) {
        delete[] magnitudesDB;
        magnitudesDB = NULL;
    }

    if(phases) {
        delete[] phases;
        phases = NULL;
    }

    if(window) {
        delete[] window;
        window = NULL;
    }
}

//////////////////////////////////////////////////////
// AUDIO DATA.
//////////////////////////////////////////////////////

void ofxFFTBase::initAudioData(ofxFFTData & audioData, int dataSize) {
    audioData.size = dataSize;

    audioData.data.resize(dataSize, 0);
    audioData.dataNorm.resize(dataSize, 0);
    audioData.dataMax.resize(dataSize, 0);
    audioData.dataPeak.resize(dataSize, 0);
    audioData.dataCut.resize(dataSize, 0);

    audioData.linearEQIntercept = 1.0;
    audioData.linearEQSlope = 0.0;
}
/////////////////////////////////////////////////////////////////

void ofxFFTBase::updateAudioData(ofxFFTData & audioData, float *dataNew)
{
    audioData.data.clear();
    audioData.data.resize(audioData.size, 0);

    for(int i=0; i<audioData.size; i++)
    {
        audioData.data[i] = dataNew[i];
    }

    audioData.peakValue = 0;
    audioData.peakAverage = 0;

    for(int i=0; i<audioData.size; i++)
    {
        if(audioData.peakValue < audioData.data[i])
        {
            audioData.peakValue = audioData.data[i];
    //cout <<"Pico = "<<audioData.peakValue<<endl;
    // devuelve  valor fijo = 256
        }
    }

    for(int i=0; i < audioData.size; i++)

        /////////////////////////////////////////////////////////////
    {
        float p = i / (float)(audioData.size - 1);

        float dataVal;
        dataVal = audioData.data[i]; // use magnitude for fft data.
        dataVal *= audioData.linearEQIntercept + p * audioData.linearEQSlope; // scale value.
        //cout<<"dataVal = "<<dataVal<<endl;
        //posible punta del hilo


        if(isinf(dataVal)) {
            ofLog(OF_LOG_ERROR, "ofxFFTBase::updateAudioData - audio data value is infinity.");
            audioData.peakValue = 0;
            return;
        }

        float dataPeakRatio;
        dataPeakRatio = dataVal / audioData.peakValue;
        //cout<<"Peak ratio = "<<dataPeakRatio  * 1000<<endl;

        float dataMaxVal;
        dataMaxVal = audioData.dataMax[i];

        if(dataMaxVal < dataVal)
        {
            dataMaxVal = dataVal;
        }
//cout<<"-   - - - - - - - - - - - - - - - "<<endl;
//cout<<"Max Val = "<<dataMaxVal * 1000<<endl;
//xx += dataMaxVal *1000 * cos(dir);
//yy += dataMaxVal * 1000 *sin(dir);


        float dataNormVal;
        dataNormVal = 0;
        if(dataMaxVal > 0)
        {
            dataNormVal = dataVal / dataMaxVal; // normalise data between 0 and 1.
        }

        if(dataVal < 0.1)
        {
            dataNormVal = 0;
        }

        dataMaxVal *= audioData.maxDecay; // decay the max value.
        //cout<<"¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨"<<endl;
        //cout<<"dataMaxVal = "<<dataMaxVal*1000<<endl;



        audioData.dataNorm[i] = dataNormVal;

        audioData.dataMax[i] = dataMaxVal;

        float dataPeakVal;
        dataPeakVal = audioData.dataPeak[i];



        dataPeakVal *= audioData.peakDecay; // decay peak value.

        if(dataPeakVal < dataNormVal) { // check if new peak.
            dataPeakVal = dataNormVal;
        }

        audioData.dataPeak[i] = dataPeakVal;

        audioData.peakAverage += dataPeakVal; // sum of all peaks.

        int dataCutVal; // switch data (on/off).
        if(dataPeakVal < audioData.cutThreshold) {
            dataCutVal = 1;
        } else {
            dataCutVal = 0;
        }

        audioData.dataCut[i] = dataCutVal;

        audioData.peakAverage /= audioData.size;




    //cout<<"     x = "<<xx<<"  y = "<<yy<<"   r ="<<r<<endl;
//ofSetColor(255,0,0);
    //ofFill();
    //ofCircle(xx,yy,r);


   }// end for
}//////////////////////////////////////

void ofxFFTBase::mirrorAudioData(ofxFFTData & audioData) {
    int audioDataSizeHalf;
    audioDataSizeHalf = (int)(audioData.size * 0.5);

    for(int i=0; i<audioDataSizeHalf; i++) {
        int j = audioData.size - 1;
        int k = j - i;

        audioData.dataNorm[k] = audioData.dataNorm[i];
        audioData.dataMax[k] = audioData.dataMax[i];
        audioData.dataPeak[k] = audioData.dataPeak[i];
        audioData.dataCut[k] = audioData.dataCut[i];
    }
}

void ofxFFTBase::resetAudioData(ofxFFTData & audioData) {
    audioData.data.resize(audioData.size);
    audioData.dataNorm.resize(audioData.size);
    audioData.dataMax.resize(audioData.size);
    audioData.dataPeak.resize(audioData.size);
    audioData.dataCut.resize(audioData.size);
}

//////////////////////////////////////////////////////
// SETTERS / GETTERS.
//////////////////////////////////////////////////////

void ofxFFTBase::setBufferSize(int value) {
    int bufferSizeNew = ofNextPow2(value);

    if(bufferSize == bufferSizeNew) {
        return;
    }

    bufferSize = bufferSizeNew;
    binSize = (int)(bufferSize * 0.5);

    killFFT();
    initFFT();
    initAudioData(fftData, bufferSize);
}

int ofxFFTBase::getBufferSize() {
    return bufferSize;
}

void ofxFFTBase::setThreshold(float value) {
    value = ofClamp(value, 0, 1);
    fftData.cutThreshold = value;
}

float ofxFFTBase::getThreshold() {
    return fftData.cutThreshold;
}

float ofxFFTBase::getAveragePeak() {
    return fftData.peakAverage;
}

void ofxFFTBase::setPeakDecay(float value) {
    value = ofClamp(value, 0, 1);
    fftData.peakDecay = value;
}

float ofxFFTBase::getPeakDecay() {
    return fftData.peakDecay;
}

void ofxFFTBase::setMaxDecay(float value) {
    value = ofClamp(value, 0, 1);
    fftData.maxDecay = value;
}

float ofxFFTBase::getMaxDecay() {
    return fftData.maxDecay;
}

void ofxFFTBase::setMirrorData(bool value) {
    bMirrorData = value;
}

//////////////////////////////////////////////////////
// GET VARIOUS DATA SAMPLES.
//////////////////////////////////////////////////////

const vector<float> & ofxFFTBase::getFftRawData() {
    return fftData.data;
}

const vector<float> & ofxFFTBase::getFftNormData() {
    return fftData.dataNorm;
}

const vector<float> & ofxFFTBase::getFftPeakData() {
    return fftData.dataPeak;
}

const vector<int> & ofxFFTBase::getGlitchData() {
    return fftData.dataCut;
}

void ofxFFTBase::getFftData(float * data, int length)
{
    for(int i=0; i<length; i++)
    {
        int j = (int)((i / (float)(length - 1)) * (binSize - 1));
        float v  = fftData.dataNorm[j];
        data[i] = v;

    }
}

void ofxFFTBase::getFftPeakData(float * data, int length) {
    for(int i=0; i<length; i++) {
        int j = (int)((i / (float)(length - 1)) * (binSize - 1));
        float v  = fftData.dataPeak[j];
        data[i] = v;

    }
}

void ofxFFTBase::getGlitchData(int * data, int length) {
    for(int i=0; i<length; i++) {
        int j = (int)((i / (float)(length - 1)) * (binSize - 1));
        float v  = fftData.dataCut[j];
        data[i] = v;
    }
}

//////////////////////////////////////////////////////
// DRAW.
//////////////////////////////////////////////////////

void ofxFFTBase::draw(int x, int y)
{


    draw(x, y, OFX_FFT_WIDTH, OFX_FFT_HEIGHT);

}

void ofxFFTBase::keyPressed(int key)
{
    tecla = key;
}

void ofxFFTBase::draw(int x, int y, int width, int height)
{
    renderSingleBandWidth = (width / (float)binSize) * 3;


    ofPushMatrix();

    ofTranslate(x, y);


    drawData(fftData, width, height);

    ofPopMatrix();
}

void ofxFFTBase::drawData(const ofxFFTData & audioData, int width, int height)
{
    drawBg(audioData, width, height);
    drawGlitchData(audioData, width, height);
    drawFftData(audioData, width, height);   // this is audio data before its been normalised, good for debugging.
    drawFftNormData(audioData, width, height);
    drawFftPeakData(audioData, width, height);
    drawThresholdLine(audioData, width, height);
    drawBorder(width, height);
}

void ofxFFTBase::drawBg(const ofxFFTData & audioData, int w, int h) {
    int bx, by; // border.
    bx = by = renderBorder;
/*
    ofPushStyle();
    ofFill();
    ofSetColor(240,0,0);
    //ofRect(bx, by, w, h);
    ofPopStyle();
    */
}

void ofxFFTBase::drawBorder(int w, int h) {
    int bx, by; // border.
    bx = by = renderBorder;
/*
    ofPushStyle();
    ofFill();
    ofSetColor(0,0,255);
    ofRect(0, 0, w + bx * 2, 1);
    ofRect(0, h + by, w + bx * 2, 1);
    ofRect(0, 0, 1, h + by * 2);
    ofRect(w + bx, 0, 1, h + by * 2);



    ofPopStyle();
    */


}

void ofxFFTBase::drawGlitchData(const ofxFFTData & audioData, int w, int h)
{
    int bx, by; // border.
    bx = by = renderBorder;
/*
    ofPushStyle();
    ofFill();
    ofSetColor(255,200,0);


    for(int i=0; i<binSize; i++)
    {
        ofRect(i * renderSingleBandWidth + bx,
               h + by,
               renderSingleBandWidth,
               -audioData.dataCut[i] * h);
//cout<< "audioData = "<< audioData.dataCut[i] * h << endl;
// arroja valor fijo = 256

    ofPopStyle();

*/
}

void ofxFFTBase::drawFftData(const ofxFFTData & audioData, int w, int h)
{

   //Estos son las bandas moviles celestes
    int bx, by; // border.
    bx = by = renderBorder;
/*
    ofPushStyle();
    for(int i=0; i<binSize; i++)
    {
        ofFill();
        ofSetColor(100,255,200);
        //ofRect(i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.data[i] * h);

        //ofNoFill();
        //ofSetColor(232);
        ofRect(i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.data[i] * h);
    }
    ofPopStyle();

     */
    float promedio = 0;
    for(int i = 0; i < binSize; i++)
    {
        promedio += audioData.data[i];
    }
    promedio /= binSize;

    dirX = ofRandom(promedio*100, promedio *1000);
    dirY = ofRandom(promedio*100, promedio *1000);
    promedio *=100;

    float mX = ofGetMouseX();
    float mY = ofGetMouseY();

    float dir = atan2( mY - yy, mX - xx );

    vX = (ofRandom(promedio , promedio*10));
    vY = (ofRandom(promedio , promedio*10));


    cout<<"mX = "<<mX<< endl;
    cout<<"my = "<<mY<< endl;
    cout<<"Dir = "<< dir<<endl;
    cout<<"dir + dirX = "<<dir +dirX<<endl;
    cout<<"dir + dirY = "<<dir + dirY<<endl;

     xx += vX * promedio * cos(dir);
     yy += vY * promedio * sin(dir);


     xx += vX * promedio * cos(dirX);
     yy += vY * promedio * sin(dirY);



    if(xx < r) {xx += ofGetWidth() - r;}

    if(xx > ofGetWidth() - r){xx = r; }

    if(yy < r){ yy += ofGetHeight() - r;}

    if(yy > ofGetHeight() - r) {yy = r;}

    cout<<"++++++++++++++++++++++++++++++++"<<endl;
    cout<<"promedio = "<< promedio << endl;
    cout<<"     x = "<<xx<<"  y = "<<yy<<"   r ="<<r<<endl;

    ofSetColor(255,89,25);
    ofNoFill();

    ofPushMatrix();
          ofTranslate(xx ,yy);

                ofDrawBox(r * 3);

        ofPopMatrix();
    //ofCircle(xx,yy,r);




}

void ofxFFTBase::drawFftNormData(const ofxFFTData & audioData, int w, int h) {
    int bx, by; // border.
    //bx = by = renderBorder;

/*
    ofPushStyle();
    for(int i = 0; i < binSize; i++)
    {
        ofFill();
        ofSetColor(100);
        ofRect(i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.dataNorm[i] * h);

        //ofNoFill();
        ofSetColor(70,232,200);
        ofRect(i * renderSingleBandWidth + bx * 2, h + by, renderSingleBandWidth * 2, -audioData.dataNorm[i] * h);
        //cout<<"############################################"<<endl;
        //cout<<"-audioData.data.Norm  = "<<-audioData.dataNorm[i] <<endl;

    }


    ofPopStyle();
*/




}

void ofxFFTBase::drawFftPeakData(const ofxFFTData & audioData, int w, int h) {

   //Estos son los rectangulos verdes
    int bx, by; // border.
    bx = by = renderBorder;

/*
    ofPushStyle();
    ofFill();
    ofSetColor(0,255,0);
    for(int i=0; i<binSize; i++) {
        float p = audioData.dataPeak[i];

        ofRect(i * renderSingleBandWidth + bx, (1 ) * (h - 2) + by, renderSingleBandWidth - 1, 15);

    }
    ofPopStyle();
    */
}

void ofxFFTBase::drawThresholdLine(const ofxFFTData & audioData, int w, int h) {
    int bx, by; // border.
    bx = by = renderBorder;
/*
    ofFill();
    ofSetColor(0);
    ofRect(bx, (1 - audioData.cutThreshold) * h + by - 1, w, 1);
    ofSetColor(255);
    ofRect(bx, (1 - audioData.cutThreshold) * h + by + 0, w, 1);
    ofSetColor(0);
    ofRect(bx, (1 - audioData.cutThreshold) * h + by + 1, w, 1);
    */
}
