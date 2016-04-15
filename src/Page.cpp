#include "Page.h"

Page::Page() {
    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        sequencer[i] = NULL;
    }

    cursor = 0;

    tool_seq_add = new Tool("+\nSEQ", OF_KEY_RETURN,
        new Change(TARGET_LEVEL_PAGE, OP_SEQ_ADD, 0));
    tool_seq_add->key_string = "RET";

    tool_step_home = new Tool("<<\nSTEP", '^',
        new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_SET, 0));
    tool_step_home->addKey(OF_KEY_HOME);

    tool_step_prev = new Tool("<\nSTEP", 'h',
        new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_DELTA, -1));
    tool_step_prev->addKey(OF_KEY_LEFT);

    tool_seq_next = new Tool("NEXT\nSEQ", 'j',
        new Change(TARGET_LEVEL_PAGE, OP_SEQ_DELTA, 1));
    tool_seq_next->addKey(OF_KEY_DOWN);

    tool_seq_prev = new Tool("PREV\nSEQ", 'k',
        new Change(TARGET_LEVEL_PAGE, OP_SEQ_DELTA, -1));
    tool_seq_prev->addKey(OF_KEY_UP);

    tool_step_next = new Tool(">\nSTEP", 'l',
        new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_DELTA, 1));
    tool_step_next->addKey(OF_KEY_RIGHT);

    tool_step_end = new Tool(">>\nSTEP", '$',
        new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_SET, -1));
    tool_step_end->addKey(OF_KEY_END);

    tool_add_note = new Tool("+\nNOTE", 'n',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_NOTE, 0));

    tool_add_div = new Tool("+\nDIV", 'd',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_DIVISION, 8));

    tool_add_output = new Tool("+\nOUT", 'O',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_OUTPUT, 0));

    tool_add_sync = new Tool("+\nSYNC", 's',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_SYNC, 0));

    tool_add_section = new Tool("+\nSECT", 'S',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_SECTION, 0));

    tool_add_loop = new Tool("+\nLOOP", 'L',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_LOOP, 4));

    tool_add_command = new Tool("+\nCMD", 'C',
        new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_COMMAND));

    tool_del_step = new Tool("DEL", 'x',
        new Change(TARGET_LEVEL_PAGE, OP_STEP_DEL, -1));
    tool_del_step->addKey(OF_KEY_DEL);
}

Page::~Page() {
    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        if (sequencer[i] != NULL) {
            delete sequencer[i];
            sequencer[i] = NULL;
        }
    }
    delete tool_seq_add;
    delete tool_seq_next;
    delete tool_seq_prev;
    delete tool_step_home;
    delete tool_step_prev;
    delete tool_step_next;
    delete tool_step_end;
    delete tool_add_note;
    delete tool_add_div;
    delete tool_add_output;
    delete tool_add_sync;
    delete tool_add_section;
    delete tool_add_loop;
    delete tool_add_command;
    delete tool_del_step;
}

void Page::step(ChangeSet* changes, TickBuffer* buffer, OutputRouter* output_router) {
    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        if (sequencer[i] == NULL) {
            break;
        }
        sequencer[i]->step(changes, buffer, output_router);
        performChanges(changes->upstream);
    }
}

void Page::draw(int x, int y, int width, int height, ofTrueTypeFont font, ofTrueTypeFont font_big, bool draw_cursor, bool redraw_all) {
    if (redraw_all) {
        ofSetColor(0);
        ofDrawRectangle(x, y, width, height);
    }

    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        if (sequencer[i] == NULL) {
            break;
        }
        sequencer[i]->draw(i, draw_cursor && i==cursor, font, font_big, redraw_all);
    }
}

void Page::mousePressed(int x, int y, int button, ChangeSet* changes){
    int col = x / STEP_OUTER;
    int row = y / STEP_OUTER;

    if (row >= MAX_SEQUENCERS) {
        return;
    }
    if (sequencer[row] == NULL) {
        return;
    }
    if (sequencer[cursor] != NULL) {
        sequencer[cursor]->cursorBlank();
    }
    cursor = row;
    sequencer[row]->setCursor(col);
    if (sequencer[row]->getCursor() == col) {
        sequencer[row]->cursorClick(changes);
        performChanges(changes->upstream);
    }
}

