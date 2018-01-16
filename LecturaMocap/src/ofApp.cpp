#include "ofApp.h"
#include <winsock2.h>
#include <vector>
#include "ofUtils.h"

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

bool analiza = true; // analiza == false = dibuja, analiza = analiza csv
int lineaAnalisis = 7;
int saltoLinea = 1;
int escala = 150;

ofEasyCam cam;

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(4);
	ofSetBackgroundAuto(false);
	ofBackground(0);

	//nombreArchivo = "Fermin_manos_edited";
	nombreArchivo = "beto_mocap_fULL";
	ofFile archivo(nombreArchivo + ".csv");

	if (analiza) {
		analizaCSV();
	}
	else {
		if (!archivo.exists()) ofLogError("The file " + nombreArchivo + " is missing");
		else ofLogNotice("Archivo Encontrado: " + nombreArchivo + "\n Procesando...");
		
		ofBuffer buffer(archivo);
		for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
			linea.push_back(*it);
		}
		ofLogNotice("Lineas Totales (linea.size()): " + ofToString(linea.size()));
		for (int i = 0; i < 6; i++) {
			ofLogNotice("Size Linea " + ofToString(i) + " : " + ofToString(linea[i].size()));
		}
		ofLogNotice(ofToString(linea[2]));
		words2 = ofSplitString(linea[2], ",");
		ofLogNotice("words2: " + ofToString(words2.size()));

		// linea 5 comienza con los datos de Posicion y Rotacion, la 6 son las coordenadas xyz
		//ofLogNotice(ofToString(linea[5]));
		words5 = ofSplitString(linea[5], ",");
		ofLogNotice("words5: " + ofToString(words5.size()));
		//
		//ofLogNotice(ofToString(linea[6]));
		words6 = ofSplitString(linea[6], ",");
		ofLogNotice("words6: " + ofToString(words6.size()));
		//
		coords.clear();

		wordsFila = ofSplitString(linea[7], ",");
		for (int i = 0; i < words2.size()-1; i += saltoLinea) {
			if (words2[i] == "Bone") {
				if (words5[i] == "Position") {
					if (words6[i] == "X") {
						x = ofToFloat(wordsFila[i]) * escala;
					}
					else if (words6[i] == "Y") {
						y = ofToFloat(wordsFila[i]) * escala;
					}
					else if (words6[i] == "Z") {
						z = ofToFloat(wordsFila[i]) * escala;
					}
					coords.push_back(ofPoint(x, y, z));
				}
			}
		}
		
		//cargaCoords();
	}
	//coordenadas();
}

//--------------------------------------------------------------
void ofApp::update() {
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());
		
	if (analiza) {
		///
	}
	else {
		cargaCoords();
	}
}
void ofApp::coordenadas() {
	
}

