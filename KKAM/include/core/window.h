#pragma once

#include "common/common.h"

namespace core {
	struct WindowCreateInfo {
		int width;
		int height;
		String title;
	};

	struct WindowHook {
        UINT msg = 0;
		Vector<Function<void(HWND, WPARAM, LPARAM)>> callbacks;
		bool override = false;
	};

    class Window {
    public:
        Window(const WindowCreateInfo& info);
        ~Window();

        // Window actions

        void show() const;
        void hide() const;
        void close();

        // Window hooks

        void createHook(UINT msg, String hookName, bool override = false);
        void removeHook(UINT msg, String hookName);

        void hook(String hookName, String funcName, Function<void(HWND, WPARAM, LPARAM)> callback);
        void unhook(String hookName, String funcName);

        // Processing

        void pollEvents();
        void waitEvents();

        // Getters

        bool isRunning() const;
        HWND getHandle() const;
        MSG getMessage() const;
    private:
        HWND m_hwnd = nullptr;
        WNDCLASSEX m_wc = {};
        MSG m_msg = {};
        bool m_isRunning = false;

        int x = 0;
        int y = 0;

        int m_width;
        int m_height;
        String m_title;

        Map<String, WindowHook> m_hooks;

        static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    };
}