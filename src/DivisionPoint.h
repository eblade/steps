#ifndef DIVSIONPOINT_H_
#define DIVSIONPOINT_H_

#include "Point.h"


class Division {
    public: 
        Division(int numerator, int denominator, int tuplet);
        int getPeriod(float bpm);
        string getNumeratorString();
        string getDenominatorString();

        int numerator;
        int denominator;
        int tuplet;
};


class DivisionPoint : public Point {
    public:
        DivisionPoint(int numerator, int denominator, int tuplet);
        ~DivisionPoint();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet execute(TickBuffer* buffer, SequencerState sequencer);
        //ChangeSet click();

        Division* division;

    private:
        static const ofColor c_text;
};

#endif /* DIVISIONPOINT_H_ */
