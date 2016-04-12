#include "Change.h"

Change::Change() {
    this->init();
}

Change::Change(int level, int operation, int value) {
    this->init();
    this->level = level;
    this->operation = operation;
    this->value = value;
}

Change::Change(int level, int operation, double float_value) {
    this->init();
    this->level = level;
    this->operation = operation;
    this->float_value = float_value;
}

Change::Change(int level, int operation, string string_value) {
    this->init();
    this->level = level;
    this->operation = operation;
    this->string_value = string_value;
}

Change::Change(int level, int operation) {
    this->init();
    this->level = level;
    this->operation = operation;
}

void Change::init() {
    level = TARGET_LEVEL_APPLICATION;
    application = THE_ACTIVE_ONE;
    page = NONE;
    sequencer = NONE;
    step = NONE;
    operation = OP_NONE;
    value = 0;
    float_value = 0.;
    string_value = "";
}

Change* Change::clone() {
    Change* copy = new Change();
    copy->level = this->level;
    copy->application = this->application;
    copy->page = this->page;
    copy->step = this->step;
    copy->operation = this->operation;
    copy->value = this->value;
    copy->float_value = this->float_value;
    copy->string_value.assign(this->string_value);
    return copy;
}

//==========================================================================


ChangeSet::ChangeSet(bool upstream) {
    head = 0;
    for (int i = 0; i < MAX_CHANGES; i++) {
        change[i] = NULL;
    }
    if (!upstream) {
        this->upstream = new ChangeSet(true);
    } else {
        this->upstream = NULL;
    }
}

ChangeSet::~ChangeSet() {
    for (int i = 0; i < MAX_CHANGES; i++) {
        if (change[i] != NULL) {
            delete change[i];
            change[i] = NULL;
        }
    }
    if (upstream != NULL) {
        delete upstream;
    }
}

void ChangeSet::push(Change* change) {
    if (head >= MAX_CHANGES) {
        ofLogError(APPLICATION) << "Max changes exceeded!";
    }
    this->change[head] = change;
    head++;
}

void ChangeSet::push(ChangeSet* changes) {
    if (changes == NULL) {
        return;
    }
    for (int i = 0; i < MAX_CHANGES; i++) {
        if (changes->change[i] != NULL) {
            this->push(changes->change[i]->clone());
        } else {
            return;
        }
    }
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

int ChangeSet::getLength() {
    int i;
    for (i = head; i < MAX_CHANGES; i++) {
        if (change[i] == NULL) {
            return i;
        }
    }
    return i;
}
