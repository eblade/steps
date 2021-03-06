#ifndef SYNCSTEP_H_
#define SYNCSTEP_H_

#include "Step.h"
#include "Change.h"
#include "Toolbar.h"


class SyncStep : public Step {
    public:
        SyncStep(int label);
        ~SyncStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font, ofTrueTypeFont font_big);
        void execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
        void click(ChangeSet* changes);
        int getLength();
        void write(ofstream& f);

        int getLabel();
        void setLabel(int label);

        Tool* tool_activate;
        Tool* tool_deactivate;
        Tool* tool_label_0;
        Tool* tool_label_1;
        Tool* tool_label_2;
        Tool* tool_label_3;
        Tool* tool_label_4;
        Tool* tool_label_5;
        Tool* tool_label_6;
        Tool* tool_label_7;
        Tool* tool_label_8;
        Tool* tool_label_9;
        Tool* tool_label_up;
        Tool* tool_label_down;

    private:
        int label;
        int length;
        static const ofColor
            c_active,
            c_inactive,
            c_sel_active,
            c_sel_inactive;
};

#endif /* NOTESTEP_H_ */
