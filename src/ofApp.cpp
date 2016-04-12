#include "ofApp.h"

void ofApp::setup() {
    // Logging
    ofSetLogLevel(OF_LOG_VERBOSE);

    // Graphics
    font.load(OF_TTF_MONO, 10, true, true);
    font_big.load(OF_TTF_MONO, 14, true, true);
    ofBackground(ofColor::black);
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetWindowTitle(APPLICATION);
    ofSetEscapeQuitsApp(false);
    ofLogNotice("Main") << "OpenFrameworks setup ok.";
    redraw_all = false;

    load();
    ofLogNotice("Main") << "Done with setup.";

    // Open file from arguments to application
    if (arguments.size() > 1) {
        edit(arguments.at(1));
    }
}

void ofApp::load() {
    // Setup up the timed buffer
    buffer = new TickBuffer(60);
    ofLogNotice("Main") << "TickBuffer setup ok.";

    // Setup up the Output Router
    output_router = new OutputRouter();
    ofLogNotice("Main") << "OutputRouter setup ok.";

    // Setup up the Toolbar
    toolbar = new Toolbar();
    tool_cmd = new Tool("CMD", ':', new Change(TARGET_LEVEL_APPLICATION, OP_COMMAND_MODE));
    tool_play = new Tool("PLAY", 'p', new Change(TARGET_LEVEL_APPLICATION, OP_PLAY_SET, 1));
    tool_stop = new Tool("STOP", 'p', new Change(TARGET_LEVEL_APPLICATION, OP_PLAY_SET, 0));
    tool_stop->changes->push(new Change(TARGET_LEVEL_PAGE, OP_SYNC));
    tool_sync = new Tool("SYNC\nALL", 'S', new Change(TARGET_LEVEL_PAGE, OP_SYNC, 0));
    tool_bpm_80 = new Tool("80\nBPM", 'Q', new Change(TARGET_LEVEL_APPLICATION, OP_BPM_SET, 80));
    tool_bpm_120 = new Tool("120\nBPM", 'W', new Change(TARGET_LEVEL_APPLICATION, OP_BPM_SET, 120));
    tool_bpm_160 = new Tool("160\nBPM", 'E', new Change(TARGET_LEVEL_APPLICATION, OP_BPM_SET, 160));
    tool_add_page = new Tool("+\nPAGE", 'P', new Change(TARGET_LEVEL_APPLICATION, OP_PAGE_ADD));
    ofLogNotice("Main") << "Toolbar setup ok.";

    // Setup the CommandLine
    command_line = new CommandLine();
    command_mode = false;

    // "Globals"
    setFilename("untitled");

    // Clear all pages and reset active page to 0
    for (int i = 0; i < MAX_PAGES; i++) {
        page[i] = NULL;
    }
    active_page = 0;
    setPlaying(false);
    ofLogNotice("Main") << "Page setup ok.";

    // Hack for drawing things again after startup and resize
    init_counter = 10;
}

void ofApp::exit() {
    unload();
}

void ofApp::unload() {
    delete buffer;
    delete output_router;
    delete toolbar;
    delete tool_cmd;
    delete tool_play;
    delete tool_stop;
    delete tool_bpm_80;
    delete tool_bpm_120;
    delete tool_bpm_160;
    delete tool_sync;
    delete tool_add_page;
    for (int i = 0; i < MAX_PAGES; i++) {
        if (page[i] != NULL) {
            delete page[i];
            page[i] = NULL;
        }
    }
}

void ofApp::reset() {
    unload();
    load();
}

void ofApp::update() {

}

