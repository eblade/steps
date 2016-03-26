#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include "ofMain.h"
#include "Const.h"
#include "Change.h"


#define TOOLBAR_STATE_IDLE 0
#define TOOLBAR_STATE_TYPING 1


class Tool {
    public:
        Tool();
        void draw(int x, int y, ofTrueTypeFont font);

        ChangeSet changes;
        int key[MAX_KEYS];
};


class Toolbar {
    public:
        Toolbar();
        void draw(ofTrueTypeFont font);
        void keyPressed(int key);

    private:
        string content;
        int state;
};

#endif /* TOOLBAR_H_ */
