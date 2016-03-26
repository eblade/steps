#ifndef ACTIVATESTEP_H_
#define ACTIVATESTEP_H_

#include "Step.h"

class ActivateStep : public Step {
    public:
        ActivateStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet click();
        ChangeSet execute(TickBuffer* buffer, SequencerState sequencer);

        static const ofColor
            c_on,
            c_off;
};


#endif /* ACTIVATESTEP_H_ */
