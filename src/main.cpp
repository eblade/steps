#include "ofMain.h"
#include "ofApp.h"


int main(int argc, char *argv[]) {

	ofSetupOpenGL(1024, 768, OF_WINDOW);

    ofApp* app = new ofApp();
    app->arguments = vector<string>(argv, argv + argc);
	ofRunApp(app);

}
