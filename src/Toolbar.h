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
        virtual void draw(int x, int y, ofTrueTypeFont font, bool redraw);
        virtual bool hasKey(int key);
        virtual void addKey(int key);
        virtual void flash();
        virtual void markForRedraw();

        ChangeSet* changes;
        string key_string;
        string label;

    protected:
        virtual bool needsFlashing();
        virtual bool needsRedraw();
        virtual void init(string label, string key_string);
        int head;
        int key[MAX_KEYS];
        int level;
        bool changed;
        int flashing;
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
        void draw(ofTrueTypeFont font, bool redraw_all=false);
        ChangeSet* keyPressed(int key);
        ChangeSet* mousePressed(int x, int y, int button);
        void push(Tool* tool);
        int getHeight();
        void markForRedraw(Toolbar *other_toolbar);
        Tool* getTool(int position);

    private:
        Tool* tool[MAX_TOOLS];
        int head;
        bool blank;
};

#endif /* TOOLBAR_H_ */
