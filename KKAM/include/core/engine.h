#pragma once

#include "common/common.h"
#include "window.h"
#include "renderer/dx11renderer.h"

namespace core {
	using namespace renderer;
	class Engine {
	public:
		Engine();
		~Engine();

		bool init();
		void run();
		void shutdown();

		Window* getWindow() const;

	private:
		void update();
		void render();
		void processEvents();

		bool m_isRunning = false;
		UniquePtr<Window> m_window = nullptr;
		UniquePtr<DX11Renderer> m_renderer = nullptr;
	};
}