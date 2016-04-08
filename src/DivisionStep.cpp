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
    changes->push(
        new Change(TARGET_LEVEL_SEQUENCER, OP_PERIOD_SET, division->getPeriod(buffer->getBPM())));
    changes->push(
        new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
    return changes;
}

void DivisionStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    ofSetColor(20);
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);
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
    if (division->getTuplet() > 1) {
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
                division->setNumerator(change->value);
                break;
            case OP_NUMERATOR_DELTA:
                division->setNumerator(division->getNumerator() + change->value);
                break;
            case OP_DENOMINATOR_SET:
                division->setDenominator(change->value);
                break;
            case OP_DENOMINATOR_DELTA:
                division->setDenominator(division->getDenominator() + change->value);
                break;
            case OP_TUPLET_SET:
                division->setTuplet(change->value);
                break;
            case OP_TUPLET_DELTA:
                division->setTuplet(division->getTuplet() + change->value);
                break;
        }
    }
}

void DivisionStep::write(ofstream& f) {
    f << "delta-cursor 1\n"
      << "add-division-step\n" 
      << "set-numerator " << ofToString(division->getNumerator()) << "\n" 
      << "set-denominator " << ofToString(division->getDenominator()) << "\n" 
      << "set-tuplet " << ofToString(division->getTuplet()) << "\n";
}

bool DivisionStep::needsRedraw() {
    if (division->needsRedraw() || changed) {
        changed = false;
        return true;
    } else {
        return false;
    }
}

Division::Division(int numerator, int denominator, int tuplet) {
    setNumerator(numerator);
    setDenominator(denominator);
    setTuplet(tuplet);
    changed = true;
}

int Division::getNumerator() { return numerator; }

void Division::setNumerator(int numerator) {
    if (numerator < 1) {
        this->numerator = 1;
    } else if (numerator > 64) {
        this->numerator = 64;
    } else {
        this->numerator = numerator;
    }
    changed = true;
}

int Division::getDenominator() { return denominator; }

void Division::setDenominator(int denominator) {
    if (denominator < 1) {
        this->denominator = 1;
    } else if (denominator > 64) {
        this->denominator = 64;
    } else {
        this->denominator = denominator;
    }
    changed = true;
}

int Division::getTuplet() { return tuplet; }

void Division::setTuplet(int tuplet) {
    if (tuplet < 1) {
        this->tuplet = 1;
    } else if (tuplet > 13) {
        this->tuplet = 13;
    } else {
        this->tuplet = tuplet;
    }
    changed = true;
}

double Division::getPeriod(double bpm) {
    double base = ((double) numerator) / ((double) denominator);
    if (tuplet > 1) {
        base *= ((double) tuplet - 1) / ((double) tuplet );
    }
    return base / (bpm / 60. / 4.);
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

bool Division::needsRedraw() {
    if (changed) {
        changed = false;
        return true;
    } else {
        return false;
    }
}
