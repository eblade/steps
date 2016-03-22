#include "Midi.h"

MidiEvent::MidiEvent(long long time, int output, int note, int velocity) {
    this->time = time;
    this->output = output;
    this->note = note;
    this->velocity = velocity;
}

void MidiEvent::fire() {
    cout << "MIDI! " << note << " @" << output << " note " << note << "/" << velocity << endl;
}
