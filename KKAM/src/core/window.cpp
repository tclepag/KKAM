#include "cpp.hint"
#include "resource.h"
#include "core/window.h"

namespace core {
    Window::Window(const WindowCreateInfo& info) :
        m_width(info.width),
        m_height(info.height),
        m_title(String(info.title)),
        m_isRunning(true)
    {
        m_wc.cbSize = sizeof(WNDCLASSEX);
        m_wc.style = CS_HREDRAW | CS_VREDRAW;
        m_wc.lpfnWndProc = Window::windowProc;
        m_wc.cbClsExtra = 0;
        m_wc.cbWndExtra = 0;
        m_wc.hInstance = GetModuleHandle(nullptr);
        m_wc.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APP_ICON));
        m_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        m_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        m_wc.lpszMenuName = nullptr;
        m_wc.lpszClassName = L"KKAMWindow";
        m_wc.hIconSm = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APP_ICON));
        if (!RegisterClassEx(&m_wc)) {
            MessageBox(nullptr, L"Window registration failed!", L"Error", MB_ICONERROR);
            return;
        }
        m_hwnd = CreateWindowEx(
            0,
            L"KKAMWindow",
            info.title.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            m_width, m_height,
            nullptr,
            nullptr,
            GetModuleHandle(nullptr),
            this
        );
        if (!m_hwnd) {
            MessageBox(nullptr, L"Window creation failed!", L"Error", MB_ICONERROR);
            return;
        }
        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
    }
    Window::~Window() {
        DestroyWindow(m_hwnd);
        UnregisterClass(L"KKAMWindow", GetModuleHandle(nullptr));
    }
    void Window::show() const {
        ShowWindow(m_hwnd, SW_SHOW);
    }
    void Window::hide() const {
        ShowWindow(m_hwnd, SW_HIDE);
    }
    void Window::close() {
        m_isRunning = false;
        DestroyWindow(m_hwnd);
    }
    void Window::createHook(UINT msg, String hookName, bool override) {
        WindowHook hook;
        hook.msg = msg;
        hook.override = override;
        m_hooks[hookName] = hook;
    }
    void Window::removeHook(UINT msg, String hookName) {
        m_hooks.erase(hookName);
    }

    void Window::hook(String hookName, String funcName, Function<void(HWND, WPARAM, LPARAM)> callback) {
        m_hooks[hookName].callbacks.push_back(callback);
    }

    void Window::unhook(String hookName, String funcName) {
        auto& hook = m_hooks[hookName];
        auto& callbacks = hook.callbacks;
        callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [&](Function<void(HWND, WPARAM, LPARAM)>& callback) {
            return String(callback.target_type().name(), callback.target_type().name() + std::strlen(callback.target_type().name())) == funcName;
            }), callbacks.end());
    }

    void Window::pollEvents() {
        if (!m_isRunning) {
            return;
        }
        while (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&m_msg);
            DispatchMessage(&m_msg);
        }
    }

    void Window::waitEvents() {
        if (!m_isRunning) {
            return;
        }
        GetMessage(&m_msg, nullptr, 0, 0);
        TranslateMessage(&m_msg);
        DispatchMessage(&m_msg);
    }

    bool Window::isRunning() const {
        return m_isRunning;
    }

    HWND Window::getHandle() const {
        return m_hwnd;
    }

    MSG Window::getMessage() const {
        return m_msg;
    }

    LRESULT Window::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (msg == WM_CREATE) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lparam)->lpCreateParams);
        }

        auto window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window) {
            auto& hooks = window->m_hooks;
            for (auto& hook : hooks) {
                auto& hookName = hook.first;
                auto& hk = hook.second;
                if (hk.msg == msg) {
                    for (auto& callback : hk.callbacks) {
                        callback(hwnd, wparam, lparam);
                    }
                    if (hk.override) {
                        return 0;
                    }
                }
            }
        }
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
};
