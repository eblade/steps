#include "Step.h"

Step::Step() {
    type = STEP_TYPE_NONE;
    active = true;
    changed = true;
}

int Step::getLength() {
    return 0;
}

void Step::execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer) {
}

void Step::draw(int x, int y, bool executing, ofTrueTypeFont font, ofTrueTypeFont font_big) {
}

void Step::click(ChangeSet* changes) {
}

void Step::populate(Toolbar* toolbar) {
}

void Step::change(ChangeSet* changes) {
}

void Step::write(ofstream& f) {
    f << "# unknown step\n";
}

bool Step::needsRedraw() {
    if (changed) {
        changed = false;
        return true;
    } else {
        return false;
    }
}

void Step::markForRedraw() {
    changed = true;
}

bool Step::getActive() { return active; };

void Step::setActive(bool active) {
    this->active = active;
    this->changed = true;
}

int Step::getType() { return type; }
