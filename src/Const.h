#ifndef CONST_H_
#define CONST_H_

// Application name
#define APPLICATION "Steps"

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
#define OP_NONE 0
#define OP_PLAY_SET 1
#define OP_PAGE_SET 2
#define OP_PAGE_DELTA 3
#define OP_BPM_SET 4
#define OP_BPM_DELTA 5
#define OP_SYNC 6

// Page level
#define OP_SEQ_ADD 100
#define OP_SEQ_DEL 101
#define OP_SEQ_SET 102
#define OP_SEQ_DELTA 103

// Sequencer level
//#define OP_STEP_ADD 200 // Require STEP_TYPE
#define OP_STEP_DEL 201
#define OP_STEP_SET 202
#define OP_STEP_DELTA 203
#define OP_ACTIVE_SET 204
#define OP_OUTPUT_SET 206
#define OP_OUTPUT_DELTA 207
#define OP_PERIOD_SET 208
#define OP_PERIOD_DELTA 209
#define OP_RELEASE_SET 210 // LONG
#define OP_RELEASE_DELTA 211
#define OP_POSITION_SET 212
#define OP_POSITION_DELTA 213
#define OP_LAST_START_SET 214 // LONG
#define OP_LAST_START_DELTA 215

#define OP_ADD_STEP_NOTE 250
#define OP_ADD_STEP_OUTPUT 251
#define OP_ADD_STEP_DIVISION 252
#define OP_ADD_STEP_SYNC 253

// Step level
#define OP_NOTE_SET 300
#define OP_NOTE_DELTA 301
#define OP_OCTAVE_SET 302
#define OP_OCTAVE_DELTA 303
#define OP_VELOCITY_SET 304
#define OP_VELOCITY_DELTA 305
#define OP_LABEL_SET 306
#define OP_LABEL_DELTA 307
#define OP_HOLD_SET 308

#define OP_NUMERATOR_SET 350
#define OP_DENOMINATOR_SET 352
#define OP_TUPLET_SET 354
#define OP_TUPLET_DELTA 355


#endif
