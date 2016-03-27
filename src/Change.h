#ifndef CHANGE_H_
#define CHANGE_H_

#include "ofMain.h"
#include "Const.h"


class Change {
    public:
        Change();
        Change(int level, int operation, int value);
        Change(int level, int operation);
        Change* clone();

        int level;

        int application;
        int page;
        int sequencer;
        int step;

        int operation;
        int value;
};

class ChangeSet {
    public:
        ChangeSet();
        ~ChangeSet();
        void push(Change* change);
        void push(ChangeSet* changes);
        Change* next(int level);
        void rewind();

    private:
        int head;
        Change* change[MAX_CHANGES];
};


#endif /* CHANGE_H_ */
