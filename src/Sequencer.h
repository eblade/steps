#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "ofMain.h"
#include "Const.h"
#include "Step.h"
#include "NoteStep.h"
#include "ActivateStep.h"
#include "OutputStep.h"
#include "DivisionStep.h"
#include "SyncStep.h"
#include "Ticking.h"
#include "Output.h"
#include "Toolbar.h"


class Sequencer {
    public:
        Sequencer();

        void draw(int row, bool inThisRow, ofTrueTypeFont font, bool redraw_all=false);

        void cursorLeft();
        void cursorRight();
        void cursorClick();
        void cursorDelete();
        void cursorInsert(Step* step);
        void cursorBlank();
        void cursorHome();
        void cursorEnd();
        int getCursor();
        void setCursor(int wanted);
        int getLength();
        void step(TickBuffer* buffer, OutputRouter* output_router);
        void change(ChangeSet* changes, TickBuffer* buffer);
        void populate(Toolbar* toolbar);
        void sync();
        void write(ofstream& f);
        void setPosition(int position);
        int getLabel();

    private:
        bool redraw;
        int cursor;
        int octave;
        int position;
        int last_executed;
        double release;
        int output;
        double period;
        int label;
        int cursor_shade;
        int cursor_blank;
        bool synced;
        Step* data[MAX_STEPS];
};

#endif /* SEQUENCER_H_ */
