# steps
Graphical Live Sequencer

This project came about because I want to have a really flexible sequencer. I do like the idea of Ableton Push, but it's quite expensive, and still, when I run into the wall, it's not going to move. Plus I like Vim, so why not create something in between those two?

Steps is:

* A seriously versatile MIDI sequencer.
* Touch controlled, like a mobile app, sort of.
* Keyboard controlled, like vim, sort of.

Steps is not:

* A synth.
* Ready for any kind of use.

## Requirements

To compile, you need:

* A C++14 compiler
* [OpenFramworks](http://openframeworks.cc/)
* [ofxMidi plugin for OpenFramwworks](https://github.com/danomatika/ofxMidi)

I compile it on Linux, but it might be cross-platform.

## Try it

```bash
git clone https://github.com/eblade/steps
cd steps
export OF_ROOT="/path/to/openframeworks"
make
```

## Screenshots

![without outputs](http://egneblad.se/files/steps1.png)

![with outputs](http://egneblad.se/files/steps2.png)

## Contact

Contact me at johan[at]egneblad.se
