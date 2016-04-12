#include "OutputStep.h"

OutputStep::OutputStep() {
    type = STEP_TYPE_OUTPUT;
    output = 0;

    tool_0 = new Tool("OUT\n0", '0',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 0));
    tool_1 = new Tool("OUT\n1", '1',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 1));
    tool_2 = new Tool("OUT\n2", '2',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 2));
    tool_3 = new Tool("OUT\n3", '3',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 3));
    tool_4 = new Tool("OUT\n4", '4',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 4));
    tool_5 = new Tool("OUT\n5", '5',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 5));
    tool_6 = new Tool("OUT\n6", '6',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 6));
    tool_7 = new Tool("OUT\n7", '7',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 7));
    tool_8 = new Tool("OUT\n8", '8',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 8));
    tool_9 = new Tool("OUT\n9", '9',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 9));
    tool_up = new Tool("+\nOUT", '+',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_DELTA, 1));
    tool_down = new Tool("-\nOUT", '-',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_DELTA, -1));
}

OutputStep::~OutputStep() {
    delete tool_0;
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

void OutputStep::execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer) {
    changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_OUTPUT_SET, output));
    changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
}

void OutputStep::click(ChangeSet* changes) {
    changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_OUTPUT_SET, output));
}

void OutputStep::draw(int x, int y, bool executing, ofTrueTypeFont font, ofTrueTypeFont font_big) {
    ofSetColor(OutputColors::color[output % OUTPUT_COLORS]);
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);

    ofSetColor(ofColor::white);
    font.drawString("OUT", x + 3, y + 13);
    font_big.drawString(ofToString(output), x + 3, y + 38);
}

void OutputStep::populate(Toolbar* toolbar) {
    toolbar->push(tool_up);
    toolbar->push(tool_down);
    toolbar->push(tool_0);
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

void OutputStep::change(ChangeSet* changes) {
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_STEP)) != NULL) {
        switch (change->operation) {
            case OP_OUTPUT_SET:
                setOutput(change->value);
                break;
            case OP_OUTPUT_DELTA:
                setOutput(output + change->value);
                break;
        }
    }
}

void OutputStep::write(ofstream& f) {
    f << "delta-cursor 1\n"
      << "add-output-step\n"
      << "set-output " << ofToString(output) << "\n";
}

int OutputStep::getOutput() { return this->output; }

void OutputStep::setOutput(int output) {
    if (output < 0) {
        this->output = 0;
    } else if (output >= MAX_OUTPUTS - 1) {
        this->output = MAX_OUTPUTS - 1;
    } else {
        this->output = output;
    }
    changed = true;
}
