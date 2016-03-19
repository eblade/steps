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

ExecutionResult NotePoint::execute(long long time, TickBuffer* buffer) {
    ExecutionResult result;
    if (active) {
        DummyEvent* start_event = new DummyEvent();
        DummyEvent* stop_event = new DummyEvent();
        start_event->time = time;
        stop_event->time = time + length;
        buffer->push(start_event);
        buffer->push(stop_event);
    }
    result.position_delta = 1;
    return result;
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

void NotePoint::click() {
    active = !active;
}
