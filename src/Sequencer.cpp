#include "Sequencer.h"


Sequencer::Sequencer() {
    name = "";
    division = 8;
    active = true;
    octave = 3;
    position = 0;
    last_executed = 0;
    release = 0;
    ackumulated = 0;

    data[0] = new ActivatePoint();
    
    for (int i = 1; i < MAX_LENGTH; i++) {
        data[i] = NULL;
    }

    cursor = 1;
}

void Sequencer::draw(int row, bool onThisRow, ofTrueTypeFont font) {
    int x, y;

    x = 0;
    y = POINT_OUTER * row;
    for (int col = 0; col < MAX_LENGTH; col++) {
        x = POINT_OUTER * col;

        if (data[col] != NULL) {
            data[col]->draw(x, y, col==last_executed, font);
        } else {
            ofSetColor(ofColor::red);
            ofDrawRectangle(x, y, 5, POINT_OUTER);
        }

        if (onThisRow && col == cursor) {
            ofSetColor(ofColor::white);
            ofDrawRectangle(x, y, POINT_OUTER, POINT_SPACING);
            ofDrawRectangle(x, y + POINT_SPACING, POINT_SPACING, POINT_INNER);
            ofDrawRectangle(x + POINT_OUTER - POINT_SPACING, y + POINT_SPACING, POINT_SPACING, POINT_INNER);
            ofDrawRectangle(x, y + POINT_OUTER - POINT_SPACING, POINT_OUTER, POINT_SPACING);
        }

        if (data[col] == NULL) {
            break;
        }
    }
}

void Sequencer::step(TickBuffer* buffer) {
    ackumulated += buffer->period;
    //cout << "step ======================== " << ackumulated << " release=" << release << " period=" << buffer->period << endl;
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
        Point* point = data[position];
        last_executed = position;
        change(point->execute(ackumulated, buffer));
        release = ackumulated + point->getLength();
        if (last_executed >= position) {
            break;
        }
    }
}

void Sequencer::change(ChangeSet changes) {
    if (changes.goto_position == -1) {
        position += changes.position_delta;
    } else {
        position = changes.goto_position;
    }
    if (changes.set_active) {
        active = true;
    } else if (changes.set_inactive) {
        active = false;
    }
}

void Sequencer::cursorLeft() {
    if (cursor > 0) {
        cursor--;
    }
}

void Sequencer::cursorRight() {
    if (cursor < (MAX_LENGTH - 1)) {
        if (data[cursor] != NULL) {
            cursor++;
        }
    }
}

void Sequencer::cursorClick() {
    if (data[cursor] != NULL) {
        change(data[cursor]->click());
    }
}

void Sequencer::cursorDelete() {
    if (data[cursor] != NULL) {
        Point* retired = data[cursor];
        for (int i = cursor; i < (MAX_LENGTH - 1); i++) {
            data[i] = data[i + 1];
        }
        data[MAX_LENGTH - 1] = NULL;
        delete retired;
    }
}

bool Sequencer::cursorInsert(Point* point) {
    if (data[cursor] != NULL) {
        if (data[MAX_LENGTH - 1] == NULL) {
            for (int i = MAX_LENGTH - 1; i >= cursor; i--) {
                data[i] = data[i - 1];
            }
            data[cursor] = point;
            cursor++;
            return true;
        } else {
            return false;
        }
    } else {
        data[cursor] = point;
        cursorRight();
        return true;
    }
}

void Sequencer::cursorNote(int value) {
    if (data[cursor] != NULL) {
        if (data[cursor]->type == POINT_TYPE_NOTE) {
            ((NotePoint*) data[cursor])->value = octave * 12 + value;
        }
    } else {
        data[cursor] = new NotePoint();
        ((NotePoint*) data[cursor])->value = octave * 12 + value;
        cursorRight();
    }
}

void Sequencer::cursorHold() {
    Point* point = new Point();
    // FIXME
    if (!cursorInsert(point)) {
        delete point;
    }
}

int Sequencer::getLength() {
    for (int i = 0; i < MAX_LENGTH; i++) {
        if (data[i] == NULL) {
            return i;
        }
    }
    return MAX_LENGTH;
}

void Sequencer::setCursor(int wanted) {
    cursor = min(wanted, getLength());
}
