#ifndef ACTIVATESTEP_H_
#define ACTIVATESTEP_H_

#include "Step.h"
#include "Change.h"
#include "Toolbar.h"

class ActivateStep : public Step {
    public:
        ActivateStep();
        ~ActivateStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet* execute(TickBuffer* buffer, SequencerState sequencer);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
        ChangeSet* click();
        void write(ofstream& f);

        int getLabel();
        void setLabel(int label);
        bool getHold();
        void setHold(bool hold);

        Tool* tool_hold;
        Tool* tool_dont_hold;
        Tool* tool_kill;
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

        static const ofColor
            c_on,
            c_off;

    private:
        int label;
        bool hold;
        bool changed_label;
};


#endif /* ACTIVATESTEP_H_ */
