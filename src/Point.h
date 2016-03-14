#ifndef POINT_H_
#define POINT_H_

#define TYPE_NONE 0
#define TYPE_NOTE 1
#define TYPE_DIVISION 2
#define TYPE_ACTIVATE 3

#include "ofMain.h"


class Point {
    public:
        Point();
        void draw(int x, int y, bool selected, ofTrueTypeFont font);
        void click();

        int type;
        int value;
        double offset;
        bool active;
};


#endif /* POINT_H_ */
