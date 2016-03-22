#include "ActivatePoint.h"


const ofColor ActivatePoint::c_on(0, 100, 0);
const ofColor ActivatePoint::c_off(100, 0, 0);


ActivatePoint::ActivatePoint() : Point() {
    type = POINT_TYPE_ACTIVATE;
}

ChangeSet ActivatePoint::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet changes;
    changes.position_delta = 1;
    return changes;
}

void ActivatePoint::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    if (active) {
        ofSetColor(c_on);
    } else {
        ofSetColor(c_off);
    }
    ofDrawRectangle(x + POINT_SPACING, y + POINT_SPACING , POINT_INNER, POINT_INNER);
}

ChangeSet ActivatePoint::click() {
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
