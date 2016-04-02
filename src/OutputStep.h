#ifndef OUTPUTSTEP_H_
#define OUTPUTSTEP_H_

#include "ofMain.h"
#include "Step.h"
#include "Change.h"
#include "Output.h"
#include "Toolbar.h"

class OutputStep : public Step {
    public:
        OutputStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet* execute(TickBuffer* buffer, SequencerState sequencer);
        ChangeSet* click();
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
        void write(ofstream& f);

        int getOutput();
        void setOutput(int output);

        Tool* tool_0;
        Tool* tool_1;
        Tool* tool_2;
        Tool* tool_3;
        Tool* tool_4;
        Tool* tool_5;
        Tool* tool_6;
        Tool* tool_7;
        Tool* tool_8;
        Tool* tool_9;
        Tool* tool_up;
        Tool* tool_down;

    private:
        int output;
};

#endif
