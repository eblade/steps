#include "Ticking.h"

TickBuffer::TickBuffer(int resolution) {
    average = 1000 / resolution;
    for (int i = 0; i < TICK_BUFFER_SIZE; i++) {
        buffer[i] = NULL;
    }
    this->reset();
}

void TickBuffer::reset() {
    position = 0;
    xruns = 0;
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
            } else {
                break;
            }
        }
        ticks++;
    }
}

void TickBuffer::push(TickEvent* event) {
    event->time += start_time;
    for (int i = 0; i < TICK_BUFFER_SIZE; i++) {
        int suggested = (i + position) % TICK_BUFFER_SIZE;
        if (buffer[suggested] == NULL) {
            buffer[suggested] = event;
            return;
        } else if (buffer[suggested]->time > event->time) {
            int j;
            for (j = 0; j > (TICK_BUFFER_SIZE - i - 1); j++) {
                int to_move = (i + j + position) % TICK_BUFFER_SIZE;
                if (buffer[to_move] == NULL) {
                    buffer[suggested] = event;
                    return;
                }
                buffer[(to_move + 1) % TICK_BUFFER_SIZE] = buffer[to_move];
            }
            buffer[suggested] = event;
            xruns++;
            return;
        }
    }
    xruns++;
}

long long TickBuffer::now() {
    return (long long) std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

bool TickBuffer::timeFor(long long time) {
    return start_time + time < (now() - average / 2);
}

void TickBuffer::draw(int x, int y) {
    int px, py, size;
    for (int i = 0; i < TICK_BUFFER_SIZE; i++) {
        px = x + (i % 16) * 5;
        py = y + (i / 16) * 5;
        if (buffer[i] == NULL) {
            ofSetColor(ofColor::gray);
        } else {
            ofSetColor(ofColor::green);
        }
        if (i == position) {
            size = 5;
        } else {
            size = 3;
            px++;
            py++;
        }
        ofDrawRectangle(px, py, size, size);
    }
}
