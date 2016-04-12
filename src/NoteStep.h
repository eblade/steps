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
        ~NoteStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        void execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
        int getLength();
        void write(ofstream& f);

        int getNote();
        void setNote(int note);
        int getOctave();
        void setOctave(int octave);
        int getVelocity();
        void setVelocity(int velocity);

        Tool* tool_activate;
        Tool* tool_deactivate;
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
        int note;
        int octave;
        int velocity;
        int length;
        static const ofColor
            c_active,
            c_inactive,
            c_sel_active,
            c_sel_inactive;
        static const char* note_name[];
};

#endif /* NOTESTEP_H_ */
