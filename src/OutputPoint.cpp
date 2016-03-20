#include "OutputPoint.h"

const ofColor OutputPoint::c_has_output(100, 100, 100);
const ofColor OutputPoint::c_has_no_output(20, 20, 20);

OutputPoint::OutputPoint() {
    output = 0;
}

ChangeSet OutputPoint::execute(TickBuffer* buffer) {
    ChangeSet changes;
    changes.output = output;
    changes.position_delta = 1;
    return changes;
}

ChangeSet OutputPoint::click() {
    ChangeSet changes;
    output++;
    output %= OUTPUT_MAX;
    changes.output = output;
    return changes;
}

void OutputPoint::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    if (output > 0) {
        ofSetColor(c_has_output);
    } else {
        ofSetColor(c_has_no_output);
    }
    ofDrawRectangle(x + POINT_SPACING, y + POINT_SPACING , POINT_INNER, POINT_INNER);

    if (active) {
        ofSetColor(ofColor::white);
    } else {
        ofSetColor(ofColor::lightGray);
    }
    font.drawString("OUT>", x + 3, y + 13);
    font.drawString(ofToString(output), x + 3, y + 33);
}
