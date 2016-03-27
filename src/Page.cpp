#include "Page.h"

Page::Page() {
    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        sequencer[i] = NULL;
    }

    addNewLine(-1);
    cursor = 0;

    tool_seq_add = new PersistantTool("+\nSEQ", OF_KEY_RETURN,
        new Change(TARGET_LEVEL_PAGE, OP_SEQ_ADD, 0));
    tool_seq_add->key_string = "RET";

    tool_step_prev = new PersistantTool("<<\nSTEP", 'h',
        new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_DELTA, -1));
    tool_step_prev->addKey(OF_KEY_LEFT);

    tool_seq_next = new PersistantTool("NEXT\nSEQ", 'j',
        new Change(TARGET_LEVEL_PAGE, OP_SEQ_DELTA, 1));
    tool_seq_next->addKey(OF_KEY_DOWN);

    tool_seq_prev = new PersistantTool("PREV\nSEQ", 'k',
        new Change(TARGET_LEVEL_PAGE, OP_SEQ_DELTA, -1));
    tool_seq_prev->addKey(OF_KEY_UP);

    tool_step_next = new PersistantTool(">>\nSTEP", 'l',
        new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_DELTA, 1));
    tool_step_next->addKey(OF_KEY_RIGHT);

    tool_add_note = new PersistantTool("+\nNOTE", 'n',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_NOTE, 0));

    tool_add_div = new PersistantTool("+\nDIV", 'd',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_DIVISION, 8));

    tool_add_output = new PersistantTool("+\nOUT", 'O',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_OUTPUT, 0));
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

void Page::populate(Toolbar* toolbar) {
    toolbar->push(tool_seq_add);
    toolbar->push(tool_step_prev);
    toolbar->push(tool_seq_prev);
    toolbar->push(tool_seq_next);
    toolbar->push(tool_step_next);
    toolbar->push(tool_add_note);
    toolbar->push(tool_add_div);
    toolbar->push(tool_add_output);
    if (sequencer[cursor] != NULL) {
        sequencer[cursor]->populate(toolbar);
    }
}

void Page::change(ChangeSet* changes, TickBuffer* buffer) {
    if (changes == NULL) {
        return;
    }
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_PAGE)) != NULL) {
        switch (change->operation) {
            case OP_SEQ_ADD:
                addNewLine(cursor);
                break;
            case OP_SEQ_DELTA:
                if (change->value > 0) {
                    cursorDown();
                } else if (change->value < 0) {
                    cursorUp();
                }
                break;
        }
    }
    if (sequencer[cursor] != NULL) {
        sequencer[cursor]->change(changes, buffer);
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
