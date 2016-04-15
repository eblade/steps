#include "Sequencer.h"


Sequencer::Sequencer() {
    octave = 3;
    position = 0;
    last_executed = 0;
    release = 0.;
    output = 0;
    period = 1.;
    label = 0;
    cursor_shade = 255;
    redraw = true;
    synced = false;
    at_end = false;

    for (int i = 1; i < MAX_STEPS; i++) {
        data[i] = NULL;
    }

    data[0] = new ActivateStep();
    cursor = 1;
    cursor_blank = -1;
}

Sequencer::~Sequencer() {
    for (int i = 1; i < MAX_STEPS; i++) {
        if (data[i] != NULL) {
            delete data[i];
        }
    }
}

void Sequencer::draw(int row, bool onThisRow, ofTrueTypeFont font, ofTrueTypeFont font_big, bool redraw_all) {
    redraw_all = redraw || redraw_all;

    int x, y;
    x = 0;
    y = STEP_OUTER * row;

    if (redraw_all) {
        ofSetColor(0);
        ofDrawRectangle(x, y, ofGetWidth(), STEP_OUTER);
    }

    for (int col = 0; col < MAX_STEPS; col++) {
        x = STEP_OUTER * col;

        if (data[col] != NULL) {
            if (redraw_all || data[col]->needsRedraw()) {
                data[col]->draw(x, y, col==last_executed, font, font_big);
            }
        }

        if (col == cursor_blank) {
            ofSetColor(0);
            ofDrawRectangle(x, y, STEP_OUTER, STEP_SPACING);
            ofDrawRectangle(x, y + STEP_SPACING, STEP_SPACING, STEP_INNER);
            ofDrawRectangle(x + STEP_OUTER - STEP_SPACING, y + STEP_SPACING, STEP_SPACING, STEP_INNER);
            ofDrawRectangle(x, y + STEP_OUTER - STEP_SPACING, STEP_OUTER, STEP_SPACING);
            cursor_blank = -1;
        }

        if (onThisRow && col == cursor) {
            ofSetColor(cursor_shade);
            ofDrawRectangle(x, y, STEP_OUTER, STEP_SPACING);
            ofDrawRectangle(x, y + STEP_SPACING, STEP_SPACING, STEP_INNER);
            ofDrawRectangle(x + STEP_OUTER - STEP_SPACING, y + STEP_SPACING, STEP_SPACING, STEP_INNER);
            ofDrawRectangle(x, y + STEP_OUTER - STEP_SPACING, STEP_OUTER, STEP_SPACING);
        }

        if (data[col] == NULL) {
            break;
        }
    }

    if (at_end && redraw_all && last_executed == 0) {
        ofSetColor(200, 50, 50);
        font.drawString("DONE.", x + 3, y + 45);
    }

    cursor_shade -= 3;
    if (cursor_shade < 100) {
        cursor_shade = 255;
    }

    redraw = false;
}

void Sequencer::step(ChangeSet* changes, TickBuffer* buffer, OutputRouter* output_router) {
    SequencerState state;
    state.output_router = output_router;
    while (true) {
        if (!buffer->timeFor(release)) {
            break;
        }
        if (data[last_executed] != NULL) {
            data[last_executed]->markForRedraw();
        }
        if (at_end) {
            release = 0.;
            last_executed = 0;
            redraw = true;
            break;
        }
        if (data[position] == NULL) {
            ofLogWarning("Sequencer") << "position at broken data.";
            release = 0.;
            position = 0;
            break;
        }
        Step* step = data[position];
        last_executed = position;
        state.output = output;
        state.period = period;
        state.release = release;
        state.label = label;
        state.synced = synced;
        delete changes->upstream;
        changes->upstream = new ChangeSet(false);
        step->execute(changes, buffer, state);
        performChanges(changes->upstream);
        if (last_executed >= position) {
            break;
        }
        synced = false;
    }
}

void Sequencer::change(ChangeSet* changes) {
    performChanges(changes);
    if (data[cursor] != NULL) {
        data[cursor]->change(changes);
    }
    performChanges(changes->upstream);
}

