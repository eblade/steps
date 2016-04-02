#include "DivisionStep.h"

const ofColor DivisionStep::c_text(0, 200, 100);

DivisionStep::DivisionStep(int numerator, int denominator, int tuplet) : Step() {
    type = STEP_TYPE_DIVISION;
    division = new Division(numerator, denominator, tuplet);

    tool_1 = new Tool("1\n1", '1',
        new Change(TARGET_LEVEL_STEP, OP_DENOMINATOR_SET, 1));
    tool_1_2 = new Tool("1\n2", '2',
        new Change(TARGET_LEVEL_STEP, OP_DENOMINATOR_SET, 2));
    tool_1_4 = new Tool("1\n4", '3',
        new Change(TARGET_LEVEL_STEP, OP_DENOMINATOR_SET, 4));
    tool_1_8 = new Tool("1\n8", '4',
        new Change(TARGET_LEVEL_STEP, OP_DENOMINATOR_SET, 8));
    tool_1_16 = new Tool("1\n16", '5',
        new Change(TARGET_LEVEL_STEP, OP_DENOMINATOR_SET, 16));
    tool_1_32 = new Tool("1\n32", '6',
        new Change(TARGET_LEVEL_STEP, OP_DENOMINATOR_SET, 32));
    tool_1_64 = new Tool("1\n64", '7',
        new Change(TARGET_LEVEL_STEP, OP_DENOMINATOR_SET, 64));

    tool_triplet = new Tool("T", 't',
        new Change(TARGET_LEVEL_STEP, OP_TUPLET_SET, 3));
    tool_no_tuplet = new Tool("no T", 't',
        new Change(TARGET_LEVEL_STEP, OP_TUPLET_SET, 1));

    tool_tuplet_up = new Tool("+\nT", '+',
        new Change(TARGET_LEVEL_STEP, OP_TUPLET_DELTA, 1));
    tool_tuplet_down = new Tool("-\nT", '-',
        new Change(TARGET_LEVEL_STEP, OP_TUPLET_DELTA, -1));
}

DivisionStep::~DivisionStep() {
    delete division;
    delete tool_1;
    delete tool_1_2;
    delete tool_1_4;
    delete tool_1_8;
    delete tool_1_16;
    delete tool_1_32;
    delete tool_1_64;
    delete tool_triplet;
    delete tool_no_tuplet;
    delete tool_tuplet_up;
    delete tool_tuplet_down;
}

ChangeSet* DivisionStep::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet* changes = new ChangeSet();
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_PERIOD_SET, division->getPeriod(buffer->bpm)));
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
    return changes;
}

void DivisionStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    ofSetColor(c_text);
    ofDrawRectangle(x + STEP_SPACING, y + 18, STEP_INNER, 1);
    font.drawString(division->getNumeratorString(), x + 6, y + 13);
    font.drawString(division->getDenominatorString(), x + 6, y + 33);
}

void DivisionStep::populate(Toolbar* toolbar) {
    toolbar->push(tool_1);
    toolbar->push(tool_1_2);
    toolbar->push(tool_1_4);
    toolbar->push(tool_1_8);
    toolbar->push(tool_1_8);
    toolbar->push(tool_1_16);
    toolbar->push(tool_1_32);
    toolbar->push(tool_1_64);
    if (division->tuplet > 1) {
        toolbar->push(tool_no_tuplet);
    } else {
        toolbar->push(tool_triplet);
    }
    toolbar->push(tool_tuplet_up);
    toolbar->push(tool_tuplet_down);
}

void DivisionStep::change(ChangeSet* changes) {
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_STEP)) != NULL) {
        switch (change->operation) {
            case OP_NUMERATOR_SET:
                division->numerator = change->value;
                break;
            case OP_DENOMINATOR_SET:
                division->denominator = change->value;
                break;
            case OP_TUPLET_SET:
                division->tuplet = change->value;
                break;
            case OP_TUPLET_DELTA:
                division->tuplet += change->value;
                division->tuplet = division->tuplet > 11 ? 11 : division->tuplet;
                division->tuplet = division->tuplet < 1 ? 1 : division->tuplet;
                break;
        }
    }
}

void DivisionStep::write(ofstream& f) {
    f << "delta-cursor 1\n"
      << "add-division-step\n" 
      << "set-numerator " << ofToString(division->numerator) << "\n" 
      << "set-denominator " << ofToString(division->denominator) << "\n" 
      << "set-tuplet " << ofToString(division->tuplet) << "\n";
}

Division::Division(int numerator, int denominator, int tuplet) {
    this->numerator = numerator;
    this->denominator = denominator;
    this->tuplet = tuplet;
}

int Division::getPeriod(float bpm) {
    float base = ((float) numerator) / ((float) denominator);
    if (tuplet > 1) {
        base *= ((float) tuplet - 1) / ((float) tuplet );
    }
    return (int) (base / (bpm / 60. / 4.) * 1000.);
}

string Division::getNumeratorString() {
    return ofToString(numerator);
}

string Division::getDenominatorString() {
    string result = ofToString(denominator);
    if (tuplet == 3) {
        result += "T";
    } else if (tuplet > 1) {
        result += "~" + ofToString(tuplet);
    }
    return result;
}
