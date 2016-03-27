#ifndef MIDI_H_
#define MIDI_H_

#include "ofMain.h"
#include "Ticking.h"
#include "Output.h"


class MidiEvent : public TickEvent {
    public:
        MidiEvent(long long time, OutputRouter* output_router, int output, int note, int velocity);
        ~MidiEvent();
        void fire();
        OutputRouter* output_router;
        int output;
        int note;
        int velocity;
        bool fired;
};


#endif /* MIDI_H_ */
