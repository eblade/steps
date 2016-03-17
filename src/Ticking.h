#ifndef QUEUE_H_
#define QUEUE_H_

#include <chrono>

#define TICK_BUFFER_SIZE 256

class TickEvent {
    public:
        TickEvent();
        virtual ~TickEvent();
        virtual void fire();
        long long time;
};

class TickBuffer {
    public:
        TickBuffer(int resolution);
        void reset();
        void tick();
        void schedule(TickEvent* event, int time);

    private:
        long long now();
        TickEvent* buffer[TICK_BUFFER_SIZE];
        long long start_time;
        long long position;
        long ticks;
        long average;
        long long last_time;
};


#endif /* QUEUE_H_ */
