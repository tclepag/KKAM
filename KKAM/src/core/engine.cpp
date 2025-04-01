#include "core/engine.h"

namespace core {
	Engine::Engine() {
	}
	Engine::~Engine() {
		UnregisterClass(L"KKAMWindow", GetModuleHandle(nullptr));
	}
	bool Engine::init() {
		// Create window
		m_window = std::make_unique<Window>(WindowCreateInfo{ 800, 600, L"KKAM" });
		if (!m_window) {
			return false;
		}

		// Create renderer
		m_renderer = std::make_unique<DX11Renderer>();
		m_renderer->init();

		// Create a viewport
		auto viewport = m_renderer->createViewport(L"main");
		if (!viewport) {
			return false;
		}

		return true;
	}
	void Engine::run() {
		// Create hooks for window events
		m_window->createHook(WM_CLOSE, L"close");

		// Create hooks for redrawing renderer
		m_window->createHook(WM_SIZE, L"redraw");

		// Subscribe to window events
		m_window->hook(L"close", L"onClose", [&](HWND hwnd, WPARAM wparam, LPARAM lparam) {
			OutputDebugString(L"Closing\n");
			m_window->close();
			});

		m_window->hook(L"redraw", L"onRedraw", [&](HWND hwnd, WPARAM wparam, LPARAM lparam) {
			OutputDebugString(L"Redrawing\n");
			m_renderer->resize();
			});

		m_isRunning = true;
		while (m_isRunning) {
			update();
			render();
			processEvents();
		}
	}
	void Engine::shutdown() {
		m_isRunning = false;
	}
	Window* Engine::getWindow() const
	{
		return m_window.get();
	}
	void Engine::update() {
	}
	void Engine::render() {
		if (!m_renderer) {
			return;
		}
		m_renderer->render();
	}
	void Engine::processEvents() {
		if (!m_window) {
			return;
		}
		m_window->pollEvents();

		if (!m_window->isRunning()) {
			m_window->close();
			shutdown();
		}
	}
}