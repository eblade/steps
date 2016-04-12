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
        double time;
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
        bool timeFor(double time);

        float getBPM();
        void setBPM(float bpm);
        int getXRuns();
        double getLastTime();
        bool isFresh();
        int getActiveSection();
        void setActiveSection(int section);

    private:
        TickEvent* buffer[TICK_BUFFER_SIZE];
        double now();
        int position;
        long ticks;
        ChangeSet* changes;
        double bpm;
        double last_time;
        double period;
        int xruns;
        int active_section;
};


#endif /* TICKING_H_ */
