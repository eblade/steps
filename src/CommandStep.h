#ifndef COMMANDSTEP_H_
#define COMMANDSTEP_H_

#include "ofMain.h"
#include "Step.h"
#include "Change.h"
#include "Toolbar.h"

class CommandStep : public Step {
    public:
        CommandStep();
        ~CommandStep();
        void draw(int x, int y, bool executing, ofTrueTypeFont font, ofTrueTypeFont font_big);
        void execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer);
        void click(ChangeSet* changes);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
        void write(ofstream& f);

        string getCommand();
        void setCommand(string command);
        string getText();
        void setText(string text);


        Tool* tool_run;

    private:
        string command;
        string text;
};

#endif
