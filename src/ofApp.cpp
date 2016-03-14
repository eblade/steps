#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    addNewLine(-1);

    cursor = 0;

    font.load(OF_TTF_SANS, 9, true, true);
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::black);
    for (int i = 0; i < MAX_LENGTH; i++) {
        if (sequencer[i] == NULL) {
            break;
        }
        sequencer[i]->draw(i, i==cursor, font);
    }
    ofSetColor(255);
    font.drawString("x: " + ofToString((int)sequencer[cursor]->cursor), ofGetWidth() -150, 20);
    font.drawString("y: " + ofToString((int)cursor), ofGetWidth() -150, 30);
    font.drawString("fps: " + ofToString((int)ofGetFrameRate()), ofGetWidth() -150, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'h' || key == OF_KEY_LEFT) {
        sequencer[cursor]->cursorLeft();
    } else if (key == 'l' || key == OF_KEY_RIGHT) {
        sequencer[cursor]->cursorRight();
    } else if (key == 'k' || key == OF_KEY_UP) {
        cursorUp();
    } else if (key == 'j' || key == OF_KEY_DOWN) {
        cursorDown();
    } else if (key == ' ') {
        sequencer[cursor]->cursorClick();
    } else if (key == 'q') {
        sequencer[cursor]->cursorNote(0);
    } else if (key == '2') {
        sequencer[cursor]->cursorNote(1);
    } else if (key == 'w') {
        sequencer[cursor]->cursorNote(2);
    } else if (key == '3') {
        sequencer[cursor]->cursorNote(3);
    } else if (key == 'e') {
        sequencer[cursor]->cursorNote(4);
    } else if (key == 'r') {
        sequencer[cursor]->cursorNote(5);
    } else if (key == '5') {
        sequencer[cursor]->cursorNote(6);
    } else if (key == 't') {
        sequencer[cursor]->cursorNote(7);
    } else if (key == '6') {
        sequencer[cursor]->cursorNote(8);
    } else if (key == 'y') {
        sequencer[cursor]->cursorNote(9);
    } else if (key == '7') {
        sequencer[cursor]->cursorNote(10);
    } else if (key == 'u') {
        sequencer[cursor]->cursorNote(11);
    } else if (key == '8') {
        sequencer[cursor]->cursorNote(12);
    } else if (key == 'i') {
        sequencer[cursor]->cursorNote(13);
    } else if (key == '9') {
        sequencer[cursor]->cursorNote(14);
    } else if (key == 'o') {
        sequencer[cursor]->cursorNote(15);
    } else if (key == 'p') {
        sequencer[cursor]->cursorNote(16);
    } else if (key == OF_KEY_RETURN) {
        addNewLine(cursor);
    }
}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y){

}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){

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

void ofApp::cursorUp() {
    if (cursor > 0) {
        cursor--;
        sequencer[cursor]->setCursor(sequencer[cursor + 1]->cursor);
    }
}

void ofApp::cursorDown() {
    if (cursor < (MAX_LINES - 1)) {
        if (sequencer[cursor + 1] != NULL) {
            cursor++;
            sequencer[cursor]->setCursor(sequencer[cursor - 1]->cursor);
        }
    }
}

void ofApp::addNewLine(int afterLine) {
    if (afterLine >= MAX_LINES) {
        return;
    }
    if (sequencer[afterLine + 1] == NULL) {
        sequencer[afterLine + 1] = new Sequencer("seq" + (afterLine + 1));
        if (cursor == afterLine) {
            cursor++;
        }
    }
}
