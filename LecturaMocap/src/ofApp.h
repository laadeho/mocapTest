#pragma once

#include "ofMain.h"
#include "ofUtils.h"
#include <winsock2.h>
#include <vector>
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void coordenadas();

		void analizaCSV();
		void cargaCoords();
		void dibujaCoords();

		ofFile editado;

		/// GUI
		void setupGUI();
		ofxDatGui* gui;
		void onDropdownEvent(ofxDatGuiDropdownEvent e);
		void onSliderEvent(ofxDatGuiSliderEvent e);
		void onToggleEvent(ofxDatGuiToggleEvent e);
		void onTextInputEvent(ofxDatGuiTextInputEvent e);
		void on2dPadEvent(ofxDatGui2dPadEvent e);
		void onButtonEvent(ofxDatGuiButtonEvent e);
		string nombreUsuario = "";
		string emailUsuario = "";
		bool showGui;

		///
		vector <string> linea;
		vector <ofPoint> coords;
		vector <vector<ofPoint> > coordsFrames;

		vector<string> words2;
		vector<string> words5;
		vector<string> words6;
		vector<string> wordsFila;

		bool borra = true;
		float x = 0, y = 0, z = 0;

		string nombreArchivo;

		bool analiza = true; // (!analiza){dibuja}, (analiza){analizaCSV}
		int lineaAnalisis = 7;
		int saltoLinea = 1;
		int escala = 150;

		/// CAMARA
		ofEasyCam cam;
};
