#include "CommandLine.h"


CommandLine::CommandLine() {
    clear();
    error = false;
    error_msg = "";
}

CommandLine::~CommandLine() {

}

void CommandLine::draw(ofTrueTypeFont font) {
    if (error) {
        ofSetColor(100, 50, 20);
    } else {
        ofSetColor(20);
    }
    ofDrawRectangle(
        0, ofGetHeight() - STEP_OUTER,
        ofGetWidth(), STEP_OUTER
    );

    int x, y;
    y = ofGetHeight() - STEP_OUTER + 30;
    if (error) {
        ofSetColor(200);
        font.drawString(error_msg, 5, y);
    } else {
        x = 10;
        ofSetColor(100, 50, 50);
        font.drawString(":", 5, y);
        ofSetColor(200);
        for (int i = 0; i < MAX_COMMAND; i++) {
            x += 15;
            if (line[i] != 0) {
                font.drawString(ofToString(line[i]), x, y);
            } else {
                break;
            }
        }
    }
}

ChangeSet* CommandLine::keyPressed(int key) {
    ChangeSet* changes = new ChangeSet();
    if (error) {
        error = false;
    }
    //ofLogNotice("CommandLine") << "Key pressed: " << ofToString((char)key);
    if (cursor < MAX_COMMAND) {
        if (
               (key >= 'a' && key <= 'z')
            || (key >= 'A' && key <= 'Z')
            || (key >= '0' && key <= '9')
            || key == ' '
            || key == '.'
            || key == ','
            || key == '='
            || key == '-'
            || key == '*'
            || key == '#'
            || key == '&'
        ) {
            line[cursor] = (char)key;
            cursor++;
        } else if (key == OF_KEY_BACKSPACE) {
            cursor--;
            cursor = cursor >= 0 ? cursor : 0;
            line[cursor] = 0;
        } else if (key == OF_KEY_ESC) {
            ofLogNotice("CommandLine") << "Sending OP_NORMAL_MODE [ESC]";
            changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_NORMAL_MODE));
        } else if (key == OF_KEY_RETURN) {
            execute(changes);
        }
    }
    return changes;
}

ChangeSet* CommandLine::run(string line) {
    for (int i = 0; i < MAX_COMMAND; i++) {
        this->line[i] = line[i];
    }
    this->line[MAX_COMMAND - 1] = 0;
    ChangeSet* changes = new ChangeSet();
    execute(changes);
    return changes;
}

