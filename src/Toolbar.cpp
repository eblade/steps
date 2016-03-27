#include "Toolbar.h"

const ofColor Tool::c_back(50, 50, 50);
const ofColor Tool::c_text(200, 200, 200);
const ofColor Tool::c_key_back(200, 50, 50);
const ofColor Tool::c_key_text(0, 0, 0);


Tool::Tool() {
    init("?", "");
}

Tool::Tool(string label, int key, Change* change) {
    init(label, ofToString(key));
    this->key[0] = key;
    this->changes->push(change);
}

PersistantTool::PersistantTool() {
    init("?", "");
    this->persistant = true;
}

PersistantTool::PersistantTool(string label, int key, Change* change) {
    init(label, ofToString(key));
    this->key[0] = key;
    this->changes->push(change);
    this->persistant = true;
}

Tool::~Tool() {
    delete changes;
}

void Tool::init(string label, string key_string) {
    for (int i = 0; i < MAX_KEYS; i++) {
        key[i] = 0;
    }
    this->label = label;
    this->key_string = key_string;
    this->changes = new ChangeSet();
    this->persistant = false;
}
    
void Tool::draw(int x, int y, ofTrueTypeFont font) {
    ofSetColor(c_key_back);
    ofDrawRectangle(
        x + STEP_SPACING, y + STEP_SPACING,
        STEP_INNER, STEP_KEY_HEIGHT
    );
    ofSetColor(c_key_text);
    font.drawString(ofToString(key_string), x + 3, y + STEP_SPACING + 13);
    ofSetColor(c_back);
    ofDrawRectangle(
        x + STEP_SPACING, y + STEP_SPACING + STEP_KEY_HEIGHT,
        STEP_INNER, STEP_INNER
    );
    ofSetColor(c_text);
    font.drawString(ofToString(label), x + 3, y + STEP_SPACING + STEP_KEY_HEIGHT + 15);
}

// ============================================================================= 

Toolbar::Toolbar() {
    for (int i = 0; i < MAX_TOOLS; i++) {
        tool[i] = NULL;
    }
    head = 0;
}

Toolbar::~Toolbar() {
    for (int i = 0; i < MAX_TOOLS; i++) {
        if (tool[i] != NULL) {
            if (!tool[i]->persistant) {
                delete tool[i];
            }
            tool[i] = NULL;
        }
    }
}

void Toolbar::draw(ofTrueTypeFont font) {
    ofSetColor(20);
    ofDrawRectangle(
        0, ofGetHeight() - STEP_OUTER - STEP_KEY_HEIGHT,
        ofGetWidth(), STEP_OUTER + STEP_KEY_HEIGHT
    );

    int x, y;
    x = 0;
    y = ofGetHeight() - STEP_OUTER - STEP_KEY_HEIGHT;
    for (int i = 0; i < MAX_TOOLS; i++) {
        x = STEP_OUTER * i;

        if (tool[i] != NULL) {
            tool[i]->draw(x, y, font);
        } else {
            break;
        }
    }
}

void Toolbar::keyPressed(int key) {

}

void Toolbar::mousePressed(int x, int y, int button) {

}

void Toolbar::push(Tool* tool) {
    if (head >= MAX_TOOLS) {
        ofLogError(APPLICATION) << "Max tools exceeded!";
    }
    this->tool[head] = tool;
    head++;
}

void Toolbar::update(Page* page) {

}

int Toolbar::getHeight() {
    return STEP_OUTER;
}
