#include "NoteStep.h"

const ofColor NoteStep::c_active(0, 0, 50);
const ofColor NoteStep::c_inactive(20, 20, 20);
const ofColor NoteStep::c_sel_active(0, 0, 100);
const ofColor NoteStep::c_sel_inactive(40, 40, 40);

const char* NoteStep::note_name[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

NoteStep::NoteStep() : Step() {
    type = STEP_TYPE_NOTE;
    note = 0;
    octave = 3;
    velocity = 100;
    length = 1000;

    tool_activate = new Tool("TURN\nON", ' ',
        new Change(TARGET_LEVEL_STEP, OP_ACTIVE_SET, 1));
    tool_deactivate = new Tool("TURN\nOFF", ' ',
        new Change(TARGET_LEVEL_STEP, OP_ACTIVE_SET, 0));
    tool_note_0 = new Tool("C", 'q',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 0));
    tool_note_1 = new Tool("C#", '2',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 1));
    tool_note_2 = new Tool("D", 'w',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 2));
    tool_note_3 = new Tool("D#", '3',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 3));
    tool_note_4 = new Tool("E", 'e',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 4));
    tool_note_5 = new Tool("F", 'r',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 5));
    tool_note_6 = new Tool("F#", '5',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 6));
    tool_note_7 = new Tool("G", 't',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 7));
    tool_note_8 = new Tool("G#", '6',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 8));
    tool_note_9 = new Tool("A", 'y',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 9));
    tool_note_10 = new Tool("A#", '7',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 10));
    tool_note_11 = new Tool("B", 'u',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 11));
    tool_note_up = new Tool("+\n1/2", '+',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_DELTA, 1));
    tool_note_down = new Tool("-\n1/2", '-',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_DELTA, -1));
    tool_octave_up = new Tool("+\n8va", 'a',
        new Change(TARGET_LEVEL_STEP, OP_OCTAVE_DELTA, 1));
    tool_octave_down = new Tool("-\n8va", 'z',
        new Change(TARGET_LEVEL_STEP, OP_OCTAVE_DELTA, -1));
    tool_accent = new Tool("VEL\nACC", '.',
        new Change(TARGET_LEVEL_STEP, OP_VELOCITY_SET, 127));
    tool_normal = new Tool("VEL\nNOR", '.',
        new Change(TARGET_LEVEL_STEP, OP_VELOCITY_SET, 100));
}

NoteStep::~NoteStep() {
    delete tool_activate;
    delete tool_deactivate;
    delete tool_note_0;
    delete tool_note_1;
    delete tool_note_2;
    delete tool_note_3;
    delete tool_note_4;
    delete tool_note_5;
    delete tool_note_6;
    delete tool_note_7;
    delete tool_note_8;
    delete tool_note_9;
    delete tool_note_10;
    delete tool_note_11;
    delete tool_note_up;
    delete tool_note_down;
    delete tool_octave_up;
    delete tool_octave_down;
    delete tool_accent;
    delete tool_normal;
}

int NoteStep::getLength() {
    return length;
}

void NoteStep::execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer) {
    double start, stop;
    start = (buffer->isFresh() || sequencer.release == 0.) ? buffer->getLastTime() : sequencer.release;
    stop = start + sequencer.period;
    if (active) {
        MidiEvent* start_event = new MidiEvent(
            start,
            sequencer.output_router,
            sequencer.output,
            octave * 12 + note,
            velocity
        );
        buffer->push(start_event);
        MidiEvent* stop_event = new MidiEvent(
            stop,
            sequencer.output_router,
            sequencer.output,
            octave * 12 + note,
            0
        );
        buffer->push(stop_event);
    }
    changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
    changes->upstream->push(new Change(TARGET_LEVEL_SEQUENCER, OP_RELEASE_SET, stop));
}

void NoteStep::draw(int x, int y, bool executing, ofTrueTypeFont font, ofTrueTypeFont font_big) {
    if (executing) {
        if (active) {
            ofSetColor(c_sel_active);
        } else {
            ofSetColor(c_sel_inactive);
        }
    } else {
        if (active) {
            ofSetColor(c_active);
        } else {
            ofSetColor(c_inactive);
        }
    }
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);

    if (active) {
        ofSetColor(ofColor::white);
        font_big.drawString(ofToString(note_name[note]) + ofToString(octave), x + 3, y + 18);
        ofSetColor(ofColor::lightGray);
        font.drawString(ofToString(velocity), x + 3, y + 38);
    }
}

void NoteStep::populate(Toolbar* toolbar) {
    if (active) {
        toolbar->push(tool_deactivate);
    } else {
        toolbar->push(tool_activate);
    }
    toolbar->push(tool_octave_down);
    toolbar->push(tool_octave_up);
    toolbar->push(tool_note_down);
    toolbar->push(tool_note_up);
    if (velocity == 100) {
        toolbar->push(tool_accent);
    } else {
        toolbar->push(tool_normal);
    }
    toolbar->push(tool_note_0); 
    toolbar->push(tool_note_1); 
    toolbar->push(tool_note_2); 
    toolbar->push(tool_note_3); 
    toolbar->push(tool_note_4); 
    toolbar->push(tool_note_5); 
    toolbar->push(tool_note_6); 
    toolbar->push(tool_note_7); 
    toolbar->push(tool_note_8); 
    toolbar->push(tool_note_9); 
    toolbar->push(tool_note_10); 
    toolbar->push(tool_note_11); 
}

void NoteStep::change(ChangeSet* changes) {
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_STEP)) != NULL) {
        switch (change->operation) {
            case OP_NOTE_SET:
                setNote(change->value);
                break;
            case OP_NOTE_DELTA:
                setNote(note + change->value);
                break;
            case OP_OCTAVE_SET:
                setOctave(change->value);
                break;
            case OP_OCTAVE_DELTA:
                setOctave(octave + change->value);
                break;
            case OP_VELOCITY_SET:
                setVelocity(change->value);
                break;
            case OP_VELOCITY_DELTA:
                setVelocity(velocity + change->value);
                break;
            case OP_ACTIVE_SET:
                setActive(change->value);
                break;
        }
    }
}

void NoteStep::write(ofstream& f) {
    f << "delta-cursor 1\n"
      << "add-note-step\n"
      << "set-active " << (active ? "1" : "0") << "\n"
      << "set-note " << ofToString(note) << "\n"
      << "set-octave " << ofToString(octave) << "\n"
      << "set-velocity " << ofToString(velocity) << "\n";
}

int NoteStep::getNote() { return this->note; }

void NoteStep::setNote(int note) {
    if (note < 0) {
        this->note = 0;
    } else if (note >= 11) {
        this->note = 11;
    } else {
        this->note = note;
    }
    changed = true;
}

int NoteStep::getOctave() { return this->octave; }

void NoteStep::setOctave(int octave) {
    if (octave < 0) {
        this->octave = 0;
    } else if (octave >= 10) {
        this->octave = 10;
    } else {
        this->octave = octave;
    }
    changed = true;
}

int NoteStep::getVelocity() { return this->velocity; }

void NoteStep::setVelocity(int velocity) {
    if (velocity < 0) {
        this->velocity = 0;
    } else if (velocity >= 127) {
        this->velocity = 127;
    } else {
        this->velocity = velocity;
    }
    changed = true;
}
