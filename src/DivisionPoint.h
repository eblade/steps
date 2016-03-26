#ifndef DIVSIONSTEP_H_
#define DIVSIONSTEP_H_


#include "Step.h"


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


class DivisionStep : public Step {
    public:
        DivisionStep(int numerator, int denominator, int tuplet);
        ~DivisionStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet execute(TickBuffer* buffer, SequencerState sequencer);
        //ChangeSet click();

        Division* division;

    private:
        static const ofColor c_text;
};

#endif /* DIVISIONSTEP_H_ */
