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
        void draw(int x, int y, int width, int height, ofTrueTypeFont font, ofTrueTypeFont font_big, bool draw_cursor=true, bool redraw_all=false);
		void mousePressed(int x, int y, int button, ChangeSet* changes);
        void cursorUp();
        void cursorDown();
        void step(ChangeSet* changes, TickBuffer* buffer, OutputRouter* output_router);
        void addNewLine(int afterLine);
        void deleteLine(int line);
        void populate(Toolbar* toolbar);
        void change(ChangeSet* changes);
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
        Tool* tool_add_section;
        Tool* tool_add_loop;
        Tool* tool_add_command;
        Tool* tool_del_step;

    private:
        void performChanges(ChangeSet* changes);

        int cursor;
        Sequencer* sequencer[MAX_SEQUENCERS];
};

#endif /* PAGE_H_ */
