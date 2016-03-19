#include "Point.h"

Point::Point() {
    type = POINT_TYPE_NONE;
    active = true;
}

int Point::getLength() {
    return 0;
}

ExecutionResult Point::execute(long long time, TickBuffer* buffer) {
    ExecutionResult result;
    return result;
}

void Point::draw(int x, int y, bool executing, ofTrueTypeFont font) {
}

void Point::click() {
    active = !active;
}

// ===========================================================================
// DUMMY EVENT

DummyEvent::DummyEvent() {
    time = 0;
}

void DummyEvent::fire() {
    cout << "Fire! " << time << endl;
}
