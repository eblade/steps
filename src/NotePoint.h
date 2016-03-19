#ifndef NOTEPOINT_H_
#define NOTEPOINT_H_

#include "Point.h"


class NotePoint : public Point {
    public:
        NotePoint();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        void click();
        ExecutionResult execute(long long time, TickBuffer* buffer);
        int getLength();

        int value;


    private:
        int length;
        static const ofColor
            c_active,
            c_inactive,
            c_sel_active,
            c_sel_inactive;
};

#endif /* NOTEPOINT_H_ */