void ofApp::draw() {
    if (init_counter > 0) {
        if (init_counter == 1) {
            redraw_all = true;
        }
        init_counter--;
    }

    // Run the logics
    step();
    if (playing) {
        buffer->tick();
    } else {
        buffer->clear();
    }

    // Clear the background
    if (redraw_all) {
        ofBackground(0);
    }

    // Draw the active page
    if (page[active_page] != NULL) {
        page[active_page]->draw(0, 0, ofGetWidth(), ofGetHeight(), font, !command_mode, redraw_all);

    } else if (redraw_all && active_page == 0) {
        ofSetColor(200, 100, 80);
        font_big.drawString("Click +PAGE to create a page.", 15, 30);
    }

    // Draw the toolbar or command line
    if (command_mode) {
        command_line->draw(font_big);
    } else {
        toolbar->draw(font, redraw_all);
    }

    // Draw some debug info to the right
    ofSetColor(0);
    ofDrawRectangle(ofGetWidth() - 95, 0, 95, 140);
    ofSetColor(200);
    font.drawString("pg" + ofToString(active_page) + ", sec" + ofToString(buffer->getActiveSection()),
                    ofGetWidth() - 90, 15);
    font.drawString("bpm " + ofToString(buffer->getBPM()), ofGetWidth() - 90, 30);
    font.drawString("fps " + ofToString((int)ofGetFrameRate()), ofGetWidth() - 90, 45);

    buffer->draw(ofGetWidth() - 90, 55);

    for (int i = 0; i < MAX_OUTPUTS; i++) {
        ofSetColor(OutputColors::color[i % OUTPUT_COLORS]);
        ofDrawRectangle(ofGetWidth() - 90, 138 + 15 * i, 90, 15);
        ofSetColor(200 + output_router->getPeak(i));
        font.drawString(output_router->getOutputString(i), ofGetWidth() - 87, 150 + 15*i);
    }

    redraw_all = false;
}

void ofApp::step() {
    ChangeSet* upstream = buffer->release();
    if (page[active_page] != NULL) {
        if (playing) {
            page[active_page]->step(buffer, output_router);
            page[active_page]->change(upstream, buffer);
        }
    }
    Toolbar* new_toolbar = new Toolbar();
    new_toolbar->push(tool_cmd);
    new_toolbar->push(playing ? tool_stop : tool_play);
    new_toolbar->push(tool_sync);
    new_toolbar->push(tool_bpm_80);
    new_toolbar->push(tool_bpm_120);
    new_toolbar->push(tool_bpm_160);
    new_toolbar->push(tool_add_page);
    if (page[active_page] != NULL) {
        page[active_page]->populate(new_toolbar);
    }
    Toolbar* old_toolbar = this->toolbar;
    new_toolbar->markForRedraw(old_toolbar);
    this->toolbar = new_toolbar;
    delete old_toolbar;
    delete upstream;
}

void ofApp::change(ChangeSet* changes) {
    if (changes == NULL) {
        return;
    }
    changes->rewind();
    Change* change;
    while ((change = changes->next(TARGET_LEVEL_APPLICATION)) != NULL) {
        switch (change->operation) {
            case OP_PLAY_SET: setPlaying(change->value); break;
            case OP_PAGE_ADD: active_page = addPage(); break;
            case OP_PAGE_SET: setActivePage(change->value); break;
            case OP_PAGE_DELTA: setActivePage(active_page + change->value); break;
            case OP_BPM_SET: buffer->setBPM(change->value); break;
            case OP_BPM_DELTA: buffer->setBPM(buffer->getBPM() + change->value); break;
            case OP_COMMAND_MODE: setCommandMode(true); break;
            case OP_NORMAL_MODE: setCommandMode(false); break;
            case OP_FILENAME_SET: setFilename(change->string_value); break;
            case OP_WRITE: write(getFilename()); break;
            case OP_EDIT: edit(getFilename()); break;
            case OP_NEW: reset(); break;
            case OP_EXIT: ofExit(change->value); break;
            case OP_REDRAW_ALL: redraw_all = true; break;
            case OP_OUTPUT_MIDI_INSTALL: {
                OutputSettings output_settings;
                output_settings.type = OUTPUT_TYPE_MIDI;
                output_settings.device = change->value;
                output_settings.channel = 0;
                output_router->install(output_settings);
            }
                break;
            case OP_OUTPUT_MIDI_UNINSTALL: output_router->uninstall(); break;
            case OP_OUTPUT_SELECT_SET: output_router->setOutput(change->value); break;
            case OP_OUTPUT_SELECT_DELTA: output_router->setOutput(output_router->getOutput() + change->value); break;
            case OP_CHANNEL_SET: output_router->setChannel(change->value); break;
            case OP_CHANNEL_DELTA: output_router->setChannel(output_router->getChannel() + change->value); break;
            case OP_SECTION_SET: buffer->setActiveSection(change->value); break;
            case OP_SECTION_DELTA: buffer->setActiveSection(buffer->getActiveSection() + change->value); break;
        }
    }
    if (page[active_page] != NULL) {
        page[active_page]->change(changes, buffer);
    }
    delete changes;
}

