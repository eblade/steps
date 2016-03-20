#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "ofMain.h"
#include "Point.h"
#include "NotePoint.h"
#include "ActivatePoint.h"
#include "Ticking.h"

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
        void cursorNote(int value);
        void setCursor(int wanted);
        int getLength();
        void step(TickBuffer* buffer);
        void change(ChangeSet changes);

        string name;
        int cursor;
        int division;
        bool active;
        int octave;
        int position;
        int last_executed;
        long long release;
        Point* data[MAX_LENGTH];
};

#endif /* SEQUENCER_H_ */
