#ifndef STEP_H_
#define STEP_H_

#include "ofMain.h"
#include "Ticking.h"
#include "Output.h"
#include "Const.h"

struct ChangeSet {
    ChangeSet() :
        position_delta(0),
        goto_position(-1),
        set_active(false),
        set_inactive(false),
        output(-1),
        period(-1)
        {}
    int position_delta;
    int goto_position;
    bool set_active;
    bool set_inactive;
    int output;
    int period;
    int release;
};

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
        virtual ChangeSet click();
        virtual ChangeSet execute(TickBuffer* buffer, SequencerState sequencer);
        virtual int getLength();

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
