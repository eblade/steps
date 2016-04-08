#ifndef CHANGE_H_
#define CHANGE_H_

#include "ofMain.h"
#include "Const.h"


class Change {
    public:
        Change();
        Change(int level, int operation, int value);
        Change(int level, int operation, double float_value);
        Change(int level, int operation, string string_value);
        Change(int level, int operation);
        void init();
        Change* clone();

        int level;

        int application;
        int page;
        int sequencer;
        int step;

        int operation;
        int value;
        double float_value;
        string string_value;
};

class ChangeSet {
    public:
        ChangeSet(bool upstream=false);
        ~ChangeSet();
        void push(Change* change);
        void push(ChangeSet* changes);
        Change* next(int level);
        void rewind();
        ChangeSet* upstream;
        int getLength();

    private:
        int head;
        Change* change[MAX_CHANGES];
};


#endif /* CHANGE_H_ */
