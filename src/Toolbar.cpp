#include "Toolbar.h"

const ofColor Tool::c_back(50, 50, 50);
const ofColor Tool::c_text(200, 200, 200);
const ofColor Tool::c_back_page(50, 50, 100);
const ofColor Tool::c_text_page(200, 200, 200);
const ofColor Tool::c_back_seq(100, 50, 50);
const ofColor Tool::c_text_seq(200, 200, 200);
const ofColor Tool::c_back_step(50, 100, 50);
const ofColor Tool::c_text_step(200, 200, 200);
const ofColor Tool::c_key_back(20, 20, 20);
const ofColor Tool::c_key_text(100, 100, 100);


Tool::Tool() {
    init("?", "");
}

Tool::Tool(string label, int key, Change* change) {
    init(label, ofToString((char) key));
    this->key[0] = key;
    this->changes->push(change);
    this->level = change->level;
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
    this->level = TARGET_LEVEL_APPLICATION;
    this->flashing = 0;
    this->changed = true;
}

void Tool::flash() {
    flashing = 20;
}

void Tool::markForRedraw() {
    changed = true;
}

bool Tool::needsRedraw() {
    if (changed) {
        changed = false;
        return true;
    } else {
        return false;
    }
}

bool Tool::needsFlashing() {
    if (flashing > 0) {
        flashing--;
        return true;
    } else {
        return false;
    }
}

void Tool::draw(int x, int y, ofTrueTypeFont font, bool redraw) {
    redraw = redraw || needsRedraw();
    if (needsFlashing() || redraw) {
        ofSetColor(20 + 10 * flashing);
        ofDrawRectangle(
            x + STEP_SPACING, y + STEP_SPACING,
            STEP_INNER, STEP_KEY_HEIGHT
        );

        ofSetColor(c_key_text);
        font.drawString(ofToString(key_string), x + 3, y + STEP_SPACING + 13);
    }

    if (redraw) {
        if (level == TARGET_LEVEL_APPLICATION) {
            ofSetColor(c_back);
        } else if (level == TARGET_LEVEL_PAGE) {
            ofSetColor(c_back_page);
        } else if (level == TARGET_LEVEL_SEQUENCER) {
            ofSetColor(c_back_seq);
        } else if (level == TARGET_LEVEL_STEP) {
            ofSetColor(c_back_step);
        }
        ofDrawRectangle(
            x + STEP_SPACING, y + STEP_SPACING + STEP_KEY_HEIGHT,
            STEP_INNER, STEP_INNER
        );
        if (level == TARGET_LEVEL_APPLICATION) {
            ofSetColor(c_text);
        } else if (level == TARGET_LEVEL_PAGE) {
            ofSetColor(c_text_page);
        } else if (level == TARGET_LEVEL_SEQUENCER) {
            ofSetColor(c_text_seq);
        } else if (level == TARGET_LEVEL_STEP) {
            ofSetColor(c_text_step);
        }
        font.drawString(ofToString(label), x + 3, y + STEP_SPACING + STEP_KEY_HEIGHT + 15);
    }
}

bool Tool::hasKey(int key) {
    for (int i = 0; i < MAX_KEYS; i++) {
        if (this->key[i] == key) {
            return true;
        } else if (this->key[i] == 0) {
            return false;
        }
    }
    return false;
}

void Tool::addKey(int key) {
    for (int i = 0; i < MAX_KEYS; i++) {
        if (this->key[i] == 0) {
            this->key[i] = key;
            return;
        }
    }
}

// ============================================================================= 

Toolbar::Toolbar() {
    for (int i = 0; i < MAX_TOOLS; i++) {
        tool[i] = NULL;
    }
    head = 0;
    blank = true;
}

Toolbar::~Toolbar() {

}

void Toolbar::draw(ofTrueTypeFont font, bool redraw_all) {
    if (redraw_all) {
        ofSetColor(20);
        ofDrawRectangle(
            0, ofGetHeight() - STEP_OUTER - STEP_KEY_HEIGHT,
            ofGetWidth(), STEP_OUTER + STEP_KEY_HEIGHT
        );
    }

    int x, y;
    x = 0;
    y = ofGetHeight() - STEP_OUTER - STEP_KEY_HEIGHT;
    for (int i = 0; i < MAX_TOOLS; i++) {
        if (tool[i] != NULL) {
            tool[i]->draw(x, y, font, redraw_all);
        } else {
            break;
        }
        x += STEP_OUTER;
    }
    if (blank) {
        ofSetColor(20);
        ofDrawRectangle(
            x, ofGetHeight() - STEP_OUTER - STEP_KEY_HEIGHT,
            ofGetWidth() - x, STEP_OUTER + STEP_KEY_HEIGHT
        );
        blank = false;
    }
}

ChangeSet* Toolbar::keyPressed(int key) {
    ChangeSet* changes = new ChangeSet();
    for (int i = 0; i < head; i++) {
        if (tool[i] != NULL && tool[i]->hasKey(key)) {
            tool[i]->flash();
            changes->push(tool[i]->changes);
        }
    }
    return changes;
}

ChangeSet* Toolbar::mousePressed(int x, int y, int button) {
    ChangeSet* changes = new ChangeSet();
    int i = x / STEP_OUTER;
    if (i < MAX_TOOLS && tool[i] != NULL) {
        tool[i]->flash();
        changes->push(tool[i]->changes);
    }
    return changes;
}

void Toolbar::push(Tool* tool) {
    if (head >= MAX_TOOLS) {
        ofLogError("Toolbar") << "Max tools exceeded!";
        return;
    }
    this->tool[head] = tool;
    head++;
}

int Toolbar::getHeight() {
    return STEP_OUTER;
}

void Toolbar::markForRedraw(Toolbar *other_toolbar) {
    for (int i = 0; i < MAX_TOOLS; i++) {
        if (tool[i] == NULL) {
            break;
        } else if (tool[i] != other_toolbar->getTool(i)) {
            tool[i]->markForRedraw();
        }
    }
}

Tool* Toolbar::getTool(int position) {
    return tool[position];
}
