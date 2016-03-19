#ifndef POINT_H_
#define POINT_H_

#define POINT_TYPE_NONE 0
#define POINT_TYPE_NOTE 1
#define POINT_TYPE_DIVISION 2
#define POINT_TYPE_ACTIVATE 3
#define POINT_TYPE_HOLD 4

#define POINT_INNER 46
#define POINT_OUTER 46
#define POINT_SPACING 2

#include "ofMain.h"
#include "Ticking.h"

struct ExecutionResult {
    ExecutionResult() : position_delta(0), goto_position(-1), set_active(false), set_inactive(false) {}
    int position_delta;
    int goto_position;
    bool set_active;
    bool set_inactive;
};

class Point {
    public:
        Point();
        virtual ~Point() {};
        virtual void draw(int x, int y, bool executing, ofTrueTypeFont font);
        virtual void click();
        virtual ExecutionResult execute(long long time, TickBuffer* buffer);
        virtual int getLength();

        int type;
        bool active;
};

class DummyEvent : public TickEvent {
    public:
        DummyEvent();
        void fire();
};

#endif /* POINT_H_ */
