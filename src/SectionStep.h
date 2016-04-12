#ifndef SECTIONSTEP_H_
#define SECTIONSTEP_H_

#include "ofMain.h"
#include "Step.h"
#include "Change.h"
#include "Toolbar.h"

class SectionStep : public Step {
    public:
        SectionStep(int section = 0);
        ~SectionStep();

        void draw(int x, int y, bool executing, ofTrueTypeFont font, ofTrueTypeFont font_big);
        void execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer);
        void click(ChangeSet* changes);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
        void write(ofstream& f);

        int getSection();
        void setSection(int section);

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
        int section;
};

#endif /* SECTIONSTEP_H_ */
