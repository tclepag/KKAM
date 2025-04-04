#pragma once

#include "common/common.h"
#include "common/math.h"
#include "window.h"
#include "renderer/dx11renderer.h"
#include "entity_manager.h"

// Test Includes
#include "resources/dx11/dx11shader.h"
#include "resources/dx11/dx11ib.h"
#include "resources/dx11/dx11vb.h"
#include "resources/dx11/dx11texture.h"

namespace classes {
	class CEntity;
	class CMesh;
}

namespace core {
	using namespace classes;
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
		DX11Renderer* getRenderer() const;
		EntityManager* getEntityManager() const;

	private:
		void update();
		void render();
		void processEvents();

		bool m_isRunning = false;
		UniquePtr<Window> m_window = nullptr;
		UniquePtr<DX11Renderer> m_renderer = nullptr;
		UniquePtr<EntityManager> m_entityManager = nullptr;

		// Test
		CEntity* m_entity = nullptr;
		CMesh* m_mesh = nullptr;

		Transform m_transform;
		DX11Texture* m_texture = nullptr;
	};

	extern Engine* g_engine;
}