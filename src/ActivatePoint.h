#ifndef ACTIVATEPOINT_H_
#define ACTIVATEPOINT_H_

#include "Point.h"

class ActivatePoint : public Point {
    public:
        ActivatePoint();
        void draw(int x, int y, bool executing, ofTrueTypeFont font);
        ChangeSet click();
        ChangeSet execute(long long time, TickBuffer* buffer);

    private:
        static const ofColor
            c_on,
            c_off;
};


#endif /* ACTIVATEPOINT_H_ */
