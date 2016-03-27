#include "Output.h"

OutputRouter::OutputRouter() {
    for (int i = 0; i < MAX_OUTPUTS; i++) {
        output[i] = OutputSettings();
    }
    for (int i = 0; i < MAX_OUTPUT_DEVICES; i++) {
        midi_output[i] = NULL;
    }
    ofxMidiOut::listPorts();
};

OutputRouter::~OutputRouter() {
    for (int i = 0; i < MAX_OUTPUT_DEVICES; i++) {
        if (midi_output[i] != NULL) {
            ofLogNotice(APPLICATION) << "Closing midi port " << i 
                 << " \"" << midi_output[i]->getName() << "\"";
            midi_output[i]->closePort();
            delete midi_output[i];
            midi_output[i] = NULL;
        }
    };
};

bool OutputRouter::install(int address, OutputSettings settings) {
    if (address < 0 || address >= MAX_OUTPUTS) {
        ofLogError(APPLICATION) << "ERROR: Bad output address " << address;
        return true;
    }
    if (output[address].used) {
        ofLogError(APPLICATION) << "ERROR: Output address " << address << " in use";
        return true;
    }
    output[address].type = settings.type;
    output[address].device = settings.device;
    output[address].channel = settings.channel;
    output[address].used = true;
    if (settings.type == OUTPUT_TYPE_MIDI) {
        if (midi_output[settings.device] == NULL) {
            midi_output[settings.device] = new ofxMidiOut();
            midi_output[settings.device]->openPort(settings.device);
            ofLogNotice(APPLICATION) << "Opened midi port " << settings.device 
                 << " \"" << midi_output[settings.device]->getName() << "\"";
        }
    }
    return false;
}

void OutputRouter::uninstall(int address) {
    if (address >= 0 && address < MAX_OUTPUTS) {
        output[address].used = false;
    } else {
        ofLogError(APPLICATION) << "WARNING: Bad attempt to unassign output " << address;
    }
}

string OutputRouter::getOutputString(int address) {
    if (!(address >= 0 && address < MAX_OUTPUTS)) {
        return ofToString(address) + ":OUT OF BOUNDS";
    } else if (output[address].used == false) {
        return ofToString(address) + ":-";
    } else {
        if (output[address].type == OUTPUT_TYPE_DUMMY) {
            return ofToString(address) + ":DUMMY " 
                                       + ofToString(output[address].device) + "/" 
                                       + ofToString(output[address].channel);
        } else if (output[address].type == OUTPUT_TYPE_MIDI) {
            return ofToString(address) + ":MIDI "
                                       + ofToString(output[address].device) + "/" 
                                       + ofToString(output[address].channel);
        }
    }
    return "?";
}

void OutputRouter::send(int address, OutputEvent event) {
    if (!(address >= 0 && address < MAX_OUTPUTS) || output[address].used == false) {
        cerr << "ERROR: Output " << address << " is not installed";
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
}

void OutputRouter::sendDummy(OutputSettings settings, OutputEvent event) {
    ofLogNotice(APPLICATION)
        << "SEND DUMMY device=" << settings.device 
        << " channel=" << settings.channel
        << " note=" << event.note
        << " velocity=" << event.velocity;
}

void OutputRouter::sendMidi(OutputSettings settings, OutputEvent event) {
    ofLogNotice(APPLICATION)
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