void CommandLine::execute(ChangeSet* changes) {
    ofLogNotice("CommandLine") << "Parsing: " << ofToString(line);

    Command command = parse();
    ofLogNotice("CommandLine") << "command=\"" << ofToString(command.command)
                               << "\" argument=\"" << ofToString(command.argument)
                               << "\"";

    // comment
    if (command.isComment()) {
        // pass

    // quit
    } else if (command.is("q") || command.is("quit")) {
        ofLogNotice("CommandLine") << "Sending OP_EXIT";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_EXIT, 0));

    // redraw
    } else if (command.is("r") || command.is("redraw")) {
        ofLogNotice("CommandLine") << "Sending OP_REDRAW_ALL";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_REDRAW_ALL));

    // new
    } else if (command.is("n") || command.is("new")) {
        ofLogNotice("CommandLine") << "Sending OP_NEW.";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_NEW));
        if (command.hasArgument()) {
            ofLogNotice("CommandLine")
                << "Sending OP_FILENAME_SET("
                << ofToString(command.argument) << ").";
            changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_FILENAME_SET,
                                     ofToString(command.argument)));
        }

    // write
    } else if (command.is("w") || command.is("write")) {
        if (command.hasArgument()) {
            ofLogNotice("CommandLine")
                << "Sending OP_FILENAME_SET("
                << ofToString(command.argument) << ").";
            changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_FILENAME_SET,
                                     ofToString(command.argument)));
        }
        ofLogNotice("CommandLine") << "Sending OP_WRITE.";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_WRITE));

    // edit
    } else if (command.is("e") || command.is("edit")) {
        ofLogNotice("CommandLine") << "Sending OP_NEW.";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_NEW));
        if (command.hasArgument()) {
            ofLogNotice("CommandLine")
                << "Sending OP_FILENAME_SET("
                << ofToString(command.argument) << ").";
            changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_FILENAME_SET,
                                     ofToString(command.argument)));
        }
        ofLogNotice("CommandLine") << "Sending OP_EDIT.";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_EDIT));

    // add-page
    } else if (command.is("add-page")) {
        ofLogNotice("CommandLine") << "Sending OP_PAGE_ADD";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_PAGE_ADD));

    // midi-install INT
    } else if (command.is("midi-install")) {
        ofLogNotice("CommandLine") << "Sending OP_OUTPUT_MIDI_INSTALL";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_OUTPUT_MIDI_INSTALL,
                                 command.argumentAsInt(0)));

    // midi-uninstall INT
    } else if (command.is("midi-uninstall")) {
        ofLogNotice("CommandLine") << "Sending OP_OUTPUT_MIDI_UNINSTALL";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_OUTPUT_MIDI_UNINSTALL,
                                 command.argumentAsInt(0)));

    // set-output-select INT
    } else if (command.is("set-output-select")) {
        ofLogNotice("CommandLine") << "Sending OP_OUTPUT_SELECT_SET";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_OUTPUT_SELECT_SET,
                                 command.argumentAsInt(0)));

    // delta-output-select INT
    } else if (command.is("delta-output-select")) {
        ofLogNotice("CommandLine") << "Sending OP_OUTPUT_SELECT_DELTA";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_OUTPUT_SELECT_DELTA,
                                 command.argumentAsInt(0)));

    // set-channel INT
    } else if (command.is("set-channel")) {
        ofLogNotice("CommandLine") << "Sending OP_CHANNEL_SET";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_CHANNEL_SET,
                                 command.argumentAsInt(0)));

    // delta-channel INT
    } else if (command.is("delta-channel")) {
        ofLogNotice("CommandLine") << "Sending OP_CHANNEL_DELTA";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_CHANNEL_DELTA,
                                 command.argumentAsInt(0)));

    // set-bpm INT
    } else if (command.is("set-bpm")) {
        ofLogNotice("CommandLine") << "Sending OP_BPM_SET";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_BPM_SET,
                                 command.argumentAsInt(0)));

    // delta-bpm INT
    } else if (command.is("delta-bpm")) {
        ofLogNotice("CommandLine") << "Sending OP_BPM_DELTA";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_BPM_DELTA,
                                 command.argumentAsInt(0)));

    // set-page INT
    } else if (command.is("set-page")) {
        ofLogNotice("CommandLine") << "Sending OP_PAGE_SET";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_PAGE_SET,
                                 command.argumentAsInt()));

    // delta-page INT
    } else if (command.is("delta-page")) {
        ofLogNotice("CommandLine") << "Sending OP_PAGE_DELTA";
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_PAGE_DELTA,
                                 command.argumentAsInt(1)));

    // add-sequencer
    } else if (command.is("add-sequencer")) {
        ofLogNotice("CommandLine") << "Sending OP_SEQ_ADD";
        changes->push(new Change(TARGET_LEVEL_PAGE, OP_SEQ_ADD));

    // add-note-step
    } else if (command.is("add-note-step")) {
        ofLogNotice("CommandLine") << "Sending OP_ADD_STEP_NOTE";
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_NOTE));

    // add-division-step
    } else if (command.is("add-division-step")) {
        ofLogNotice("CommandLine") << "Sending OP_ADD_STEP_DIVISION";
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_DIVISION));

    // add-sync-step
    } else if (command.is("add-sync-step")) {
        ofLogNotice("CommandLine") << "Sending OP_ADD_STEP_SYNC";
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_SYNC));

    // add-output-step
    } else if (command.is("add-output-step")) {
        ofLogNotice("CommandLine") << "Sending OP_ADD_STEP_OUTPUT";
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_ADD_STEP_OUTPUT));

    // set-cursor INT
    } else if (command.is("set-cursor")) {
        ofLogNotice("CommandLine") << "Sending OP_STEP_SET";
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_SET,
                                 command.argumentAsInt(0)));
    // delta-cursor INT
    } else if (command.is("delta-cursor")) {
        ofLogNotice("CommandLine") << "Sending OP_STEP_DELTA";
        changes->push(new Change(TARGET_LEVEL_SEQUENCER, OP_STEP_DELTA,
                                 command.argumentAsInt(1)));

    // set-active BOOL
    } else if (command.is("set-active")) {
        ofLogNotice("CommandLine") << "Sending OP_ACTIVE_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_ACTIVE_SET,
                                 command.argumentAsBool()));

    // set-label INT
    } else if (command.is("set-label")) {
        ofLogNotice("CommandLine") << "Sending OP_LABEL_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_LABEL_SET,
                                 command.argumentAsInt()));

    // delta-label INT
    } else if (command.is("delta-label")) {
        ofLogNotice("CommandLine") << "Sending OP_LABEL_DELTA";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_LABEL_DELTA,
                                 command.argumentAsInt()));

    // set-note INT
    } else if (command.is("set-note")) {
        ofLogNotice("CommandLine") << "Sending OP_NOTE_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_NOTE_SET,
                                 command.argumentAsInt()));

    // delta-note INT
    } else if (command.is("delta-note")) {
        ofLogNotice("CommandLine") << "Sending OP_NOTE_DELTA";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_NOTE_DELTA,
                                 command.argumentAsInt()));

    // set-octave INT
    } else if (command.is("set-octave")) {
        ofLogNotice("CommandLine") << "Sending OP_OCTAVE_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_OCTAVE_SET,
                                 command.argumentAsInt()));

    // delta-octave INT
    } else if (command.is("delta-octave")) {
        ofLogNotice("CommandLine") << "Sending OP_OCTAVE_DELTA";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_OCTAVE_DELTA,
                                 command.argumentAsInt()));

    // set-velocity INT
    } else if (command.is("set-velocity")) {
        ofLogNotice("CommandLine") << "Sending OP_VELOCITY_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_VELOCITY_SET,
                                 command.argumentAsInt()));

    // delta-velocity INT
    } else if (command.is("delta-velocity")) {
        ofLogNotice("CommandLine") << "Sending OP_VELOCITY_DELTA";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_VELOCITY_DELTA,
                                 command.argumentAsInt()));

    // set-hold BOOL
    } else if (command.is("set-hold")) {
        ofLogNotice("CommandLine") << "Sending OP_HOLD_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_HOLD_SET,
                                 command.argumentAsBool()));

    // set-output INT
    } else if (command.is("set-output")) {
        ofLogNotice("CommandLine") << "Sending OP_OUTPUT_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_OUTPUT_SET,
                                 command.argumentAsInt()));

    // delta-output INT
    } else if (command.is("delta-output")) {
        ofLogNotice("CommandLine") << "Sending OP_OUTPUT_DELTA";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_OUTPUT_DELTA,
                                 command.argumentAsInt()));

    // set-numerator INT
    } else if (command.is("set-numerator")) {
        ofLogNotice("CommandLine") << "Sending OP_NUMERATOR_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_NUMERATOR_SET,
                                 command.argumentAsInt()));

    // delta-numerator INT
    } else if (command.is("delta-numerator")) {
        ofLogNotice("CommandLine") << "Sending OP_NUMERATOR_DELTA";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_NUMERATOR_DELTA,
                                 command.argumentAsInt()));

    // set-denominator INT
    } else if (command.is("set-denominator")) {
        ofLogNotice("CommandLine") << "Sending OP_DENOMINATOR_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_DENOMINATOR_SET,
                                 command.argumentAsInt()));

    // delta-denominator INT
    } else if (command.is("delta-denominator")) {
        ofLogNotice("CommandLine") << "Sending OP_DENOMINATOR_DELTA";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_DENOMINATOR_DELTA,
                                 command.argumentAsInt()));

    // set-tuplet INT
    } else if (command.is("set-tuplet")) {
        ofLogNotice("CommandLine") << "Sending OP_TUPLET_SET";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_TUPLET_SET,
                                 command.argumentAsInt()));

    // delta-tuplet INT
    } else if (command.is("delta-tuplet")) {
        ofLogNotice("CommandLine") << "Sending OP_TUPLET_DELTA";
        changes->push(new Change(TARGET_LEVEL_STEP, OP_TUPLET_DELTA,
                                 command.argumentAsInt()));

    // Invalid/unrecognized
    } else {
        ofLogError("CommandLine")
            << "Invalid command \""
            << ofToString(command.command) << "\".";
        error_msg = "Invalid command \"" + ofToString(command.command) + "\"";
        error = true;
    }

    clear();

    if (error) {
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_COMMAND_MODE));
    } else {
        changes->push(new Change(TARGET_LEVEL_APPLICATION, OP_NORMAL_MODE));
    }
}

