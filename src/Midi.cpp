#include "Midi.h"

MidiEvent::MidiEvent(long long time, OutputRouter* output_router, int output, int note, int velocity) {
    this->time = time;
    this->output_router = output_router;
    this->output = output;
    this->note = note;
    this->velocity = velocity;
}

MidiEvent::~MidiEvent() {
    if (velocity == 0) {
        fire(); // This is a NoteOff, send it before dying
    }
}

void MidiEvent::fire() {
    ofLogNotice(APPLICATION) << "MIDI! " << note << " @" << output << " note " << note << "/" << velocity << endl;
    OutputEvent output_event;
    output_event.note = note;
    output_event.velocity = velocity;
    output_router->send(output, output_event);
}