void Page::populate(Toolbar* toolbar) {
    toolbar->push(tool_seq_add);
    toolbar->push(tool_step_home);
    toolbar->push(tool_step_prev);
    toolbar->push(tool_seq_prev);
    toolbar->push(tool_seq_next);
    toolbar->push(tool_step_next);
    toolbar->push(tool_step_end);
    toolbar->push(tool_add_note);
    toolbar->push(tool_add_div);
    toolbar->push(tool_add_output);
    toolbar->push(tool_add_sync);
    toolbar->push(tool_add_section);
    toolbar->push(tool_add_loop);
    toolbar->push(tool_add_command);
    toolbar->push(tool_del_step);
    if (sequencer[cursor] != NULL) {
        sequencer[cursor]->populate(toolbar);
    }
}

void Page::change(ChangeSet* changes) {
    performChanges(changes);
    if (sequencer[cursor] != NULL) {
        sequencer[cursor]->change(changes);
    }
    performChanges(changes->upstream);
}

void Page::performChanges(ChangeSet* changes) {
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
            case OP_SYNC:
                for (int i = 0; i < MAX_SEQUENCERS; i++) {
                    if (sequencer[i] != NULL) {
                        int label = sequencer[i]->getLabel();
                        if (label != 0) {
                            if (change->value == 0 || label == change->value) {
                                sequencer[i]->sync();
                            }
                        }
                    } else {
                        break;
                    }
                }
                break;
            case OP_STEP_DEL: 
                if (sequencer[cursor] != NULL) {
                    if (sequencer[cursor]->getCursor() == 0) {
                        deleteLine(cursor);
                    } else {
                        sequencer[cursor]->cursorDelete();
                    }
                }
                break;
        }
    }
}

void Page::write(ofstream& f) {
    f << "# Page\nadd-page\n";
    for (int i = 0; i < MAX_SEQUENCERS; i++) {
        if (sequencer[i] == NULL) {
            break;
        }
        sequencer[i]->write(f);
    }
}

void Page::cursorUp() {
    if (cursor > 0) {
        if (sequencer[cursor] != NULL) {
            sequencer[cursor]->cursorBlank();
        }
        cursor--;
        sequencer[cursor]->setCursor(sequencer[cursor + 1]->getCursor());
    }
}

void Page::cursorDown() {
    if (cursor < (MAX_SEQUENCERS - 1)) {
        if (sequencer[cursor + 1] != NULL) {
            if (sequencer[cursor] != NULL) {
                sequencer[cursor]->cursorBlank();
            }
            cursor++;
            sequencer[cursor]->setCursor(sequencer[cursor - 1]->getCursor());
        }
    }
}

void Page::deleteLine(int line) {
    if (sequencer[line] != NULL) {
        if (line == 0 && sequencer[1] == NULL) {
            return; // can't delete the last line
        }
        Sequencer* retired = sequencer[line];
        for (int i = line; i < (MAX_SEQUENCERS - 1); i++) {
            sequencer[i] = sequencer[i + 1];
        }
        if (line == cursor && sequencer[line] != NULL) {
            sequencer[line]->setCursor(retired->getCursor());
        }
        sequencer[MAX_SEQUENCERS - 1] = NULL;
        if (sequencer[cursor] == NULL) {
            cursor--;
        }
        delete retired;
    }
}

void Page::addNewLine(int afterLine) {
    if (afterLine >= MAX_SEQUENCERS) {
        return;
    }

    // This is the first line to be added
    if (cursor == 0 && sequencer[0] == NULL) {
        sequencer[0] = new Sequencer();

    // Cursor is on the last existing line
    } else if (sequencer[afterLine + 1] == NULL) {
        sequencer[afterLine + 1] = new Sequencer();
        if (cursor == afterLine) {
            if (sequencer[cursor] != NULL) {
                sequencer[cursor]->cursorBlank();
            }
            cursor++;
        }
    
    // Cursor is before other lines
    } else if (sequencer[MAX_SEQUENCERS-1] == NULL && afterLine > 0) {
        for (int i = MAX_SEQUENCERS - 1; i > afterLine; i--) {
            sequencer[i] = sequencer[i - 1];
        }
        sequencer[afterLine + 1] = new Sequencer();
        if (cursor >= afterLine) {
            if (sequencer[cursor] != NULL) {
                sequencer[cursor]->cursorBlank();
            }
            cursor++;
        }
    }
}
