#ifndef PAGE_H_
#define PAGE_H_

#include "ofMain.h"
#include "Sequencer.h"
#include "Const.h"
#include "Toolbar.h"
#include "Change.h"


class Page {
    public:
        Page();
        ~Page();
        void draw(int x, int y, int width, int height, ofTrueTypeFont font, bool draw_cursor=true, bool redraw_all=false);
		void mousePressed(int x, int y, int button);
        void cursorUp();
        void cursorDown();
        void step(TickBuffer* buffer, OutputRouter* output_router);
        void addNewLine(int afterLine);
        void deleteLine(int line);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes, TickBuffer* buffer);
        void write(ofstream& f);

        Tool* tool_seq_add;
        Tool* tool_seq_next;
        Tool* tool_seq_prev;
        Tool* tool_step_home;
        Tool* tool_step_prev;
        Tool* tool_step_next;
        Tool* tool_step_end;
        Tool* tool_add_note;
        Tool* tool_add_div;
        Tool* tool_add_output;
        Tool* tool_add_sync;
        Tool* tool_del_step;

    private:
        int cursor;
        Sequencer* sequencer[MAX_SEQUENCERS];
};

#endif /* PAGE_H_ */
