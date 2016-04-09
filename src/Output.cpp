#include "Output.h"

OutputRouter::OutputRouter() {
    for (int i = 0; i < MAX_OUTPUTS; i++) {
        output[i] = OutputSettings();
        peak[i] = 0;
    }
    for (int i = 0; i < MAX_OUTPUT_DEVICES; i++) {
        midi_output[i] = NULL;
    }
    ofxMidiOut::listPorts();
    active_output = 0;
};

OutputRouter::~OutputRouter() {
    for (int i = 0; i < MAX_OUTPUT_DEVICES; i++) {
        if (midi_output[i] != NULL) {
            ofLogNotice("OutputRouter") << "Closing midi port " << i 
                 << " \"" << midi_output[i]->getName() << "\"";
            midi_output[i]->closePort();
            delete midi_output[i];
            midi_output[i] = NULL;
        }
    };
};

bool OutputRouter::install(OutputSettings settings) {
    if (output[active_output].used) {
        ofLogError("OutputRouter") << "ERROR: Output address " << active_output << " in use";
        return true;
    }
    output[active_output].type = settings.type;
    output[active_output].device = settings.device;
    output[active_output].channel = settings.channel;
    output[active_output].used = true;
    if (settings.type == OUTPUT_TYPE_MIDI) {
        if (midi_output[settings.device] == NULL) {
            midi_output[settings.device] = new ofxMidiOut();
            midi_output[settings.device]->openPort(settings.device);
            ofLogNotice("OutputRouter") << "Opened midi port " << settings.device 
                 << " \"" << midi_output[settings.device]->getName() << "\"";
        }
    }
    return false;
}

void OutputRouter::uninstall() {
    output[active_output].used = false;
}

int OutputRouter::getChannel() { return output[active_output].channel; }

void OutputRouter::setChannel(int channel) {
    if (channel >= 0 && channel <= 16) {
        output[active_output].channel = channel;
    } else {
        ofLogError("OutputRouter") << "WARNING: Bad channel " << channel;
    }
}

int OutputRouter::getOutput() { return active_output; }

void OutputRouter::setOutput(int active_output) {
    if (active_output >= 0 && active_output < MAX_OUTPUTS) {
        this->active_output = active_output;
    } else {
        ofLogError("OutputRouter") << "WARNING: Bad output " << active_output;
    }
}

string OutputRouter::getOutputString(int address) {
    string active_marker = (address == active_output) ? ">" : " ";
    if (!(address >= 0 && address < MAX_OUTPUTS)) {
        return " " + ofToString(address) + ":OUT OF BOUNDS";
    } else if (output[address].used == false) {
        return active_marker + ofToString(address) + ":-";
    } else {
        if (output[address].type == OUTPUT_TYPE_DUMMY) {
            return active_marker + ofToString(address) + ":D" 
                                       + ofToString(output[address].device) + "/" 
                                       + ofToString(output[address].channel);
        } else if (output[address].type == OUTPUT_TYPE_MIDI) {
            return active_marker + ofToString(address) + ":M"
                                       + ofToString(output[address].device) + "/" 
                                       + ofToString(output[address].channel);
        }
    }
    return "?";
}

int OutputRouter::getPeak(int address) {
    peak[address]--;
    peak[address] = peak[address] >= 0 ? peak[address] : 0;
    return peak[address];
}

void OutputRouter::send(int address, OutputEvent event) {
    if (!(address >= 0 && address < MAX_OUTPUTS) || output[address].used == false) {
        ofLogError("OutputRouter") << "Output " << address << " is not installed";
        return;
    }
    switch (output[address].type) {
        case OUTPUT_TYPE_DUMMY:
            sendDummy(output[address], event);
            break;
        case OUTPUT_TYPE_MIDI:
            sendMidi(output[address], event);
            break;
    }
    peak[address] = 55;
}

void OutputRouter::sendDummy(OutputSettings settings, OutputEvent event) {
    ofLogNotice("OutputRouter")
        << "SEND DUMMY device=" << settings.device 
        << " channel=" << settings.channel
        << " note=" << event.note
        << " velocity=" << event.velocity;
}

void OutputRouter::sendMidi(OutputSettings settings, OutputEvent event) {
    ofLogNotice("OutputRouter")
        << "SEND MIDI device=" << settings.device 
        << " channel=" << settings.channel
        << " note=" << event.note
        << " velocity=" << event.velocity;
    if (midi_output[settings.device] == NULL) {
        cerr << "ERROR: Device " << settings.device << " is not installed";
        return;
    }
    midi_output[settings.device]->sendNoteOn(settings.channel, event.note, event.velocity);
}

const ofColor OutputColors::color[] = {
    ofColor(50, 50, 50),
    ofColor(100, 50, 50),
    ofColor(50, 100, 50),
    ofColor(50, 50, 100),
    ofColor(100, 100, 50),
    ofColor(100, 50, 100),
    ofColor(50, 100, 100),
    ofColor(200, 100, 50),
    ofColor(100, 200, 50),
    ofColor(100, 50, 200),
    ofColor(50, 100, 200),
    ofColor(50, 200, 100),
    ofColor(0, 0, 0),
    ofColor(20, 50, 100),
    ofColor(50, 20, 100),
    ofColor(100, 50, 20)
};
