#pragma once

#include "ofMain.h"
#include "Const.h"
#include "Ticking.h"
#include "Output.h"
#include "Page.h"
#include "Toolbar.h"
#include "CommandLine.h"


class ofApp : public ofBaseApp {
	public:
		void setup();
        void load();
        void unload();
        void exit();
        void reset();
		void update();
		void draw();
        void change(ChangeSet* changes);
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void step();
        int addPage();
        void write(string filename);
        void edit(string filename);
        
        bool getPlaying();
        void setPlaying(bool playing);
        string getFilename();
        void setFilename(string filename);
        int getActivePage();
        void setActivePage(int active_page);
        bool getCommandMode();
        void setCommandMode(bool command_mode);

        ofTrueTypeFont font;
        ofTrueTypeFont font_big;
        TickBuffer* buffer;
        OutputRouter* output_router;
        Toolbar* toolbar;
        vector<string> arguments;
    
        Tool* tool_cmd;
        Tool* tool_play;
        Tool* tool_stop;
        Tool* tool_bpm_80;
        Tool* tool_bpm_120;
        Tool* tool_bpm_160;
        Tool* tool_sync;
        Tool* tool_add_page;

    private:
        void performChanges(ChangeSet* changes);

        Page* page[MAX_PAGES];
        CommandLine* command_line;

        bool playing;
        string filename;
        int active_page;
        bool command_mode;
        bool redraw_all;
        int init_counter;
};
