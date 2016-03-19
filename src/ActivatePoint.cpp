#include "ActivatePoint.h"


const ofColor ActivatePoint::c_on(0, 100, 0);
const ofColor ActivatePoint::c_off(100, 0, 0);


ActivatePoint::ActivatePoint() : Point() {
    type = POINT_TYPE_ACTIVATE;
}

ExecutionResult ActivatePoint::execute(long long time, TickBuffer* buffer) {
    ExecutionResult result;
    if (active) {
        result.set_active = true;
    } else {
        result.set_inactive = true;
    }
    result.position_delta = 1;
    return result;
}

void ActivatePoint::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    if (active) {
        ofSetColor(c_on);
    } else {
        ofSetColor(c_off);
    }
    ofDrawRectangle(x + POINT_SPACING, y + POINT_SPACING , POINT_INNER, POINT_INNER);
}

void ActivatePoint::click() {
    active = !active;
}
