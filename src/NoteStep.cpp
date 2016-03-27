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

    tool_note_0 = new PersistantTool("C", 'q',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 0));
    tool_note_1 = new PersistantTool("C#", '2',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 1));
    tool_note_2 = new PersistantTool("D", 'w',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 2));
    tool_note_3 = new PersistantTool("D#", '3',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 3));
    tool_note_4 = new PersistantTool("E", 'e',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 4));
    tool_note_5 = new PersistantTool("F", 'r',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 5));
    tool_note_6 = new PersistantTool("F#", '5',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 6));
    tool_note_7 = new PersistantTool("G", 't',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 7));
    tool_note_8 = new PersistantTool("G#", '6',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 8));
    tool_note_9 = new PersistantTool("A", 'y',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 9));
    tool_note_10 = new PersistantTool("A#", '7',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 10));
    tool_note_11 = new PersistantTool("B", 'u',
        new Change(TARGET_LEVEL_STEP, OP_NOTE_SET, 11));
}

int NoteStep::getLength() {
    return length;
}

ChangeSet* NoteStep::execute(TickBuffer* buffer, SequencerState sequencer) {
    ChangeSet* changes = new ChangeSet();
    if (active) {
        MidiEvent* start_event = new MidiEvent(
            buffer->relative_time,
            sequencer.output_router,
            sequencer.output,
            octave * 12 + note,
            velocity
        );
        buffer->push(start_event);
        MidiEvent* stop_event = new MidiEvent(
            buffer->relative_time + sequencer.period,
            sequencer.output_router,
            sequencer.output,
            octave * 12 + note,
            0
        );
        buffer->push(stop_event);
    }
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_POSITION_DELTA, 1));
    changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_RELEASE_DELTA, sequencer.period));
    return changes;
}

void NoteStep::draw(int x, int y, bool executing, ofTrueTypeFont font) {
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
        font.drawString(ofToString(note_name[note]) + ofToString(octave), x + 3, y + 13);
        ofSetColor(ofColor::lightGray);
        font.drawString(ofToString(velocity), x + 3, y + 28);
    }
}

void NoteStep::populate(Toolbar* toolbar) {
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
                note = change->value;
                break;
            case OP_NOTE_DELTA:
                note += change->value;
                note = note > 11 ? 11 : note;
                note = note < 0 ? 0 : note;
                break;
            case OP_OCTAVE_SET:
                octave = change->value;
                break;
            case OP_OCTAVE_DELTA:
                octave += change->value;
                octave = octave > 10 ? 10 : octave;
                octave = octave < 0 ? 0 : octave;
                break;
        }
    }
}
