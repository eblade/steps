#pragma once

#include "ofMain.h"
#include "Sequencer.h"
#include "Ticking.h"


#define MAX_LINES 8


class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		
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
        void addNewLine(int afterLine);
        void deleteLine(int line);
        void cursorUp();
        void cursorDown();

        void step();

        int cursor;
        
        ofTrueTypeFont font;
        Sequencer* sequencer[MAX_LINES];
        TickBuffer* buffer;
        bool playing;
};
