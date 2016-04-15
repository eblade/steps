#include "CommandStep.h"

CommandStep::CommandStep() {
    type = STEP_TYPE_COMMAND;
    command = "";
    text = "?";

    tool_run = new Tool("RUN", ' ',
        new Change(TARGET_LEVEL_STEP, OP_COMMAND_RUN));
}

CommandStep::~CommandStep() {
    delete tool_run;
}

void CommandStep::execute(ChangeSet* changes, TickBuffer* buffer, SequencerState sequencer) {
    changes->upstream->push(new Change(TARGET_LEVEL_APPLICATION, OP_COMMAND, command));
}

void CommandStep::click(ChangeSet* changes) {
    changes->upstream->push(new Change(TARGET_LEVEL_APPLICATION, OP_COMMAND, command));
}

void CommandStep::draw(int x, int y, bool executing, ofTrueTypeFont font, ofTrueTypeFont font_big) {
    ofSetColor(0);
    ofDrawRectangle(x + STEP_SPACING, y + STEP_SPACING , STEP_INNER, STEP_INNER);

    ofSetColor(ofColor::white);
    font.drawString("CMD", x + 3, y + 13);
    font.drawString(text, x + 3, y + 38);
}

void CommandStep::populate(Toolbar* toolbar) {
    toolbar->push(tool_run);
}

void CommandStep::change(ChangeSet* changes) {
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_STEP)) != NULL) {
        switch (change->operation) {
            case OP_COMMAND_SET:
                setCommand(change->string_value);
                break;
            case OP_COMMAND_RUN:
                changes->upstream->push(new Change(TARGET_LEVEL_APPLICATION, OP_COMMAND, command));
                break;
            case OP_TEXT_SET:
                setText(change->string_value);
                break;
        }
    }
}

void CommandStep::write(ofstream& f) {
    f << "delta-cursor 1\n"
      << "add-command-step\n"
      << "set-command " << ofToString(command) << "\n"
      << "set-text " << ofToString(text) << "\n";
}

string CommandStep::getCommand() { return this->command; }

void CommandStep::setCommand(string command) {
    this->command.assign(command);
    changed = true;
}

string CommandStep::getText() { return this->text; }

void CommandStep::setText(string text) {
    this->text.assign(text);
    changed = true;
}
