#include "LoopStep.h"

LoopStep::LoopStep(int loops) {
    type = STEP_TYPE_LOOP;
    this->loops = loops;
    vamp = false;
    last = false;
    count = 1;

    tool_set_vamp = new Tool("VAMP", 'v',
        new Change(TARGET_LEVEL_STEP, OP_VAMP_SET, 1));
    tool_unset_vamp = new Tool("NO\nVAMP", 'v',
        new Change(TARGET_LEVEL_STEP, OP_VAMP_SET, 0));
    tool_last = new Tool("LAST", ' ',
        new Change(TARGET_LEVEL_STEP, OP_LAST));
    tool_reset = new Tool("RESET", 'r',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_RESET));
    tool_1 = new Tool("1\nLOOP", '1',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_SET, 1));
    tool_2 = new Tool("2\nLOOPS", '2',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_SET, 2));
    tool_3 = new Tool("3\nLOOPS", '3',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_SET, 3));
    tool_4 = new Tool("4\nLOOPS", '4',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_SET, 4));
    tool_5 = new Tool("5\nLOOPS", '5',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_SET, 5));
    tool_6 = new Tool("6\nLOOPS", '6',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_SET, 6));
    tool_7 = new Tool("7\nLOOPS", '7',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_SET, 7));
    tool_8 = new Tool("8\nLOOPS", '8',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_SET, 8));
    tool_9 = new Tool("9\nLOOPS", '9',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_SET, 9));
    tool_up = new Tool("+\nLOOP", '+',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_DELTA, 1));
    tool_down = new Tool("-\nLOOP", '-',
        new Change(TARGET_LEVEL_STEP, OP_LOOP_DELTA, -1));
}

LoopStep::~LoopStep() {
    delete tool_set_vamp;
    delete tool_unset_vamp;
    delete tool_last;
    delete tool_reset;
    delete tool_1;
    delete tool_2;
    delete tool_3;
    delete tool_4;
    delete tool_5;
    delete tool_6;
    delete tool_7;
    delete tool_8;
    delete tool_9;
    delete tool_up;
    delete tool_down;
}

void LoopStep::execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer) {
    if (vamp) {
        if (last) {
            changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
        } else {
            changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_SET, 0));
            count++;
            changed = true;
        }
    } else {
        if (count == loops) {
            changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
        } else {
            changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_SET, 0));
            count++;
            changed = true;
        }
    }
}

void LoopStep::click(ChangeSet* changes) {
    if (vamp) {
        setLast();
    } else {
        reset();
    }
}

void LoopStep::draw(int x, int y, bool executing, ofTrueTypeFont font, ofTrueTypeFont font_big) {
    ofSetColor(0);
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);

    if (vamp) {
        ofSetColor(ofColor::white);
        font.drawString("VAMP", x + 3, y + 13);
        if (last) {
            font_big.drawString("LAST", x + 3, y + 38);
        } else {
            font_big.drawString(ofToString(count), x + 3, y + 38);
        }
    } else {
        ofSetColor(ofColor::white);
        font.drawString("LOOP", x + 3, y + 13);
        font_big.drawString(ofToString(count) + "/" + ofToString(loops), x + 3, y + 38);
    }
}

void LoopStep::populate(Toolbar* toolbar) {
    if (vamp) {
        toolbar->push(tool_unset_vamp);
    } else {
        toolbar->push(tool_set_vamp);
    }
    toolbar->push(tool_last);
    toolbar->push(tool_reset);
    toolbar->push(tool_up);
    toolbar->push(tool_down);
    toolbar->push(tool_1);
    toolbar->push(tool_2);
    toolbar->push(tool_3);
    toolbar->push(tool_4);
    toolbar->push(tool_5);
    toolbar->push(tool_6);
    toolbar->push(tool_7);
    toolbar->push(tool_8);
    toolbar->push(tool_9);
}

void LoopStep::change(ChangeSet* changes) {
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_STEP)) != NULL) {
        switch (change->operation) {
            case OP_LOOP_SET: setLoops(change->value); break;
            case OP_LOOP_DELTA: setLoops(loops + change->value); break;
            case OP_VAMP_SET: setVamp(change->value ? true : false); break;
            case OP_LAST: setLast(); break;
            case OP_LOOP_RESET: reset(); break;
        }
    }
}

void LoopStep::write(ofstream& f) {
    f << "delta-cursor 1\n"
      << "add-loop-step\n"
      << "set-loops " << ofToString(loops) << "\n"
      << "set-vamp " << (vamp ? "1" : "0") << "\n";
}

int LoopStep::getLoops() { return this->loops; }

void LoopStep::setLoops(int loops) {
    this->loops = loops < 0 ? 0 : loops;
    changed = true;
}

bool LoopStep::isVamp() { return vamp; }

void LoopStep::setVamp(bool vamp) {
    changed = this->vamp != vamp;
    this->vamp = vamp;
}

bool LoopStep::isLast() { return last; }

void LoopStep::setLast() {
    last = true;
    if (!vamp) {
        count = loops;
    }
    changed = true;
}

void LoopStep::reset() {
    count = 1;
    last = count == loops;
    changed = true;
}
