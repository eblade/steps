#include "Ticking.h"

TickBuffer::TickBuffer(int resolution) {
    average = 1000 / resolution;
    this->reset();
}

void TickBuffer::reset() {
    position = 0;
    ticks = -1; // first tick is never of full length
    start_time = now();
    last_time = 0;
    for (int i = 0; i < TICK_BUFFER_SIZE; i++) {
        if (buffer[i] != NULL) {
            delete buffer[i];
        }
        buffer[i] = NULL;
    }
}

void TickBuffer::tick() {
    if (ticks == -1) {
        last_time = now();
        ticks = 0;
    } else {
        long long this_time = now();
        while (true) {
            TickEvent* next_event = buffer[position];
            if (next_event != NULL) {
                long long next_time = next_event->time;
                if ((next_time <= this_time) || ((next_time - this_time) < (average / 2))) {
                    next_event->fire();
                    delete next_event;
                    buffer[position] = NULL;
                    position++;
                    position = position % TICK_BUFFER_SIZE;
                } else {
                    break;
                }
            }
        }
        ticks++;
    }
}

long long TickBuffer::now() {
    return (long long) std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}
