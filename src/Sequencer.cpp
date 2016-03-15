#include "Sequencer.h"


Sequencer::Sequencer() {
    name = "";
    division = 8;
    active = true;
    octave = 3;

    data[0] = new Point();
    data[0]->type = TYPE_ACTIVATE;
    
    for (int i = 1; i < MAX_LENGTH; i++) {
        data[i] = NULL;
    }

    cursor = 1;
}

void Sequencer::draw(int row, bool onThisRow, ofTrueTypeFont font) {
    int x, y;
    for (int col=0; col < MAX_LENGTH; col++) {
        x = 50 * col;
        y = 50 * row;

        if (onThisRow) {
            ofSetColor(ofColor::darkGray);
            ofDrawRectangle(x, y, 50 * MAX_LENGTH, 50);
        }

        if (data[col] != NULL) {
            ofSetColor(ofColor::blue);
            data[col]->draw(x, y, col==cursor, font);
        } else {
            ofSetColor(ofColor::black);
            ofDrawRectangle(x + 1, y + 1 , 48, 48);
        }

        if (onThisRow && col == cursor) {
            ofSetColor(ofColor::white);
            ofDrawRectangle(x, y + 48, 50, 5);
        }

        if (data[col] == NULL) {
            break;
        }
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
        data[cursor]->click();
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
        if (data[cursor]->type == TYPE_NOTE) {
            data[cursor]->value = octave * 12 + value;
        }
    } else {
        data[cursor] = new Point();
        data[cursor]->type = TYPE_NOTE;
        data[cursor]->value = octave * 12 + value;
        cursorRight();
    }
}

void Sequencer::cursorReturn() {
    Point* point = new Point();
    point->type = TYPE_RETURN;
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
