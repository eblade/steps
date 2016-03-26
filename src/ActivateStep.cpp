#include "ActivateStep.h"


const ofColor ActivateStep::c_on(0, 100, 0);
const ofColor ActivateStep::c_off(100, 0, 0);


ActivateStep::ActivateStep() : Step() {
    type = STEP_TYPE_ACTIVATE;
}

ChangeSet ActivateStep::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet changes;
    changes.position_delta = 1;
    return changes;
}

void ActivateStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    if (active) {
        ofSetColor(c_on);
    } else {
        ofSetColor(c_off);
    }
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);
}

ChangeSet ActivateStep::click() {
    ChangeSet changes;
    active = !active;
    if (active) {
        changes.set_active = true;
    } else {
        changes.set_inactive = true;
        changes.goto_position = 0;
    }
    return changes;
}
