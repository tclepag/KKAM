#pragma once

#include "common/common.h"
#include "common/math.h"
#include "window.h"
#include "renderer/dx11renderer.h"

// Test Includes
#include "resources/dx11/dx11shader.h"
#include "resources/dx11/dx11ib.h"
#include "resources/dx11/dx11vb.h"


namespace core {
	using namespace resources;
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

		// Test
		UniquePtr<DX11Shader> m_shader = nullptr;
		UniquePtr<DX11IndexBuffer> m_ib = nullptr;
		UniquePtr<DX11VertexBuffer> m_vb = nullptr;

		Transform m_transform;
	};
}