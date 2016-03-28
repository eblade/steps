#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "ofMain.h"
#include "Const.h"
#include "Step.h"
#include "NoteStep.h"
#include "ActivateStep.h"
#include "OutputStep.h"
#include "DivisionStep.h"
#include "Ticking.h"
#include "Output.h"
#include "Toolbar.h"


class Sequencer {
    public:
        Sequencer();

        void draw(int row, bool inThisRow, ofTrueTypeFont font);

        void cursorLeft();
        void cursorRight();
        void cursorClick();
        void cursorDelete();
        void cursorInsert(Step* step);
        void setCursor(int wanted);
        int getLength();
        void step(TickBuffer* buffer, OutputRouter* output_router);
        void change(ChangeSet* changes, TickBuffer* buffer);
        void populate(Toolbar* toolbar);
        void sync();

        string name;
        int cursor;
        bool active;
        int octave;
        int position;
        int last_executed;
        long long release;
        int output;
        int period;
        Step* data[MAX_STEPS];
};

#endif /* SEQUENCER_H_ */
