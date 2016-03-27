#pragma once

#include "ofMain.h"
#include "Const.h"
#include "Ticking.h"
#include "Output.h"
#include "Page.h"
#include "Toolbar.h"


class ofApp : public ofBaseApp {
	public:
		void setup();
        void exit();
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
        
        ofTrueTypeFont font;
        TickBuffer* buffer;
        OutputRouter* output_router;
        bool playing;
        Page* page[MAX_PAGES];
        int active_page;
        Toolbar* toolbar;
    
        Tool* tool_play;
        Tool* tool_stop;
        Tool* tool_bpm_80;
        Tool* tool_bpm_120;
        Tool* tool_bpm_160;
};
