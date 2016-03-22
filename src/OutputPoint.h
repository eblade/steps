#ifndef OUTPUTPOINT_H_
#define OUTPUTPOINT_H_

#include "ofMain.h"
#include "Point.h"
#include "Output.h"

class OutputPoint : public Point {
    public:
        OutputPoint();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet execute(TickBuffer* buffer, SequencerState sequencer);
        ChangeSet click();

    private:
        int output;
        static const ofColor
            c_has_output,
            c_has_no_output;
};

#endif
