#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include "ofMain.h"
#include "Const.h"
#include "Change.h"


class Tool {
    public:
        Tool();
        Tool(string label, int key, Change* change);
        virtual ~Tool();
        virtual void draw(int x, int y, ofTrueTypeFont font);
        virtual bool hasKey(int key);
        virtual void addKey(int key);
        virtual void flash(int peak = 200);

        ChangeSet* changes;
        string key_string;
        string label;
        int peak;

    protected:
        virtual void init(string label, string key_string);
        int head;
        int key[MAX_KEYS];
        int level;
        static const ofColor
            c_back,
            c_text,
            c_key_back,
            c_key_text,
            c_back_page,
            c_text_page,
            c_back_seq,
            c_text_seq,
            c_back_step,
            c_text_step;
};


class Toolbar {
    public:
        Toolbar();
        ~Toolbar();
        void draw(ofTrueTypeFont font);
        ChangeSet* keyPressed(int key);
        ChangeSet* mousePressed(int x, int y, int button);
        void push(Tool* tool);
        int getHeight();

    private:
        Tool* tool[MAX_TOOLS];
        int head;
};

#endif /* TOOLBAR_H_ */
