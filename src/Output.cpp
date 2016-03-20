#include "Output.h"

Output::Output() {
    for (int i = 0; i < OUTPUT_MAX; i++) {
        output[i] = OutputSettings();
    }
};

bool Output::install(int address, OutputSettings settings) {
    if (address >= 0 && address < OUTPUT_MAX && !output[address].used) {
        output[address].type = settings.type;
        output[address].device = settings.device;
        output[address].channel = settings.channel;
        output[address].used = true;
        return false;
    } else {
        cerr << "WARNING: Bad attempt to assign output " << address << endl;
        return true;
    }
}

void Output::uninstall(int address) {
    if (address >= 0 && address < OUTPUT_MAX) {
        output[address].used = false;
    } else {
        cerr << "WARNING: Bad attempt to unassign output " << address << endl;
    }
}

string Output::getOutputString(int address) {
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

void Output::send(int address, OutputEvent event) {
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

void Output::sendDummy(OutputSettings settings, OutputEvent event) {
    cout << "SEND DUMMY device=" << settings.device 
         << " channel=" << settings.channel
         << " note=" << event.note
         << " velocity=" << event.velocity
         << endl;
}

void Output::sendMidi(OutputSettings settings, OutputEvent event) {
    cout << "SEND DUMMY device=" << settings.device 
         << " channel=" << settings.channel
         << " note=" << event.note
         << " velocity=" << event.velocity
         << endl;
}
