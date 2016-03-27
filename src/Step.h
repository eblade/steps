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
        period(0)
        {}
    int output;
    int period;
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

        int type;
        bool active;
};

class DummyEvent : public TickEvent {
    public:
        DummyEvent(string name, long long time);
        void fire();
        string name;
};

#endif /* STEP_H_ */
