#include "Point.h"


Point::Point() {
    type = TYPE_NOTE;
    value = 0;
    offset = 0;
    active = true;
}

void Point::draw(int x, int y, bool selected, ofTrueTypeFont font) {
    if (type == TYPE_NOTE) {
        if (active) {
            ofSetColor(ofColor::darkGray);
            ofDrawRectangle(x + 2, y + 2 , 46, 46);
            ofSetColor(ofColor::blue);
        } else {
            ofSetColor(ofColor::lightGray);
        }
        font.drawString(ofToString(value), x + 3, y + 13);
    } else if (type == TYPE_ACTIVATE) {
        if (active) {
            ofSetColor(ofColor::green);
            ofDrawRectangle(x + 2, y + 2 , 46, 46);
            ofSetColor(ofColor::white);
            font.drawString("ON", x + 3, y + 13);
        } else {
            ofSetColor(ofColor::gray);
            ofDrawRectangle(x + 2, y + 2 , 46, 46);
            ofSetColor(ofColor::red);
            font.drawString("OFF", x + 3, y + 13);
        }
    } else if (type == TYPE_RETURN) {
        if (active) {
            ofSetColor(ofColor::green);
            font.drawString("RET", x + 3, y + 40);
        } else {
            ofSetColor(ofColor::lightGray);
            font.drawString("RET", x + 3, y + 40);
        }
    }
}

void Point::click() {
    if (type == TYPE_NOTE || type == TYPE_ACTIVATE || type == TYPE_RETURN) {
        active = !active;
    }
}
