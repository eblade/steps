#ifndef DIVSIONSTEP_H_
#define DIVSIONSTEP_H_


#include "Step.h"
#include "Change.h"
#include "Toolbar.h"
#include "Output.h"


class Division {
    public: 
        Division(int numerator, int denominator, int tuplet);
        int getPeriod(float bpm);
        string getNumeratorString();
        string getDenominatorString();
        bool needsRedraw();

        int getNumerator();
        void setNumerator(int numerator);
        int getDenominator();
        void setDenominator(int denominator);
        int getTuplet();
        void setTuplet(int tuplet);

    private:
        int numerator;
        int denominator;
        int tuplet;
        bool changed;
};


class DivisionStep : public Step {
    public:
        DivisionStep(int numerator=1, int denominator=8, int tuplet=1);
        ~DivisionStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet* execute(TickBuffer* buffer, SequencerState sequencer);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
        void write(ofstream& f);
        bool needsRedraw();

        Division* division;

        Tool* tool_1;
        Tool* tool_1_2;
        Tool* tool_1_4;
        Tool* tool_1_8;
        Tool* tool_1_16;
        Tool* tool_1_32;
        Tool* tool_1_64;
        Tool* tool_triplet;
        Tool* tool_no_tuplet;
        Tool* tool_tuplet_up;
        Tool* tool_tuplet_down;

    private:
        static const ofColor c_text;
};

#endif /* DIVISIONSTEP_H_ */
