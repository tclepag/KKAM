#include "core/engine.h"
using core::Engine;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Engine engine;
    if (!engine.init()) {
        return -1;
    }
    engine.run();
    engine.shutdown();
    return 0;
}