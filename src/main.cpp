#include "App.h"

int main()
{   
    AudioEngine audioEngine = AudioEngine(44100, 2, 512);
    App app = App();

    app.init(&audioEngine);
    app.start();

    return 0;
}