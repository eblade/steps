#ifndef STEP_H_
#define STEP_H_

#include "ofMain.h"
#include "Const.h"
#include "Ticking.h"
#include "Output.h"
#include "Change.h"
#include "Toolbar.h"

struct SequencerState {
    SequencerState () :
        output(0),
        period(0),
        label(0),
        release(0)
        {}
    int output;
    int period;
    int label;
    long long release;
    OutputRouter* output_router;
};

class Step {
    public:
        Step();
        virtual ~Step() {};
        virtual void draw(int x, int y, bool executing, ofTrueTypeFont font);
        virtual ChangeSet* click();
        virtual ChangeSet* execute(TickBuffer* buffer, SequencerState sequencer);
        virtual int getLength();
        virtual void change(ChangeSet* changes);
        virtual void populate(Toolbar* toolbar);
        virtual void write(ofstream& f);
        virtual bool needsRedraw();
        virtual void markForRedraw();

        virtual bool getActive();
        virtual void setActive(bool active);

    protected:
        int type;
        bool active;
        bool changed;
};

#endif /* STEP_H_ */
