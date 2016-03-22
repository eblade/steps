#ifndef TICKING_H_
#define TICKING_H_

#include "ofMain.h"
#include <chrono>

#define TICK_BUFFER_SIZE 256

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
        void tick();
        void push(TickEvent* event);
        void draw(int x, int y);
        bool timeFor(long long time);

        long long start_time;
        int period;
        int xruns;
        long long relative_time;

    private:
        TickEvent* buffer[TICK_BUFFER_SIZE];
        long long now();
        long long position;
        long ticks;
        long long last_time;
};


#endif /* TICKING_H_ */
