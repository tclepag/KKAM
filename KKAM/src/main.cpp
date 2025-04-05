#include "core/engine.h"
using core::Engine;

bool CheckVCRedistPackages()
{
    try {
        // Try to load one of the VC runtime DLLs
        HMODULE hMod = LoadLibrary(L"MSVCP140.dll");
        if (hMod == NULL) {
            MessageBoxA(NULL, "Microsoft Visual C++ Redistributable is not installed. Please install the Visual C++ Redistributable for Visual Studio 2019/2022.", "Missing Dependency", MB_OK | MB_ICONERROR);
            return false;
        }
        FreeLibrary(hMod);
        return true;
    }
    catch (...) {
        MessageBoxA(NULL, "Failed to check for Microsoft Visual C++ Redistributable.", "Error", MB_OK | MB_ICONERROR);
        return false;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	CheckVCRedistPackages();
    try {
        Engine engine;
        if (!engine.init()) {
            MessageBox(nullptr, L"Failed to initialize engine", L"Error", MB_OK | MB_ICONERROR);
            return -1;
        }
        engine.run();
        engine.shutdown();
	}
	catch (const std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Exception", MB_OK | MB_ICONERROR);
		return -1;
	}
	catch (...) {
		MessageBox(nullptr, L"Unknown error occurred", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}
    return 0;
}