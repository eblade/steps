#include "ofApp.h"

void ofApp::setup() {
    // Logging
    ofSetLogLevel(OF_LOG_VERBOSE);

    // Setup up the timed buffer
    buffer = new TickBuffer(60);

    // Setup up the Output Router
    output_router = new OutputRouter();

    // Set up the first non-through device as 0
    OutputSettings output_settings;
    output_settings.device = 1;
    output_settings.type = OUTPUT_TYPE_MIDI;
    output_settings.channel = 1;
    output_router->install(0, output_settings);

    // Create a new Page
    for (int i = 0; i < MAX_PAGES; i++) {
        page[i] = NULL;
    }
    active_page = addPage();
    playing = true;

    // Graphics
    font.load(OF_TTF_SANS, 9, true, true);
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);

    ofLogNotice(APPLICATION) << "Done with setup.";
}

void ofApp::exit() {
    delete buffer;
    delete output_router;
    for (int i = 0; i < MAX_PAGES; i++) {
        if (page[i] != NULL) {
            delete page[i];
            page[i] = NULL;
        }
    }
}

void ofApp::update() {

}

void ofApp::draw() {
    if (playing) {
        step();
        buffer->tick();
    } else {
        buffer->clear();
    }

    // Clear the background
    ofBackground(ofColor::black);

    // Draw the active page
    if (page[active_page] != NULL) {
        page[active_page]->draw(0, 0, ofGetWidth(), ofGetHeight(), font);
    }

    // Draw some debug info to the right
    ofSetColor(255);
    font.drawString("page: " + ofToString(active_page), ofGetWidth() - 90, 10);
    font.drawString("bpm: " + ofToString(buffer->bpm), ofGetWidth() - 90, 25);
    font.drawString("fps: " + ofToString((int)ofGetFrameRate()), ofGetWidth() - 90, 40);

    buffer->draw(ofGetWidth() - 90, 50);

    for (int i = 0; i < OUTPUT_MAX; i++) {
        font.drawString(output_router->getOutputString(i), ofGetWidth() - 90, 150 + 15*i);
    }
}

void ofApp::step() {
    if (page[active_page] != NULL) {
        page[active_page]->step(buffer, output_router);
    }
}

int ofApp::addPage() {
    for (int i = 0; i < MAX_PAGES; i++) {
        if (page[i] == NULL) {
            page[i] = new Page();
            return i;
        }
    }
    return -1;
}

void ofApp::keyPressed(int key) {
    if (page[active_page] != NULL) {
        page[active_page]->keyPressed(key);
    }
}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y){

}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){
    if (page[active_page] != NULL) {
        page[active_page]->mousePressed(x, y, button);
    }
}

void ofApp::mouseReleased(int x, int y, int button){

}

void ofApp::mouseEntered(int x, int y){

}

void ofApp::mouseExited(int x, int y){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