ChangeSet* CommandLine::mousePressed(int x, int y, int button) {
    ChangeSet* changes = new ChangeSet();
    return changes;
}

int CommandLine::getHeight() {
    return STEP_OUTER;
}

Command CommandLine::parse() {
    return Command(line);
}

void CommandLine::clear() {
    cursor = 0;
    for (int i = 0; i < MAX_COMMAND; i++) {
        line[i] = 0;
    }
}

bool CommandLine::hasError() {
    return error;
}

Command::Command(char* line) {
    command[0] = 0;
    argument[0] = 0;

    if (line[0] == '#') {
        is_comment = true;

    } else {
        is_comment = false;

        int i;
        for (i = 0; i < MAX_COMMAND; i++) {
            if (line[i] == ' ') {
                command[i] = 0;
                i++;
                break;
            }
            command[i] = line[i];
            if (line[i] == 0) {
                break;
            }
        }
        command[MAX_COMMAND - 1] = 0;

        int a = 0;
        for (; i < MAX_COMMAND; i++) {
            argument[a++] = line[i];
            if (line[i] == 0) {
                break;
            }
        }
        argument[MAX_COMMAND - 1] = 0;
    }
}

bool Command::isComment() {
    return is_comment;
}

bool Command::is(const char* candidate) {
    return (strncmp(command, candidate, MAX_COMMAND) == 0);
}

bool Command::hasArgument() {
    return argument[0] != 0;
}

int Command::argumentAsInt(int default_value) {
    if (!hasArgument()) {
        return default_value;
    }
    try {
        return boost::lexical_cast<int>(argument);
    } catch (const boost::bad_lexical_cast&) {
        ofLogError("Command") << "Argument not integer: " << argument;
        return default_value;
    }
}

bool Command::argumentAsBool(bool default_value) {
    return argumentAsInt(default_value ? 1 : 0);
}
