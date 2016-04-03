#include "Ticking.h"

TickBuffer::TickBuffer(int resolution) {
    period = 1000 / resolution;
    bpm = 120.;
    for (int i = 0; i < TICK_BUFFER_SIZE; i++) {
        buffer[i] = NULL;
    }
    this->changes = new ChangeSet(true);
    this->reset();
}

TickBuffer::~TickBuffer() {
    clear();
}

void TickBuffer::reset() {
    xruns = 0;
    ticks = -1; // first tick is never of full length
    last_time = now();
    clear();
}

void TickBuffer::clear() {
    position = 0;
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
    }

    long long this_time = now();
    period = this_time - last_time;
    while (true) {
        TickEvent* next_event = buffer[position];
        if (next_event != NULL) {
            long long next_time = next_event->time;
            if ((next_time <= this_time) || ((next_time - this_time) < (period / 2))) {
                ofLogNotice("TickBuffer") << "Time delta: " << (this_time - next_time);
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
    last_time = this_time;
}

void TickBuffer::push(TickEvent* event) {
    for (int i = 0; i < TICK_BUFFER_SIZE; i++) {
        int suggested = (i + position) % TICK_BUFFER_SIZE;
        if (buffer[suggested] == NULL) {
            buffer[suggested] = event;
            return;
        } else if (event->time < buffer[suggested]->time) {
            for (int j = TICK_BUFFER_SIZE - i - 1; j >= 0; j--) {
                int move_from = (i + j + position) % TICK_BUFFER_SIZE;
                int move_to = (i + j + position + 1) % TICK_BUFFER_SIZE;
                if (buffer[move_from] != NULL) {
                    buffer[move_to] = buffer[move_from];
                }
            }
            buffer[suggested] = event;
            return;
        }
    }
    ofLogError("TickBuffer") << "WARNING: FULL RING XRUN!";
    xruns++;
}

long long TickBuffer::now() {
    return (long long) std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

bool TickBuffer::timeFor(long long time) {
    return time < (now() - period / 2);
}

void TickBuffer::hold(ChangeSet* changes) {
    if (changes != NULL) {
        this->changes->push(changes);
    }
}

ChangeSet* TickBuffer::release() {
    ChangeSet* changes_to_release = this->changes;
    this->changes = new ChangeSet(true);
    return changes_to_release;
}

void TickBuffer::draw(int x, int y) {
    int px, py, size;
    for (int i = 0; i < TICK_BUFFER_SIZE; i++) {
        px = x + (i % 16) * 5;
        py = y + (i / 16) * 5;
        if (buffer[i] == NULL) {
            ofSetColor(ofColor::gray);
        } else if (i == position) {
            ofSetColor(ofColor::yellow);
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

float TickBuffer::getBPM() { return bpm; }

void TickBuffer::setBPM(float bpm) {
    if (bpm < 1.) {
        this->bpm = 1;
    } else if (bpm > MAX_BPM) {
        this->bpm = MAX_BPM;
    } else {
        this->bpm = bpm;
    }
}

int TickBuffer::getPeriod() { return period; }

int TickBuffer::getXRuns() { return xruns; }

long long TickBuffer::getLastTime() { return last_time; }
