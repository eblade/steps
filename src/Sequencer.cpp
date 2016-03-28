#include "Sequencer.h"


Sequencer::Sequencer() {
    name = "";
    active = true;
    octave = 3;
    position = 0;
    last_executed = 0;
    release = 0;
    output = 0;
    period = 1000;

    for (int i = 1; i < MAX_STEPS; i++) {
        data[i] = NULL;
    }

    data[0] = new ActivateStep();
    cursor = 1;
    ofLogNotice(APPLICATION) << "Seqeuncer setup ok.";
}

void Sequencer::draw(int row, bool onThisRow, ofTrueTypeFont font) {
    int x, y;

    x = 0;
    y = STEP_OUTER * row;
    for (int col = 0; col < MAX_STEPS; col++) {
        x = STEP_OUTER * col;

        if (data[col] != NULL) {
            data[col]->draw(x, y, col==last_executed, font);
        } else {
            if (active) {
                ofSetColor(ActivateStep::c_on);
            } else {
                ofSetColor(ActivateStep::c_off);
            }
            ofDrawRectangle(x, y, 5, STEP_OUTER);
        }

        if (onThisRow && col == cursor) {
            ofSetColor(ofColor::white);
            ofDrawRectangle(x, y, STEP_OUTER, STEP_SPACING);
            ofDrawRectangle(x, y + STEP_SPACING, STEP_SPACING, STEP_INNER);
            ofDrawRectangle(x + STEP_OUTER - STEP_SPACING, y + STEP_SPACING, STEP_SPACING, STEP_INNER);
            ofDrawRectangle(x, y + STEP_OUTER - STEP_SPACING, STEP_OUTER, STEP_SPACING);
        }

        if (data[col] == NULL) {
            break;
        }
    }
}

void Sequencer::step(TickBuffer* buffer, OutputRouter* output_router) {
    SequencerState state;
    state.output_router = output_router;
    if (!active) {
        return;
    }
    while (true) {
        if (data[position] == NULL) {
            position = 0;
            break;
        }
        if (!buffer->timeFor(release)) {
            break;
        }
        Step* step = data[position];
        last_executed = position;
        state.output = output;
        state.period = period;
        state.release = release;
        change(step->execute(buffer, state), buffer);
        if (last_executed >= position) {
            break;
        }
    }
}

void Sequencer::change(ChangeSet* changes, TickBuffer* buffer) {
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_SEQUENCER)) != NULL) {
        switch (change->operation) {
            case OP_POSITION_SET:
                position = change->value;
                position = position < MAX_STEPS ? position : 0;
                position = position >= 0 ? position : 0;
                if (data[position] == NULL) {
                    position = 0;
                }
                break;
            case OP_POSITION_DELTA:
                position += change->value;
                position = position < MAX_STEPS ? position : 0;
                position = position >= 0 ? position : 0;
                if (data[position] == NULL) {
                    position = 0;
                }
                break;
            case OP_STEP_SET:
                cursor = change->value;
                break;
            case OP_STEP_DELTA:
                if (change->value > 0) {
                    cursorRight();
                } else if (change->value < 0) {
                    cursorLeft();
                }
                break;
            case OP_ACTIVE_SET:
                active = change->value ? true : false;
                break;
            case OP_OUTPUT_SET:
                output = change->value;
                break;
            case OP_OUTPUT_DELTA:
                output += change->value;
                output = output < MAX_OUTPUTS ? output : 0;
                break;
            case OP_PERIOD_SET:
                period = change->value;
                break;
            case OP_PERIOD_DELTA:
                period += change->value;
                period = period > 0 ? period : 0;
                break;
            case OP_RELEASE_SET:
                release = change->long_value;
                break;
            case OP_RELEASE_DELTA:
                release += change->value;
                break;
            //case OP_LAST_START_SET:
            //    last_start = change->long_value;
            //    break;
            //case OP_LAST_START_DELTA:
            //    last_start += change->value;
            //    break;
            case OP_ADD_STEP_NOTE: {
                NoteStep* new_note = new NoteStep();
                new_note->note = change->value;
                cursorInsert(new_note);
                break;
            }
            case OP_ADD_STEP_OUTPUT: {
                OutputStep* new_output = new OutputStep();
                new_output->output = change->value;
                cursorInsert(new_output);
                break;
            }
            case OP_ADD_STEP_DIVISION: {
                DivisionStep* new_division =
                    new DivisionStep(1, change->value, 1);
                cursorInsert(new_division);
                break;
            }
        }
    }
    if (data[cursor] != NULL) {
        data[cursor]->change(changes);
    }
}

void Sequencer::populate(Toolbar* toolbar) {
    if (data[cursor] != NULL) {
        data[cursor]->populate(toolbar);
    }
}

void Sequencer::cursorLeft() {
    if (cursor > 0) {
        cursor--;
    }
}

void Sequencer::cursorRight() {
    if (cursor < (MAX_STEPS - 1)) {
        if (data[cursor] != NULL) {
            cursor++;
        }
    }
}

void Sequencer::cursorClick() {
    if (data[cursor] != NULL) {
        change(data[cursor]->click(), NULL);
    }
}

void Sequencer::cursorDelete() {
    if (data[cursor] != NULL) {
        Step* retired = data[cursor];
        for (int i = cursor; i < (MAX_STEPS - 1); i++) {
            data[i] = data[i + 1];
        }
        data[MAX_STEPS - 1] = NULL;
        delete retired;
    }
}

void Sequencer::cursorInsert(Step* step) {
    if (data[cursor] != NULL) {
        if (data[MAX_STEPS - 1] == NULL) {
            for (int i = MAX_STEPS - 1; i >= cursor; i--) {
                data[i] = data[i - 1];
            }
            data[cursor] = step;
            cursor++;
        } else {
            delete step;
        }
    } else {
        data[cursor] = step;
        cursorRight();
    }
}

int Sequencer::getLength() {
    for (int i = 0; i < MAX_STEPS; i++) {
        if (data[i] == NULL) {
            return i;
        }
    }
    return MAX_STEPS;
}

void Sequencer::setCursor(int wanted) {
    cursor = min(wanted, getLength());
}

void Sequencer::sync() {
    position = 0;
    release = 0;
    last_executed = 0;
}
