#include "NotePoint.h"

const ofColor NotePoint::c_active(0, 0, 50);
const ofColor NotePoint::c_inactive(20, 20, 20);
const ofColor NotePoint::c_sel_active(0, 0, 100);
const ofColor NotePoint::c_sel_inactive(40, 40, 40);

NotePoint::NotePoint() : Point() {
    type = POINT_TYPE_NOTE;
    value = 0;
    length = 1000;
}

int NotePoint::getLength() {
    return length;
}

ChangeSet NotePoint::execute(TickBuffer* buffer) {
    ChangeSet changes;
    if (active) {
        DummyEvent* start_event = new DummyEvent("ON", buffer->relative_time);
        buffer->push(start_event);
        DummyEvent* stop_event = new DummyEvent("OFF", buffer->relative_time + length);
        buffer->push(stop_event);
    }
    changes.position_delta = 1;
    return changes;
}

void NotePoint::draw(int x, int y, bool executing, ofTrueTypeFont font) {
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
    ofDrawRectangle(x + POINT_SPACING, y + POINT_SPACING , POINT_INNER, POINT_INNER);

    if (active) {
        ofSetColor(ofColor::white);
    } else {
        ofSetColor(ofColor::lightGray);
    }
    font.drawString(ofToString(value), x + 3, y + 13);
}
