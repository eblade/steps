#include "OutputStep.h"

OutputStep::OutputStep() {
    type = STEP_TYPE_OUTPUT;
    output = 0;

    tool_0 = new PersistantTool("OUT\n0", '0',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 0));
    tool_1 = new PersistantTool("OUT\n1", '1',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 1));
    tool_2 = new PersistantTool("OUT\n2", '2',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 2));
    tool_3 = new PersistantTool("OUT\n3", '3',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 3));
    tool_4 = new PersistantTool("OUT\n4", '4',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 4));
    tool_5 = new PersistantTool("OUT\n5", '5',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 5));
    tool_6 = new PersistantTool("OUT\n6", '6',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 6));
    tool_7 = new PersistantTool("OUT\n7", '7',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 7));
    tool_8 = new PersistantTool("OUT\n8", '8',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 8));
    tool_9 = new PersistantTool("OUT\n9", '9',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET, 9));
    tool_up = new PersistantTool("+\nOUT", '+',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_DELTA, 1));
    tool_down = new PersistantTool("-\nOUT", '-',
        new Change(TARGET_LEVEL_STEP, OP_OUTPUT_DELTA, -1));
}

ChangeSet* OutputStep::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet* changes = new ChangeSet();
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_OUTPUT_SET, output));
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
    return changes;
}

ChangeSet* OutputStep::click() {
    ChangeSet* changes = new ChangeSet();
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_OUTPUT_SET, output));
    return changes;
}

void OutputStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    ofSetColor(OutputColors::color[output % OUTPUT_COLORS]);
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);

    ofSetColor(ofColor::white);
    font.drawString("OUT", x + 3, y + 13);
    font.drawString(ofToString(output), x + 3, y + 33);
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
                output = change->value;
                break;
            case OP_OUTPUT_DELTA:
                output += change->value;
                output %= MAX_OUTPUTS;
                break;
        }
    }
}

