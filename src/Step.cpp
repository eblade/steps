#include "Step.h"

Step::Step() {
    type = STEP_TYPE_NONE;
    active = true;
}

int Step::getLength() {
    return 0;
}

ChangeSet* Step::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet* changes = new ChangeSet();
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_DELTA, 1));
    return changes;
}

void Step::draw(int x, int y, bool executing, ofTrueTypeFont font) {
}

ChangeSet* Step::click() {
    ChangeSet* changes = new ChangeSet();
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
