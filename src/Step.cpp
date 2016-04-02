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
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
    return changes;
}

void Step::draw(int x, int y, bool executing, ofTrueTypeFont font) {

}

ChangeSet* Step::click() {
    ChangeSet* changes = new ChangeSet();
    active = !active;
    return changes;
}

void Step::populate(Toolbar* toolbar) {

}

void Step::change(ChangeSet* changes) {

}

void Step::write(ofstream& f) {
    f << "# unknown step\n";
}

bool Step::getActive() { return active; };

void Step::setActive(bool active) {
    this->active = active ? true : false;
}
