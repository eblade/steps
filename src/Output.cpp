#include "Output.h"

OutputRouter::OutputRouter() {
    for (int i = 0; i < OUTPUT_MAX; i++) {
        output[i] = OutputSettings();
    }
    for (int i = 0; i < OUTPUT_MAX_DEVICES; i++) {
        midi_output[i] = NULL;
    }
    ofxMidiOut::listPorts();
};

OutputRouter::~OutputRouter() {
    for (int i = 0; i < OUTPUT_MAX_DEVICES; i++) {
        if (midi_output[i] != NULL) {
            cout << "Closing midi port " << i 
                 << " \"" << midi_output[i]->getName() << "\"" << endl;
            midi_output[i]->closePort();
            delete midi_output[i];
            midi_output[i] = NULL;
        }
    };
};

bool OutputRouter::install(int address, OutputSettings settings) {
    if (address < 0 || address >= OUTPUT_MAX) {
        cerr << "ERROR: Bad output address " << address << endl;
        return true;
    }
    if (output[address].used) {
        cerr << "ERROR: Output address " << address << " in use"<< endl;
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
            cout << "Opened midi port " << settings.device 
                 << " \"" << midi_output[settings.device]->getName() << "\"" << endl;
        }
    }
    return false;
}

void OutputRouter::uninstall(int address) {
    if (address >= 0 && address < OUTPUT_MAX) {
        output[address].used = false;
    } else {
        cerr << "WARNING: Bad attempt to unassign output " << address << endl;
    }
}

string OutputRouter::getOutputString(int address) {
    if (!(address >= 0 && address < OUTPUT_MAX)) {
        return ofToString(address) + ":OUT OF BOUNDS";
    } else if (output[address].used == false) {
        return ofToString(address) + ":-";
    } else {
        if (output[address].type == OUTPUT_TYPE_DUMMY) {
            return ofToString(address) + ":DUMMY " 
                                       + ofToString(output[address].device) + "/" 
                                       + ofToString(output[address].channel);
        } else if (output[address].type == OUTPUT_TYPE_DUMMY) {
            return ofToString(address) + ":MIDI "
                                       + ofToString(output[address].device) + "/" 
                                       + ofToString(output[address].channel);
        }
    }
    return "?";
}

void OutputRouter::send(int address, OutputEvent event) {
    if (!(address >= 0 && address < OUTPUT_MAX) || output[address].used == false) {
        cerr << "ERROR: Output " << address << " is not installed" << endl;
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
    cout << "SEND DUMMY device=" << settings.device 
         << " channel=" << settings.channel
         << " note=" << event.note
         << " velocity=" << event.velocity
         << endl;
}

void OutputRouter::sendMidi(OutputSettings settings, OutputEvent event) {
    cout << "SEND MIDI device=" << settings.device 
         << " channel=" << settings.channel
         << " note=" << event.note
         << " velocity=" << event.velocity
         << endl;
    if (midi_output[settings.device] == NULL) {
        cerr << "ERROR: Device " << settings.device << " is not installed";
        return;
    }
    midi_output[settings.device]->sendNoteOn(settings.channel, event.note, event.velocity);
}
