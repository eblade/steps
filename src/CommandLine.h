#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include "ofMain.h"
#include "boost/lexical_cast.hpp"
#include "Const.h"
#include "Change.h"


class Command {
    public:
        Command(char* line);
        bool is(const char* candidate);
        bool isComment();
        bool hasArgument();
        int argumentAsInt(int default_value = 0);
        bool argumentAsBool(bool default_value = true);

        char command[MAX_COMMAND];
        char argument[MAX_COMMAND];

    private:
        bool is_comment;
};

class CommandLine {
    public:
        CommandLine();
        ~CommandLine();
        void draw(ofTrueTypeFont font);
        ChangeSet* keyPressed(int key);
        ChangeSet* mousePressed(int x, int y, int button);
        int getHeight();
        void clear();
        ChangeSet* run(string line);
        bool hasError();

    private:
        int cursor;
        char line[MAX_COMMAND];
        string error_msg;
        bool error;
        void execute(ChangeSet* changes);
        Command parse();
};

#endif /* COMMANDLINE_H_ */
