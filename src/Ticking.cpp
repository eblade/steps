#include "Ticking.h"

TickBuffer::TickBuffer(int resolution) {
    period = 1. / ((double) resolution);
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
    active_section = 0;
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

    double this_time = now();
    period = this_time - last_time;
    TickEvent* next_event = buffer[position];
    if (next_event != NULL) {
        double next_time = next_event->time;
        if (next_time < (this_time - period)) {
            ofLogNotice("TickBuffer") << "now=" << ((long long) (this_time * 1000.))
                                      << " event=" << ((long long) (next_time * 1000.))
                                      << " period=" << period
                                      << " delta=" << (this_time - next_time);
            next_event->fire();
            delete next_event;
            buffer[position] = NULL;
            position++;
            position = position % TICK_BUFFER_SIZE;
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

double TickBuffer::now() {
    return ((double) std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count()) / 1000000.;
}

bool TickBuffer::timeFor(double time) {
    return time < (now() - period / 2.);
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
    int px, py, size, delta;
    double now = this->now();
    for (int i = 0; i < TICK_BUFFER_SIZE; i++) {
        px = x + (i % 16) * 5;
        py = y + (i / 16) * 5;
        if (buffer[i] == NULL) {
            ofSetColor(ofColor::gray);
        } else {
            delta = buffer[i]->time - now;
            if (delta >= 0) {
                ofSetColor(ofColor::green);
            } else {
                ofSetColor(ofColor::red);
            }
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

int TickBuffer::getXRuns() { return xruns; }

double TickBuffer::getLastTime() { return last_time; }

bool TickBuffer::isFresh() {
    return ticks == -1;
}

int TickBuffer::getActiveSection() { return active_section; }

void TickBuffer::setActiveSection(int section) {
    section = section < 0 ? 0 : section;
    section = section < MAX_SECTIONS ? section : MAX_SECTIONS - 1;
    active_section = section;
}
