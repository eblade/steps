#include "Toolbar.h"


Toolbar::Toolbar() {
    state = TOOLBAR_STATE_IDLE;
}

void Toolbar::draw(ofTrueTypeFont font) {
    ofSetColor(50);
    ofDrawRectangle(0, ofGetHeight() - STEP_OUTER, ofGetWidth(), STEP_OUTER);
}

void Toolbar::keyPressed(int key) {

}
