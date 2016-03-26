#ifndef CHANGE_H_
#define CHANGE_H_


#define TARGET_LEVEL_APPLICATION 0
#define TARGET_LEVEL_PAGE 1
#define TARGET_LEVEL_SEQUENCER 2
#define TARGET_LEVEL_STEP 3


class Change {
    public:
        Change();
        int level;
        int application;
        int page;
        int sequencer;
        int step;

};


#endif /* CHANGE_H_ */
