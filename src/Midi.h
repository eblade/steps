#ifndef MIDI_H_
#define MIDI_H_

#include "Ticking.h"


class MidiEvent : public TickEvent {
    public:
        MidiEvent(long long time, int output, int note, int velocity);
        void fire();
        int output;
        int note;
        int velocity;
};


#endif /* MIDI_H_ */
