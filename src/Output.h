#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "ofMain.h"

#define OUTPUT_MAX 16

#define OUTPUT_TYPE_DUMMY 0
#define OUTPUT_TYPE_MIDI 1

struct OutputSettings {
    OutputSettings() : used(false), type(0), device(-1), channel(0) {}
    bool used;
    int type;
    int device;
    int channel;
};

struct OutputEvent {
    OutputEvent() : note(0), velocity(0) {}
    int note;
    int velocity;
};

class OutputRouter {
    public:
        OutputRouter();
        bool install(int address, OutputSettings settings);
        void uninstall(int address);
        void send(int address, OutputEvent event);
        string getOutputString(int address);

    private:
        void sendDummy(OutputSettings settings, OutputEvent event);
        void sendMidi(OutputSettings settings, OutputEvent event);
        OutputSettings output[OUTPUT_MAX];
};

#endif /* OUTPUT_H_ */
