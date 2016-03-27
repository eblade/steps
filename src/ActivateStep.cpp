#include "ActivateStep.h"


const ofColor ActivateStep::c_on(0, 100, 0);
const ofColor ActivateStep::c_off(100, 0, 0);


ActivateStep::ActivateStep() : Step() {
    type = STEP_TYPE_ACTIVATE;
}

void ActivateStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    if (active) {
        ofSetColor(c_on);
    } else {
        ofSetColor(c_off);
    }
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);
}

ChangeSet* ActivateStep::click() {
    ChangeSet* changes = new ChangeSet();
    active = !active;
    if (active) {
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ACTIVE_SET, 1));
    } else {
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ACTIVE_SET, 0));
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_SET, 0));
    }
    return changes;
}
