#include "DivisionStep.h"

const ofColor DivisionStep::c_text(0, 200, 100);

DivisionStep::DivisionStep(int numerator, int denominator, int tuplet) : Step() {
    type = STEP_TYPE_DIVISION;
    division = new Division(numerator, denominator, tuplet);
}

DivisionStep::~DivisionStep() {
    delete division;
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
