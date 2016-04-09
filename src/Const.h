#ifndef CONST_H_
#define CONST_H_

// Application name and version
#define APPLICATION "Steps"
#define VERSION "0"

// Max number of Pages buffered
#define MAX_PAGES 8

// Max number of Sequencers/lines on a Page
#define MAX_SEQUENCERS 8

// Max number of Steps in a Sequencer
#define MAX_STEPS 20

// Max number of Outputs defined by an OutputRouter
#define MAX_OUTPUTS 16

// Max number of Devices to be used by the OutputRouter
#define MAX_OUTPUT_DEVICES 16

// Max number of Keys mapped to a Tool
#define MAX_KEYS 8

// Max number of Changes in a ChangeSet
#define MAX_CHANGES 32

// Max number of Tools to put in a Toolbar
#define MAX_TOOLS 64

// Max number of Labels to use on a Page
#define MAX_LABELS 16

// Max TickBuffer size
#define TICK_BUFFER_SIZE 256

// Max CommandLine length
#define MAX_COMMAND 128

// Max beats per minute (BPM)
#define MAX_BPM 300

// Step Types
#define STEP_TYPE_NONE 0
#define STEP_TYPE_NOTE 1
#define STEP_TYPE_DIVISION 2
#define STEP_TYPE_ACTIVATE 3
#define STEP_TYPE_OUTPUT 4
#define STEP_TYPE_SYNC 5

// Output Types
#define OUTPUT_TYPE_DUMMY 0
#define OUTPUT_TYPE_MIDI 1

// Step dimensions
#define STEP_INNER 46
#define STEP_OUTER 50
#define STEP_SPACING 2
#define STEP_KEY_HEIGHT 20

// Change target levels
#define TARGET_LEVEL_APPLICATION 0
#define TARGET_LEVEL_PAGE 1
#define TARGET_LEVEL_SEQUENCER 2
#define TARGET_LEVEL_STEP 3

// Special targets (not levels)
#define NONE -1
#define THE_ACTIVE_ONE -2

// Change operations
// Application level
#define OP_EXIT -1 
#define OP_NONE 0
#define OP_PLAY_SET 1 // play|stop
#define OP_PAGE_SET 2 // set-page
#define OP_PAGE_DELTA 3
#define OP_BPM_SET 4 // set-bpm
#define OP_BPM_DELTA 5
#define OP_SYNC 6
#define OP_COMMAND_MODE 7
#define OP_NORMAL_MODE 8
#define OP_FILENAME_SET 9
#define OP_WRITE 10 // write
#define OP_PAGE_ADD 11 // add-page
#define OP_PAGE_DEL 12 // del-page
#define OP_EDIT 13 // edit
#define OP_NEW 14 // new
#define OP_REDRAW_ALL 15 // redraw

// Page level
#define OP_SEQ_ADD 100 // add-sequencer
#define OP_SEQ_DEL 101 // del-sequencer
#define OP_SEQ_SET 102 // set-sequencer
#define OP_SEQ_DELTA 103

// Sequencer level
//#define OP_STEP_ADD 200 // Require STEP_TYPE
#define OP_STEP_DEL 201
#define OP_STEP_SET 202
#define OP_STEP_DELTA 203
#define OP_ACTIVE_SET 204 // set-active BOOL
#define OP_OUTPUT_SET 206 // set-output INT
#define OP_OUTPUT_DELTA 207
#define OP_PERIOD_SET 208
#define OP_PERIOD_DELTA 209
#define OP_RELEASE_SET 210 // LONG
#define OP_RELEASE_DELTA 211
#define OP_POSITION_SET 212
#define OP_POSITION_DELTA 213
#define OP_LAST_START_SET 214 // LONG
#define OP_LAST_START_DELTA 215

#define OP_ADD_STEP_ACTIVATE 250 // add-activate-step
#define OP_ADD_STEP_NOTE 251 // add-note-step
#define OP_ADD_STEP_OUTPUT 252 // add-output-step
#define OP_ADD_STEP_DIVISION 253 // add-division-step
#define OP_ADD_STEP_SYNC 254 // add-sync-step

// Step level
#define OP_NOTE_SET 300 // set-note INT
#define OP_NOTE_DELTA 301 
#define OP_OCTAVE_SET 302 // set-octave INT
#define OP_OCTAVE_DELTA 303
#define OP_VELOCITY_SET 304 // set-velocity INT
#define OP_VELOCITY_DELTA 305
#define OP_LABEL_SET 306 // set-label INT
#define OP_LABEL_DELTA 307
#define OP_HOLD_SET 308 // set-hold BOOL

#define OP_NUMERATOR_SET 350 // set-numerator INT
#define OP_NUMERATOR_DELTA 351 // delta-numerator INT
#define OP_DENOMINATOR_SET 352 // set-denominator INT
#define OP_DENOMINATOR_DELTA 353 // delta-denominator INT
#define OP_TUPLET_SET 354 // set-tuplet INT
#define OP_TUPLET_DELTA 355 // delta-tuplet

#define OP_KILL 360

#endif
