#ifndef OUTPUTSTEP_H_
#define OUTPUTSTEP_H_

#include "ofMain.h"
#include "Step.h"
#include "Change.h"
#include "Output.h"

class OutputStep : public Step {
    public:
        OutputStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet* execute(TickBuffer* buffer, SequencerState sequencer);
        ChangeSet* click();
        int output;

    private:
        static const ofColor
            c_has_output,
            c_has_no_output;
};

#endif
