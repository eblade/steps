#include "SyncStep.h"

const ofColor SyncStep::c_active(100, 50, 50);
const ofColor SyncStep::c_inactive(20, 20, 20);
const ofColor SyncStep::c_sel_active(200, 100, 100);
const ofColor SyncStep::c_sel_inactive(40, 40, 40);


SyncStep::SyncStep(int label) : Step() {
    type = STEP_TYPE_SYNC;
    this->label = label;
    length = 1000;

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

SyncStep::~SyncStep() {
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

int SyncStep::getLength() {
    return length;
}

ChangeSet* SyncStep::execute(TickBuffer* buffer, SequencerState sequencer) {
    long long start, stop;
    ChangeSet* changes = new ChangeSet();
    start = sequencer.release > 0 ? sequencer.release : buffer->getLastTime();
    stop = start + sequencer.period;
    if (active) {
        changes->upstream->push(new Change(TARGET_LEVEL_PAGE, OP_SYNC, label));
    }
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_LABEL_SET, 0)); // Do not recieve sync
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_RELEASE_SET, stop));
    return changes;
}

void SyncStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
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
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING, STEP_INNER, STEP_INNER);

    if (active) {
        ofSetColor(ofColor::white);
        font.drawString(ofToString("SYNC"), x + 3, y + 13);
        ofSetColor(ofColor::lightGray);
        font.drawString(ofToString("#") + ofToString(label), x + 3, y + 28);
    }
}

void SyncStep::populate(Toolbar* toolbar) {
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

void SyncStep::change(ChangeSet* changes) {
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_STEP)) != NULL) {
        switch (change->operation) {
            case OP_LABEL_SET: setLabel(change->value); break;
            case OP_LABEL_DELTA: setLabel(label + change->value); break;
            case OP_ACTIVE_SET: setActive(change->value); break;
        }
    }
}

ChangeSet* SyncStep::click() {
    ChangeSet* changes = new ChangeSet();
    if (active) {
        changes->push(new Change(TARGET_LEVEL_PAGE, OP_SYNC, label));
    }
    return changes;
}

void SyncStep::write(ofstream& f) {
    f << "delta-cursor 1\n"
      << "add-sync-step\n"
      << "set-active " << ofToString(active ? "1" : "0") << "\n"
      << "set-label " << ofToString(label) << "\n";
}

int SyncStep::getLabel() { return label; }

void SyncStep::setLabel(int label) {
    if (label < 0) {
        this->label = 0;
    } else if (label >= MAX_LABELS - 1) {
        this->label = MAX_LABELS - 1;
    } else {
        this->label = label;
    }
    changed = true;
}
