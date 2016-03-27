#include "Change.h"

Change::Change() {
    level = TARGET_LEVEL_APPLICATION;
    application = THE_ACTIVE_ONE;
    page = NONE;
    sequencer = NONE;
    step = NONE;
    operation = OP_NONE;
    value = 0;
}

Change::Change(int level, int operation, int value) {
    Change();
    this->level = level;
    this->operation = operation;
    this->value = value;
}

Change::Change(int level, int operation) {
    Change();
    this->level = level;
    this->operation = operation;
}


ChangeSet::ChangeSet() {
    head = 0;
    for (int i = 0; i < MAX_CHANGES; i++) {
        change[i] = NULL;
    }
}

ChangeSet::~ChangeSet() {
    for (int i = 0; i < MAX_CHANGES; i++) {
        if (change[i] != NULL) {
            delete change[i];
            change[i] = NULL;
        }
    }
}

void ChangeSet::push(Change* change) {
    if (head >= MAX_CHANGES) {
        ofLogError(APPLICATION) << "Max changes exceeded!";
    }
    this->change[head] = change;
    head++;
}

Change* ChangeSet::next(int level) {
    for (int i = head; i < MAX_CHANGES; i++) {
        if (change[i] == NULL) {
            break;
        } else if (change[i]->level == level) {
            head = i + 1;
            return change[i];
        }
    }
    return NULL;
}

void ChangeSet::rewind() {
    head = 0;
}
