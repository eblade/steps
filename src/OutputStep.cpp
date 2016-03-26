#include "OutputStep.h"

const ofColor OutputStep::c_has_output(100, 100, 100);
const ofColor OutputStep::c_has_no_output(20, 20, 20);

OutputStep::OutputStep() {
    output = 0;
}

ChangeSet OutputStep::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet changes;
    changes.output = output;
    changes.position_delta = 1;
    return changes;
}

ChangeSet OutputStep::click() {
    ChangeSet changes;
    output++;
    output %= OUTPUT_MAX;
    changes.output = output;
    return changes;
}

void OutputStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    if (output > 0) {
        ofSetColor(c_has_output);
    } else {
        ofSetColor(c_has_no_output);
    }
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);

    if (active) {
        ofSetColor(ofColor::white);
    } else {
        ofSetColor(ofColor::lightGray);
    }
    font.drawString("OUT", x + 3, y + 13);
    font.drawString(ofToString(output), x + 3, y + 33);
}