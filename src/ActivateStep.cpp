#include "ActivateStep.h"


const ofColor ActivateStep::c_on(0, 100, 0);
const ofColor ActivateStep::c_off(100, 0, 0);


ActivateStep::ActivateStep() : Step() {
    type = STEP_TYPE_ACTIVATE;
    label = 0;
    changed_label = true;

    tool_activate = new Tool("TURN\nON", ' ',
        new Change(TARGET_LEVEL_STEP, OP_ACTIVE_SET, 1));
    tool_deactivate = new Tool("TURN\nOFF", ' ',
        new Change(TARGET_LEVEL_STEP, OP_ACTIVE_SET, 0));
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
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
    if (changed_label) {
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_LABEL_SET, label));
        changed_label = false;
    } else {
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
}

void ActivateStep::populate(Toolbar* toolbar) {
    if (active) {
        toolbar->push(tool_deactivate);
    } else {
        toolbar->push(tool_activate);
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
            case OP_LABEL_SET:
                changed_label = (label != change->value);
                label = change->value;
                break;
            case OP_LABEL_DELTA:
                changed_label = (change->value != 0);
                label += change->value;
                label = label > MAX_LABELS ? MAX_LABELS : label;
                label = label < 0 ? 0 : label;
                break;
            case OP_ACTIVE_SET:
                active = change->value ? true : false;
                break;
        }
    }
}

ChangeSet* ActivateStep::click() {
    ChangeSet* changes = new ChangeSet();
    active = !active;
    if (active) {
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ACTIVE_SET, 1));
    } else {
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ACTIVE_SET, 0));
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_SET, 0));
    }
    return changes;
}
