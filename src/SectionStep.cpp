#include "SectionStep.h"

SectionStep::SectionStep(int section) {
    type = STEP_TYPE_SECTION;
    this->section = section;

    tool_0 = new Tool("SEC\n0", '0',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 0));
    tool_1 = new Tool("SEC\n1", '1',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 1));
    tool_2 = new Tool("SEC\n2", '2',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 2));
    tool_3 = new Tool("SEC\n3", '3',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 3));
    tool_4 = new Tool("SEC\n4", '4',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 4));
    tool_5 = new Tool("SEC\n5", '5',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 5));
    tool_6 = new Tool("SEC\n6", '6',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 6));
    tool_7 = new Tool("SEC\n7", '7',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 7));
    tool_8 = new Tool("SEC\n8", '8',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 8));
    tool_9 = new Tool("SEC\n9", '9',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_SET, 9));
    tool_up = new Tool("+\nSEC", '+',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_DELTA, 1));
    tool_down = new Tool("-\nSEC", '-',
        new Change(TARGET_LEVEL_STEP, OP_SECTION_DELTA, -1));
}

SectionStep::~SectionStep() {
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

void SectionStep::execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer) {
    if (section == buffer->getActiveSection()) {
        changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
    }
}

void SectionStep::click(ChangeSet* changes) {
    changes->upstream->push(new Change(TARGET_LEVEL_APPLICATION, OP_SECTION_SET, section));
}

void SectionStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    ofSetColor(200);
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);

    ofSetColor(0);
    font.drawString("SEC", x + 3, y + 13);
    font.drawString(ofToString(section), x + 3, y + 33);
}

void SectionStep::populate(Toolbar* toolbar) {
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

void SectionStep::change(ChangeSet* changes) {
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_STEP)) != NULL) {
        switch (change->operation) {
            case OP_SECTION_SET:
                setSection(change->value);
                break;
            case OP_SECTION_DELTA:
                setSection(section + change->value);
                break;
        }
    }
}

void SectionStep::write(ofstream& f) {
    f << "delta-cursor 1\n"
      << "add-section-step " << ofToString(section) << "\n";
}

int SectionStep::getSection() { return section; }

void SectionStep::setSection(int section) {
    section = section < 0 ? 0 : section;
    section = section < MAX_SECTIONS ? section : MAX_SECTIONS - 1;
    this->section = section;
    changed = true;
}
