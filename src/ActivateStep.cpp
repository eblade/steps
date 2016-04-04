#include "ActivateStep.h"


const ofColor ActivateStep::c_on(0, 100, 0);
const ofColor ActivateStep::c_off(100, 0, 0);


ActivateStep::ActivateStep() : Step() {
    type = STEP_TYPE_ACTIVATE;
    label = 0;
    changed_label = true;
    hold = false;

    tool_activate = new Tool("TURN\nON", ' ',
        new Change(TARGET_LEVEL_STEP, OP_ACTIVE_SET, 1));
    tool_deactivate = new Tool("TURN\nOFF", ' ',
        new Change(TARGET_LEVEL_STEP, OP_ACTIVE_SET, 0));
    tool_hold = new Tool("HOLD", 'h',
        new Change(TARGET_LEVEL_STEP, OP_HOLD_SET, 1));
    tool_dont_hold = new Tool("DON'T\nHOLD", 'h',
        new Change(TARGET_LEVEL_STEP, OP_HOLD_SET, 0));
    tool_label_0 = new Tool("LABEL\n#0", '0',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 0));
    tool_label_1 = new Tool("LABEL\n#1", '1',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 1));
    tool_label_2 = new Tool("LABEL\n#2", '2',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 2));
    tool_label_3 = new Tool("LABEL\n#3", '3',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 3));
    tool_label_4 = new Tool("LABEL\n#4", '4',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 4));
    tool_label_5 = new Tool("LABEL\n#5", '5',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 5));
    tool_label_6 = new Tool("LABEL\n#6", '6',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 6));
    tool_label_7 = new Tool("LABEL\n#7", '7',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 7));
    tool_label_8 = new Tool("LABEL\n#8", '8',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 8));
    tool_label_9 = new Tool("LABEL\n#9", '9',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_SET, 9));
    tool_label_up = new Tool("LABEL\n+1", '+',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_DELTA, 1));
    tool_label_down = new Tool("LABEL\n-1", '-',
        new Change(TARGET_LEVEL_STEP, OP_LABEL_DELTA, -1));
}

ActivateStep::~ActivateStep() {
    delete tool_activate;
    delete tool_deactivate;
    delete tool_hold;
    delete tool_dont_hold;
    delete tool_label_0;
    delete tool_label_1;
    delete tool_label_2;
    delete tool_label_3;
    delete tool_label_4;
    delete tool_label_5;
    delete tool_label_6;
    delete tool_label_7;
    delete tool_label_8;
    delete tool_label_9;
    delete tool_label_up;
    delete tool_label_down;
}

ChangeSet* ActivateStep::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet* changes = new ChangeSet();
    if (!hold || sequencer.release == 0) { // release == 0 means just synced
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
        changed = true;
    }
    if (changed_label) {
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_LABEL_SET, label));
        changed_label = false;
    } else {
        if (label != sequencer.label) {
            changed = true;
        }
        label = sequencer.label;
    }
    return changes;
}

void ActivateStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    if (active) {
        ofSetColor(c_on);
    } else {
        ofSetColor(c_off);
    }
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);
    
    ofSetColor(255);
    font.drawString(ofToString("#") + ofToString(label), x + 3, y + 13);
    if (hold && executing) {
        font.drawString(ofToString("HELD"), x + 3, y + 28);
    } else if (hold) {
        font.drawString(ofToString("HOLD"), x + 3, y + 28);
    }
}

void ActivateStep::populate(Toolbar* toolbar) {
    if (active) {
        toolbar->push(tool_deactivate);
    } else {
        toolbar->push(tool_activate);
    }
    if (hold) {
        toolbar->push(tool_dont_hold);
    } else {
        toolbar->push(tool_hold);
    }
    toolbar->push(tool_label_down);
    toolbar->push(tool_label_up);
    toolbar->push(tool_label_0); 
    toolbar->push(tool_label_1); 
    toolbar->push(tool_label_2); 
    toolbar->push(tool_label_3); 
    toolbar->push(tool_label_4); 
    toolbar->push(tool_label_5); 
    toolbar->push(tool_label_6); 
    toolbar->push(tool_label_7); 
    toolbar->push(tool_label_8); 
    toolbar->push(tool_label_9); 
}

void ActivateStep::change(ChangeSet* changes) {
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_STEP)) != NULL) {
        switch (change->operation) {
            case OP_LABEL_SET: setLabel(change->value); break;
            case OP_LABEL_DELTA: setLabel(label + change->value); break;
            case OP_ACTIVE_SET: setActive(change->value);
                changes->upstream->push(
                    new Change(TARGET_LEVEL_SEQUENCER, OP_ACTIVE_SET, change->value));
                if (change->value == 0) {
                    changes->upstream->push(
                        new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_SET, 0));
                } else {
                    changes->upstream->push(
                        new Change(TARGET_LEVEL_SEQUENCER, OP_SYNC));
                }
                break;
            case OP_HOLD_SET: setHold(change->value); break;
        }
    }
}

ChangeSet* ActivateStep::click() {
    ChangeSet* changes = new ChangeSet();
    if (hold) {
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_SYNC));
    } else {
        active = !active;
        if (active) {
            changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ACTIVE_SET, 1));
        } else {
            changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ACTIVE_SET, 0));
            changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_SET, 0));
        }
    }
    return changes;
}

void ActivateStep::write(ofstream& f) {
    f << "set-active " << (active ? "1" : "0") << "\n"
      << "set-label " << ofToString(label) << "\n"
      << "set-hold " << (hold ? "1" : "0") << "\n";
}

int ActivateStep::getLabel() { return label; }

void ActivateStep::setLabel(int label) {
    int what_it_was = this->label;
    if (label < 0) {
        this->label = 0;
    } else if (label >= MAX_LABELS - 1) {
        this->label = MAX_LABELS - 1;
    } else {
        this->label = label;
    }
    changed_label = this->label != what_it_was;
    changed = true;
}

bool ActivateStep::getHold() { return hold; };

void ActivateStep::setHold(bool hold) {
    this->hold = hold ? true : false;
    changed = true;
}
