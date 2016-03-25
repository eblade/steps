#include "Page.h"

Page::Page() {
    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        sequencer[i] = NULL;
    }

    addNewLine(-1);
    cursor = 0;
    //playing = true;

    ofLogNotice(APPLICATION) << "Page setup ok.";
}

Page::~Page() {
    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        if (sequencer[i] != NULL) {
            delete sequencer[i];
            sequencer[i] = NULL;
        }
    }
}

void Page::step(TickBuffer* buffer, OutputRouter* output_router) {
    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        if (sequencer[i] == NULL) {
            break;
        }
        sequencer[i]->step(buffer, output_router);
    }
}

void Page::draw(int x, int y, int width, int height, ofTrueTypeFont font) {
    ofSetColor(0);
    ofDrawRectangle(x, y, width, height);

    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        if (sequencer[i] == NULL) {
            break;
        }
        sequencer[i]->draw(i, i==cursor, font);
    }
}

void Page::keyPressed(int key) {
    if (key == 'h' || key == OF_KEY_LEFT) {
        sequencer[cursor]->cursorLeft();
    } else if (key == 'l' || key == OF_KEY_RIGHT) {
        sequencer[cursor]->cursorRight();
    } else if (key == 'k' || key == OF_KEY_UP) {
        cursorUp();
    } else if (key == 'j' || key == OF_KEY_DOWN) {
        cursorDown();
    } else if (key == ' ') {
        sequencer[cursor]->cursorClick();
    //} else if (key == 'P') {
    //    playing = !playing;
    } else if (key == 'x' || key == OF_KEY_DEL) {
        if (sequencer[cursor]->cursor == 0) {
            deleteLine(cursor);
        } else {
            sequencer[cursor]->cursorDelete();
        }
    } else if (key == 'q') {
        sequencer[cursor]->cursorNote(0);
    } else if (key == '2') {
        sequencer[cursor]->cursorNote(1);
    } else if (key == 'w') {
        sequencer[cursor]->cursorNote(2);
    } else if (key == '3') {
        sequencer[cursor]->cursorNote(3);
    } else if (key == 'e') {
        sequencer[cursor]->cursorNote(4);
    } else if (key == 'r') {
        sequencer[cursor]->cursorNote(5);
    } else if (key == '5') {
        sequencer[cursor]->cursorNote(6);
    } else if (key == 't') {
        sequencer[cursor]->cursorNote(7);
    } else if (key == '6') {
        sequencer[cursor]->cursorNote(8);
    } else if (key == 'y') {
        sequencer[cursor]->cursorNote(9);
    } else if (key == '7') {
        sequencer[cursor]->cursorNote(10);
    } else if (key == 'u') {
        sequencer[cursor]->cursorNote(11);
    } else if (key == '8') {
        sequencer[cursor]->cursorNote(12);
    } else if (key == 'i') {
        sequencer[cursor]->cursorNote(13);
    } else if (key == '9') {
        sequencer[cursor]->cursorNote(14);
    } else if (key == 'o') {
        sequencer[cursor]->cursorNote(15);
    } else if (key == 'p') {
        sequencer[cursor]->cursorNote(16);
    } else if (key == OF_KEY_RETURN) {
        addNewLine(cursor);
    } else if (key == 'g') {
        sequencer[cursor]->cursorHold();
    } else if (key == 'O') {
        sequencer[cursor]->cursorOutput();
    } else if (key == 'n') {
        sequencer[cursor]->cursorDivision(8);
    }
}

void Page::mousePressed(int x, int y, int button){
    int col = x / 50;
    int row = y / 50;

    if (row >= MAX_SEQUENCERS) {
        return;
    }
    if (sequencer[row] == NULL) {
        return;
    }
    sequencer[row]->setCursor(col);
    if (sequencer[row]->cursor == col) {
        sequencer[row]->cursorClick();
    }
}

void Page::cursorUp() {
    if (cursor > 0) {
        cursor--;
        sequencer[cursor]->setCursor(sequencer[cursor + 1]->cursor);
    }
}

void Page::cursorDown() {
    if (cursor < (MAX_SEQUENCERS - 1)) {
        if (sequencer[cursor + 1] != NULL) {
            cursor++;
            sequencer[cursor]->setCursor(sequencer[cursor - 1]->cursor);
        }
    }
}

void Page::deleteLine(int line) {
    if (sequencer[line] != NULL) {
        if (line == 0 && sequencer[1] == NULL) {
            return; // can't delete the last line
        }
        Sequencer* retired = sequencer[cursor];
        for (int i = line; i < (MAX_SEQUENCERS - 1); i++) {
            sequencer[i] = sequencer[i + 1];
        }
        if (line == cursor) {
            sequencer[line]->setCursor(retired->cursor);
        }
        sequencer[MAX_SEQUENCERS - 1] = NULL;
        delete retired;
    }
}

void Page::addNewLine(int afterLine) {
    if (afterLine >= MAX_SEQUENCERS) {
        return;
    }
    if (sequencer[afterLine + 1] == NULL) {
        sequencer[afterLine + 1] = new Sequencer();
        if (cursor == afterLine) {
            cursor++;
        }
    } else if (sequencer[MAX_SEQUENCERS-1] == NULL && afterLine > 0) {
        for (int i = MAX_SEQUENCERS - 1; i > afterLine; i--) {
            sequencer[i] = sequencer[i - 1];
        }
        sequencer[afterLine + 1] = new Sequencer();
        if (cursor >= afterLine) {
            cursor++;
        }
    }
}