int ofApp::addPage() {
    for (int i = 0; i < MAX_PAGES; i++) {
        if (page[i] == NULL) {
            page[i] = new Page();
            redraw_all = true;
            return i;
        }
    }
    return -1;
}

void ofApp::write(string filename) {
    ofstream f(filename);
    if (f.is_open()) {
        f << "# " << APPLICATION << " v." << VERSION << "\n";
        f << "set-bpm " << ofToString(buffer->getBPM()) << "\n";
        output_router->write(f);
        for (int i = 0; i < MAX_PAGES; i++) {
            if (page[i] == NULL) {
                break;
            }
            page[i]->write(f);
        }
        f << "set-page " << ofToString(active_page) << "\n";
        f << "# end of file\n";
        f.close();
        ofLogNotice("Main") << "Wrote to " << filename << ".";
    } else {
        ofLogError("Main") << "Unable to open file \"" << filename << "\"";
    }
}

void ofApp::edit(string filename) {
    command_mode = true;
    ifstream f(filename);
    if (f.is_open()) {
        string command;
        while (getline(f, command)) {
            change(command_line->run(command));
        }
        f.close();
        ofLogNotice("Main") << "Read from " << filename << ".";
    } else {
        ofLogError("Main") << "Unable to open file \"" << filename << "\"";
    }
}

void ofApp::keyPressed(int key) {
    if (command_mode) {
        change(command_line->keyPressed(key));
    } else {
        change(toolbar->keyPressed(key));
    }
}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y){

}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){
    int toolbar_start_y = ofGetHeight() - toolbar->getHeight();
    if (y >= toolbar_start_y) {
        if (!command_mode) {
            change(toolbar->mousePressed(x, y - toolbar_start_y, button));
        }
    } else if (page[active_page] != NULL) {
        page[active_page]->mousePressed(x, y, button, buffer);
    }
}

void ofApp::mouseReleased(int x, int y, int button){

}

void ofApp::mouseEntered(int x, int y){

}

void ofApp::mouseExited(int x, int y){

}

void ofApp::windowResized(int w, int h){
    init_counter = 5;
}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

bool ofApp::getPlaying() { return playing; }

void ofApp::setPlaying(bool playing) {
    buffer->reset();
    this->playing = playing;
}

string ofApp::getFilename() { return filename; }

void ofApp::setFilename(string filename) {
    this->filename.assign(filename);
    ofLogNotice("Main") << "Filename is now \"" << this->filename << "\"";
    ofSetWindowTitle(ofToString(APPLICATION) + " [" + this->filename + "]");
}

int ofApp::getActivePage() { return active_page; }

void ofApp::setActivePage(int active_page) {
    active_page = active_page < MAX_PAGES ? active_page : MAX_PAGES - 1;
    active_page = active_page >= 0 ? active_page : 0;
    if (page[active_page] == NULL) {
        active_page = 0;
    }
    this->active_page = active_page;
}

bool ofApp::getCommandMode() { return command_mode; }

void ofApp::setCommandMode(bool command_mode) {
    if (command_mode != this->command_mode) {
        redraw_all = true;
    }
    if (command_mode && !this->command_mode) {
        command_line->clear();
    }
    this->command_mode = command_mode;
}
