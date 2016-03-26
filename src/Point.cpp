#include "Step.h"

Step::Step() {
    type = STEP_TYPE_NONE;
    active = true;
}

int Step::getLength() {
    return 0;
}

ChangeSet Step::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet result;
    return result;
}

void Step::draw(int x, int y, bool executing, ofTrueTypeFont font) {
}

ChangeSet Step::click() {
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
