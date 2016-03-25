#ifndef PAGE_H_
#define PAGE_H_

#include "ofMain.h"
#include "Sequencer.h"
#include "Const.h"


class Page {
    public:
        Page();
        ~Page();
        void draw(int x, int y, int width, int height, ofTrueTypeFont font);
		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
        void cursorUp();
        void cursorDown();
        void step(TickBuffer* buffer, OutputRouter* output_router);
        void addNewLine(int afterLine);
        void deleteLine(int line);

    private:
        int cursor;
        Sequencer* sequencer[MAX_SEQUENCERS];
};

#endif /* PAGE_H_ */