void ofApp::analizaCSV(){
	/// DEFINE NOMBRE DE ARCHIVO CSV A ANALIZAR
	ofFile archivo(nombreArchivo + ".csv");
	if (!archivo.exists()) ofLogError("The file " + nombreArchivo + " is missing");
	else ofLogNotice("Archivo Encontrado: " + nombreArchivo + "\n Procesando...");
	/// Crea archivo a escribir con datos procesados
	editado.open(nombreArchivo + "_b_editado.csv", ofFile::WriteOnly);
	/// 
	ofBuffer buffer(archivo);
	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
		linea.push_back(*it);
	}
	ofLogNotice("Lineas Totales (linea.size()): " + ofToString(linea.size()));
	for (int i = 0; i < 5; i++) {
		ofLogNotice("Size Linea " + ofToString(i) + " : " + ofToString(linea[i].size()));
	}
	//ofLogNotice(ofToString(linea[3]));
	words2 = ofSplitString(linea[2], ",");
	ofLogNotice("words2: " + ofToString(words2.size()));

	// linea 5 comienza con los datos de Posicion y Rotacion, la 6 son las coordenadas xyz
	//ofLogNotice(ofToString(linea[5]));
	words5 = ofSplitString(linea[5], ",");
	ofLogNotice("words5: " + ofToString(words5.size()));
	//
	//ofLogNotice(ofToString(linea[6]));
	words6 = ofSplitString(linea[6], ",");
	ofLogNotice("words6: " + ofToString(words6.size()));
	//
	coords.clear();

	for (int i = 0; i < words2.size(); i += saltoLinea) {
		if (words2[i] == "Bone") {
			editado << "Bone,";
		}
	}
	editado << "\n";
	for (int i = 0; i < words2.size(); i += saltoLinea) {
		if (words2[i] == "Bone") {
			if (words5[i] == "Position") {
				editado << "Position,";
			}
		}
	}
	editado << "\n";
	wordsFila = ofSplitString(linea[6], ",");

	for (int i = 0; i < words2.size(); i += saltoLinea) {
		if (words2[i] == "Bone") {
			if (words5[i] == "Position") {
				if (words6[i] == "X") {
					editado << "X,";
					x = ofToFloat(wordsFila[i]) * escala;
				}
				else if (words6[i] == "Y") {
					editado << "Y,";
					y = ofToFloat(wordsFila[i]) * escala;
				}
				else if (words6[i] == "Z") {
					editado << "Z,";
					z = ofToFloat(wordsFila[i]) * escala;
				}
				coords.push_back(ofPoint(x, y, z));
			}
		}

	}
	editado << "\n";

	for (int j = 7; j<linea.size() / 10 - 1; j++) {
		wordsFila = ofSplitString(linea[j], ",");
		//wordsFila = ofSplitString(linea[7], ",");
		int num = 0;
		for (int i = 0; i < words2.size(); i += saltoLinea) {
			if (words2[i] == "Bone") {
				if (words5[i] == "Position") {
					if (words6[i] == "X") {
						x = ofToFloat(wordsFila[i]) * escala;
						num++;
						editado << ofToString(x, 2) << ",";
					}
					else if (words6[i] == "Y") {
						y = ofToFloat(wordsFila[i]) * escala;
						num++;
						editado << ofToString(y, 2) << ",";
					}
					else if (words6[i] == "Z") {
						z = ofToFloat(wordsFila[i]) * escala;
						num++;
						editado << ofToString(z, 2);// << ",";
					}
					coords.push_back(ofPoint(x, y, z));
					//ofLogNotice("x:" + ofToString(x) + ",y:" + ofToString(y) + ",z:" + ofToString(z));
					//editado << x << "," << y << "," << z << ",";
					//editado << "\n";
				}
			}
			if (num == 3) {
				editado << "\n";
				num = 0;
			}
		}
		//editado << "\n";
	}
	editado.close();
	lineaAnalisis += 2;
	lineaAnalisis = lineaAnalisis % (linea.size() - 5);

	ofLogNotice("Coordenadas Listas");
}

//--------------------------------------------------------------
void ofApp::cargaCoords() {
	lineaAnalisis+=2;
	lineaAnalisis = lineaAnalisis % (linea.size() - 7);
	wordsFila = ofSplitString(linea[7+lineaAnalisis], ",");

	/*for (int i = 0; i < words2.size()-1; i += saltoLinea) {
		if (words2[i] == "Bone") {
			if (words5[i] == "Position") {
				if (words6[i] == "X") {
					x = ofToFloat(wordsFila[i]) * escala;
				}
				else if (words6[i] == "Y") {
					y = ofToFloat(wordsFila[i]) * escala;
				}
				else if (words6[i] == "Z") {
					z = ofToFloat(wordsFila[i]) * escala;
				}
				coords.push_back(ofPoint(x, y, z));
			}
		}
	}*/

	coords.clear();

	for (int i = 0; i < words2.size() - 1; i += saltoLinea) {
		if (words2[i] == "Bone") {
			if (words5[i] == "Position") {
				if (words6[i] == "X") {
					x = ofToFloat(wordsFila[i]) * escala;
				}
				else if (words6[i] == "Y") {
					y = ofToFloat(wordsFila[i]) * escala;
				}
				else if (words6[i] == "Z") {
					y = ofToFloat(wordsFila[i]) * escala;
				}
				coords.push_back(ofPoint(x, y, z));
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::dibujaCoords() {
	if (borra)
		ofSetColor(0, 0);
	else
		ofSetColor(0, 2);

	ofFill();
	ofRect(0, 0, ofGetWidth(), ofGetHeight());

	ofSetColor(255, 100);
	ofFill();

	cam.begin();
	if (coords.size() != 0) {
		for (int i = 0; i < coords.size(); i++) {
			ofDrawEllipse(coords[i], 15, 15);
		}
	}
	cam.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (analiza) {

	}
	else {
		dibujaCoords();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		ofBackground(0, 0);
		borra = true;
		lineaAnalisis = 0;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == ' ')
		borra = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
