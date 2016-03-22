#include "Point.h"

Point::Point() {
    type = POINT_TYPE_NONE;
    active = true;
}

int Point::getLength() {
    return 0;
}

ChangeSet Point::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet result;
    return result;
}

void Point::draw(int x, int y, bool executing, ofTrueTypeFont font) {
}

ChangeSet Point::click() {
    ChangeSet changes;
    active = !active;
    return changes;
}

// ===========================================================================
// DUMMY EVENT

DummyEvent::DummyEvent(string name, long long time) {
    this->name = name;
    this->time = time;
    cout << "Sched " << name << " @" << time << " addr " << this << endl;
}

void DummyEvent::fire() {
    cout << "Fire! " << name << " @" << time << " addr " << this << endl;
}
