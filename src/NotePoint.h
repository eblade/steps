#ifndef NOTESTEP_H_
#define NOTESTEP_H_

#include "Step.h"
#include "Midi.h"
#include "Output.h"


class NoteStep : public Step {
    public:
        NoteStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet execute(TickBuffer* buffer, SequencerState sequencer);
        int getLength();

        int note;
        int velocity;
        OutputRouter* output_router;

    private:
        int length;
        static const ofColor
            c_active,
            c_inactive,
            c_sel_active,
            c_sel_inactive;
};

#endif /* NOTESTEP_H_ */
