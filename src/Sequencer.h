#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "ofMain.h"
#include "Point.h"
#include "NotePoint.h"
#include "ActivatePoint.h"
#include "OutputPoint.h"
#include "Ticking.h"
#include "Output.h"

#define MAX_LENGTH 20


class Sequencer {
    public:
        Sequencer();

        void draw(int row, bool inThisRow, ofTrueTypeFont font);

        void cursorLeft();
        void cursorRight();
        void cursorClick();
        void cursorDelete();
        bool cursorInsert(Point* point);
        void cursorHold();
        void cursorOutput();
        void cursorNote(int note);
        void setCursor(int wanted);
        int getLength();
        void step(TickBuffer* buffer, OutputRouter* output_router);
        void change(ChangeSet changes);

        string name;
        int cursor;
        int division;
        bool active;
        int octave;
        int position;
        int last_executed;
        long long release;
        int output;
        Point* data[MAX_LENGTH];
};

#endif /* SEQUENCER_H_ */
