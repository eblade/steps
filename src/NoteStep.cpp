#include "NoteStep.h"

const ofColor NoteStep::c_active(0, 0, 50);
const ofColor NoteStep::c_inactive(20, 20, 20);
const ofColor NoteStep::c_sel_active(0, 0, 100);
const ofColor NoteStep::c_sel_inactive(40, 40, 40);

NoteStep::NoteStep() : Step() {
    type = STEP_TYPE_NOTE;
    note = 0;
    velocity = 100;
    length = 1000;
}

int NoteStep::getLength() {
    return length;
}

ChangeSet* NoteStep::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet* changes = new ChangeSet();
    if (active) {
        MidiEvent* start_event = new MidiEvent(
            buffer->relative_time, sequencer.output_router, sequencer.output, note, velocity);
        buffer->push(start_event);
        MidiEvent* stop_event = new MidiEvent(
            buffer->relative_time + sequencer.period, sequencer.output_router, sequencer.output, note, 0);
        buffer->push(stop_event);
    }
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_DELTA, 1));
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_RELEASE_DELTA, sequencer.period));
    return changes;
}

void NoteStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    if (executing) {
        if (active) {
            ofSetColor(c_sel_active);
        } else {
            ofSetColor(c_sel_inactive);
        }
    } else {
        if (active) {
            ofSetColor(c_active);
        } else {
            ofSetColor(c_inactive);
        }
    }
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);

    if (active) {
        ofSetColor(ofColor::white);
    } else {
        ofSetColor(ofColor::lightGray);
    }
    font.drawString(ofToString(note), x + 3, y + 13);
    font.drawString(ofToString(velocity), x + 3, y + 28);
}
