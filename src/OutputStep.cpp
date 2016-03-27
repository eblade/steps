#include "OutputStep.h"

const ofColor OutputStep::c_has_output(100, 100, 100);
const ofColor OutputStep::c_has_no_output(20, 20, 20);

OutputStep::OutputStep() {
    output = 0;
}

ChangeSet* OutputStep::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet* changes = new ChangeSet();
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_OUTPUT_SET, output));
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_DELTA, 1));
    return changes;
}

ChangeSet* OutputStep::click() {
    ChangeSet* changes = new ChangeSet();
    output++;
    output %= MAX_OUTPUTS;
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_OUTPUT_SET, output));
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
