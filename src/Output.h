#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "ofMain.h"
#include "ofxMidi.h"
#include "Const.h"

#define OUTPUT_COLORS 16


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
        ~OutputRouter();
        bool install(int address, OutputSettings settings);
        void uninstall(int address);
        void send(int address, OutputEvent event);
        string getOutputString(int address);
        int getPeak(int address);

    private:
        void sendDummy(OutputSettings settings, OutputEvent event);
        void sendMidi(OutputSettings settings, OutputEvent event);
        OutputSettings output[MAX_OUTPUTS];
        ofxMidiOut* midi_output[MAX_OUTPUT_DEVICES];
        int peak[MAX_OUTPUTS];
};

class OutputColors {
    public:
        static const ofColor color[OUTPUT_COLORS];
};

#endif /* OUTPUT_H_ */