void Sequencer::performChanges(ChangeSet* changes) {
    if (changes == NULL) {
        return;
    }
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_SEQUENCER)) != NULL) {
        switch (change->operation) {
            case OP_POSITION_SET: setPosition(change->value); break;
            case OP_POSITION_DELTA: setPosition(position + change->value); break;
            case OP_STEP_SET:
                if (change->value == 0) {
                    cursorHome();
                } else if (change->value == -1) {
                    cursorEnd();
                } else {
                    setCursor(change->value);
                }
                break;
            case OP_STEP_DELTA:
                if (change->value > 0) {
                    cursorRight();
                } else if (change->value < 0) {
                    cursorLeft();
                }
                break;
            case OP_OUTPUT_SET:
                output = change->value;
                break;
            case OP_OUTPUT_DELTA:
                output += change->value;
                output = output < MAX_OUTPUTS ? output : 0;
                break;
            case OP_PERIOD_SET:
                period = change->float_value;
                break;
            case OP_PERIOD_DELTA:
                period += change->float_value;
                period = period > 0 ? period : 0;
                break;
            case OP_RELEASE_SET:
                release = change->float_value;
                break;
            case OP_RELEASE_DELTA:
                release += change->float_value;
                break;
            case OP_LABEL_SET:
                label = change->value;
                break;
            case OP_SYNC:
                this->sync();
                break;
            case OP_ADD_STEP_NOTE: {
                NoteStep* new_note = new NoteStep();
                new_note->setNote(change->value);
                cursorInsert(new_note);
                break;
            }
            case OP_ADD_STEP_OUTPUT: {
                OutputStep* new_output = new OutputStep();
                new_output->setOutput(change->value);
                cursorInsert(new_output);
                break;
            }
            case OP_ADD_STEP_DIVISION: {
                DivisionStep* new_division =
                    new DivisionStep(1, change->value, 1);
                cursorInsert(new_division);
                break;
            }
            case OP_ADD_STEP_SYNC: {
                SyncStep* new_sync =
                    new SyncStep(change->value);
                cursorInsert(new_sync);
                break;
            }
            case OP_ADD_STEP_SECTION: {
                SectionStep* new_section =
                    new SectionStep(change->value);
                cursorInsert(new_section);
                break;
            }
            case OP_ADD_STEP_LOOP: {
                LoopStep* new_loop =
                    new LoopStep(change->value);
                cursorInsert(new_loop);
                break;
            }
            case OP_ADD_STEP_COMMAND: {
                CommandStep* new_command =
                    new CommandStep();
                cursorInsert(new_command);
                break;
            }
        }
    }
}

void Sequencer::populate(Toolbar* toolbar) {
    if (data[cursor] != NULL) {
        data[cursor]->populate(toolbar);
    }
}

void Sequencer::write(ofstream& f) {
    f << "add-sequencer\n";
    f << "set-cursor 0\n";
    for (int i = 0; i < MAX_STEPS; i++) {
        if (data[i] == NULL) {
            break;
        }
        data[i]->write(f);
    }
}

void Sequencer::cursorLeft() {
    if (cursor > 0) {
        cursor_blank = cursor;
        cursor--;
    }
}

void Sequencer::cursorRight() {
    if (cursor < (MAX_STEPS - 1)) {
        if (data[cursor] != NULL) {
            cursor_blank = cursor;
            cursor++;
        }
    }
}

void Sequencer::cursorClick(ChangeSet* changes) {
    if (data[cursor] != NULL) {
        data[cursor]->click(changes);
        performChanges(changes->upstream);
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
        redraw = true;
    }
}

void Sequencer::cursorInsert(Step* step) {
    if (cursor == 0 && data[0] != NULL) {
        cursor = 1;
    }
    if (data[cursor] != NULL) {
        if (data[MAX_STEPS - 1] == NULL) {
            for (int i = MAX_STEPS - 1; i >= cursor; i--) {
                data[i] = data[i - 1];
            }
            data[cursor] = step;
            redraw = true;
        } else {
            delete step;
        }
    } else {
        data[cursor] = step;
    }
}

void Sequencer::cursorBlank() {
    cursor_blank = cursor;
}

void Sequencer::cursorHome() {
    cursor_blank = cursor;
    cursor = 0;
}

void Sequencer::cursorEnd() {
    cursor_blank = cursor;
    for (int i = 0; i < MAX_STEPS; i++) {
        if (data[i] == NULL) {
            cursor = i;
            return;
        }
    }
    cursor = MAX_STEPS - 1;
}

int Sequencer::getLength() {
    for (int i = 0; i < MAX_STEPS; i++) {
        if (data[i] == NULL) {
            return i;
        }
    }
    return MAX_STEPS;
}

int Sequencer::getCursor() { return cursor; }

void Sequencer::setCursor(int wanted) {
    cursor = min(wanted, getLength());
}

void Sequencer::sync() {
    position = 0;
    release = 0.;
    if (data[last_executed] != NULL) {
        data[last_executed]->markForRedraw();
    }
    last_executed = 0;
    synced = true;
    at_end = false;
    redraw = true;
}

void Sequencer::setPosition(int position) {
    position = position <= MAX_STEPS ? position : 0;
    position = position >= 0 ? position : 0;
    at_end = false;
    if (data[position] == NULL) {
        position = 0;
        at_end = true;
        redraw = true;
    }
    if (data[position] != NULL) {
        data[position]->markForRedraw();
    }
    if (data[this->position] != NULL) {
        data[this->position]->markForRedraw();
    }
    this->position = position;
}

int Sequencer::getLabel() { return label; }

int Sequencer::peekType(int position) {
    if (position < 0 || position >= MAX_STEPS) {
        return STEP_TYPE_NONE;
    } else if (data[position] == NULL) {
        return STEP_TYPE_NONE;
    } else {
        return data[position]->getType();
    }
}
