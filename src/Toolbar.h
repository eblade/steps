#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include "ofMain.h"
#include "Const.h"
#include "Change.h"
#include "Page.h"


class Tool {
    public:
        Tool();
        Tool(string label, int key, Change* change);
        virtual ~Tool();
        virtual void draw(int x, int y, ofTrueTypeFont font);

        ChangeSet* changes;
        bool persistant;

    protected:
        virtual void init(string label, string key_string);
        int head;
        string key_string;
        string label;
        int key[MAX_KEYS];
        static const ofColor
            c_back,
            c_text,
            c_key_back,
            c_key_text;
};


class PersistantTool : public Tool {
    public:
        PersistantTool();
        PersistantTool(string label, int key, Change* change);
};


class Toolbar {
    public:
        Toolbar();
        ~Toolbar();
        void draw(ofTrueTypeFont font);
        void keyPressed(int key);
        void mousePressed(int x, int y, int button);
        void push(Tool* tool);
        void update(Page* page);
        int getHeight();

    private:
        Tool* tool[MAX_TOOLS];
        int head;
};

#endif /* TOOLBAR_H_ */
