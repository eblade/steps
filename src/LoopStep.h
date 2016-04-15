#ifndef LOOPSTEP_H_
#define LOOPSTEP_H_

#include "ofMain.h"
#include "Step.h"
#include "Change.h"
#include "Toolbar.h"

class LoopStep : public Step {
    public:
        LoopStep(int loops = 4);
        ~LoopStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font, ofTrueTypeFont font_big);
        void execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer);
        void click(ChangeSet* changes);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
        void write(ofstream& f);

        int getLoops();
        void setLoops(int loops);
        bool isVamp();
        void setVamp(bool vamp);
        bool isLast();
        void setLast();
        void reset();

        Tool* tool_set_vamp;
        Tool* tool_unset_vamp;
        Tool* tool_last;
        Tool* tool_reset;
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
        int count;
        int loops;
        bool vamp;
        bool last;
};

#endif /* LOOPSTEP_H_ */
