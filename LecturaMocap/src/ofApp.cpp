#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(4);
	ofSetBackgroundAuto(false);
	ofBackground(0);

	/// GUI
	setupGUI();
	ofxDatGui* gui = new ofxDatGui(100, 100);
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
/*--- GUI --------------------------------------*/
void ofApp::setupGUI() {	////////////// OFXDATGUI
	std::string pathGui;
	pathGui = "../";
	ofxDatGui::setAssetPath(pathGui);
	gui = new ofxDatGui(ofGetWidth() - 300, 50);
	//gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
	gui->setTheme(new ofxDatGuiThemeArteJoyas());
	//gui->setTheme(new ofxDatGuiThemeSmoke());
	gui->addLabel(": : : C  O  N  F  I  G  U  R  A  D  O  R : : :");
	gui->addBreak()->setHeight(10.0f);
	// =============================================
	ofxDatGuiFolder* guiDatos = gui->addFolder(":: D A T O S ::", ofColor::blue);
	/*guiDatos->addTextInput("N O M B R E", " -- TU NOMBRE -- ");
	guiDatos->addTextInput("C O R R E O", " -- CORREO ELECTRONICO -- ");
	gui->addBreak()->setHeight(10.0f);
	// =============================================
	gui->addLabel(": : : SELECTOR  A N I L L O  //  D I J E : : :");
	gui->addToggle("A N I L L O")->setChecked(true);
	gui->addBreak()->setHeight(10.0f);
	// =============================================
	gui->addDropdown("S E L E C C I O N A R   T A L L A", tallaArray)->setLabelColor(ofColor::fromHex(0xeeeeee));
	gui->addDropdown("D I A M E T R O   D I J E", dijeDiamArray)->setLabelColor(ofColor::fromHex(0xeeeeee));
	gui->addBreak()->setHeight(10.0f);
	// =============================================
	ofxDatGuiFolder* guiAnillo = gui->addFolder(":: P E R S O N A L I Z A ::", ofColor::cyan);
	guiAnillo->addSlider("V O L U M E N", 1, 50, 10)->setPrecision(1);
	guiAnillo->addSlider("V E L O C I D A D", 1, 4, velAudio)->setPrecision(0);
	// ANILLO
	guiAnillo->addSlider("A M P L I T U D", 10, subRad / 3, indiceMedio)->setPrecision(0);
	guiAnillo->addSlider("E s p e s o r", 15, 30, espesor)->setPrecision(0);
	guiAnillo->addSlider("G r o s o r", 30, 70, grosor)->setPrecision(0);
	// DIJE
	guiAnillo->addSlider("E s p e s o r ", 15, 25, espesorD)->setPrecision(0);
	guiAnillo->addSlider("G r o s o r ", 15, 40, grosorD)->setPrecision(0); /// DIJE
																			// GENERALES
	guiAnillo->addSlider("B o l e a d o", 5, 15, redondea)->setPrecision(0);
	guiAnillo->addSlider("O N D A", 0, 1, segundaOnda)->setPrecision(2);
	guiAnillo->addSlider("S A L T A R", 1, 15, 1)->setPrecision(0);
	gui->addBreak()->setHeight(10.0f);
	// =============================================
	ofxDatGuiFolder* guiVis = gui->addFolder(":: V I S U A L I Z A C I O N ::", ofColor::green);
	guiVis->addToggle("S U P E R F I C I E")->setChecked(true);
	guiVis->addToggle("M A T E R I A L")->setChecked(false);
	guiVis->addToggle("W I R E F R A M E")->setChecked(true);
	gui->addBreak()->setHeight(10.0f);
	// =============================================
	ofxDatGuiFolder* guiCam = gui->addFolder(":: C A M A R A ::", ofColor::white);
	guiCam->add2dPad("Camara")->setBounds(ofRectangle(ofPoint(0, 0), 100, 100), false);
	guiCam->addToggle("R O T A R")->setChecked(false);
	gui->addBreak()->setHeight(10.0f);

	// =============================================
	/// DEBUG //////////////////////////////////////////////////////
	if (debug) {
		ofxDatGuiFolder* guiDebug = gui->addFolder(":: D E B U G ::", ofColor::hotPink);
		guiDebug->addToggle("DEBUG")->setChecked(false);
		guiDebug->addSlider("AudioMinLevel", 0, 0.1f, volMin)->setPrecision(3);
		gui->addBreak()->setHeight(10.0f);
		// =============================================
	}
	ofxDatGuiFolder* guiCompra = gui->addFolder(":: C O M P R A R ::", ofColor::crimson);
	guiCompra->addButton("C O M P R A R    A H O R A");
	gui->addBreak()->setHeight(10.0f);
	// =============================================
	gui->addFooter();
	gui->getFooter()->setLabelWhenExpanded(" - M I N I M I Z A R - ");
	gui->getFooter()->setLabelWhenCollapsed(" - E X P A N D I R :: M E N U - ");
	gui->collapse();
	///////// EVENTOS ///////////////////////////////////
	gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
	gui->onSliderEvent(this, &ofApp::onSliderEvent);
	gui->onToggleEvent(this, &ofApp::onToggleEvent);
	gui->onTextInputEvent(this, &ofApp::onTextInputEvent);
	gui->on2dPadEvent(this, &ofApp::on2dPadEvent);
	gui->onButtonEvent(this, &ofApp::onButtonEvent);
	/// GUI A MOSTRAR AL INICIO /////////////////////////
	gui->getDropdown("S E L E C C I O N A R   T A L L A")->setVisible(true);
	gui->getDropdown("D I A M E T R O   D I J E")->setVisible(false);
	gui->getSlider("G R O S O R")->setVisible(true);
	gui->getSlider("G R O S O R ")->setVisible(false);
	gui->getSlider("E s p e s o r")->setVisible(true);
	gui->getSlider("E s p e s o r ")->setVisible(false);
	gui->getSlider("A M P L I T U D")->setVisible(true);
	*/
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
	editado.open(nombreArchivo + "_c_editado.csv", ofFile::WriteOnly);
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
