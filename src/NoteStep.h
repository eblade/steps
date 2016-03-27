#ifndef NOTESTEP_H_
#define NOTESTEP_H_

#include "Step.h"
#include "Change.h"
#include "Midi.h"
#include "Output.h"
#include "Toolbar.h"


class NoteStep : public Step {
    public:
        NoteStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet* execute(TickBuffer* buffer, SequencerState sequencer);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
        int getLength();

        int note;
        int octave;
        int velocity;
        OutputRouter* output_router;

        Tool* tool_note_0;
        Tool* tool_note_1;
        Tool* tool_note_2;
        Tool* tool_note_3;
        Tool* tool_note_4;
        Tool* tool_note_5;
        Tool* tool_note_6;
        Tool* tool_note_7;
        Tool* tool_note_8;
        Tool* tool_note_9;
        Tool* tool_note_10;
        Tool* tool_note_11;
        Tool* tool_note_up;
        Tool* tool_note_down;
        Tool* tool_octave_up;
        Tool* tool_octave_down;
        Tool* tool_accent;
        Tool* tool_normal;

    private:
        int length;
        static const ofColor
            c_active,
            c_inactive,
            c_sel_active,
            c_sel_inactive;
        static const char* note_name[];
};

#endif /* NOTESTEP_H_ */
