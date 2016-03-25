#include "DivisionPoint.h"

const ofColor DivisionPoint::c_text(0, 200, 100);

DivisionPoint::DivisionPoint(int numerator, int denominator, int tuplet) : Point() {
    type = POINT_TYPE_DIVISION;
    division = new Division(numerator, denominator, tuplet);
}

DivisionPoint::~DivisionPoint() {
    delete division;
}

ChangeSet DivisionPoint::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet changes;
    changes.period = division->getPeriod(buffer->bpm);
    changes.position_delta = 1;
    return changes;
}

void DivisionPoint::draw(int x, int y, bool executing, ofTrueTypeFont font) {
    ofSetColor(c_text);
    ofDrawRectangle(x + POINT_SPACING, y + 18, POINT_INNER, 1);
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
