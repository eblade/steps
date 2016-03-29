#ifndef TICKING_H_
#define TICKING_H_

#include "ofMain.h"
#include <chrono>
#include "Const.h"
#include "Change.h"


class TickEvent {
    public:
        TickEvent() { time = 0; }
        virtual ~TickEvent() {}
        virtual void fire() = 0;
        long long time;
};

class TickBuffer {
    public:
        TickBuffer(int resolution);
        ~TickBuffer();
        void reset();
        void clear();
        void tick();
        void push(TickEvent* event);
        void draw(int x, int y);
        bool timeFor(long long time);
        void hold(ChangeSet* changes);
        ChangeSet* release();

        long long sync_time;
        long long last_time;
        int period;
        int xruns;
        float bpm;

    private:
        TickEvent* buffer[TICK_BUFFER_SIZE];
        long long now();
        long long position;
        long ticks;
        ChangeSet* changes;
};


#endif /* TICKING_H_ */
