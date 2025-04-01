#pragma once

#include "common/common.h"
#include "window.h"

namespace core {
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
	};
}