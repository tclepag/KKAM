#include "core/engine.h"
#include "classes/entities/entity.h"
#include "classes/components/mesh.h"

namespace core {
	Engine* g_engine;
	Engine::Engine() {
		g_engine = this;
	}
	Engine::~Engine() {
		UnregisterClass(L"KKAMWindow", GetModuleHandle(nullptr));
		if (g_engine != nullptr) {
			g_engine = nullptr;
		}
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

		// Create entity manager
		m_entityManager = std::make_unique<EntityManager>();

		// Set transform to identity
		m_transform.model = Matrix4::identity();
		m_transform.view = Matrix4::identity();
		m_transform.projection = Matrix4::identity();

		// Create texture
		m_texture = new DX11Texture();
		m_texture->setFilePath(L"content/images/house.jpg");
		m_texture->setContext(m_renderer->getContext().Get());
		m_texture->loadFromFile();

		// Test object
		m_entity = new CEntity();
		m_mesh = m_entity->addComponent<CMesh>();
		m_mesh->setContext(m_renderer->getContext().Get());
		m_mesh->setMeshPath("content/test/test_mesh.json");
		m_mesh->setVertexShader(L"content/shaders/vertex.hlsl");
		m_mesh->setPixelShader(L"content/shaders/pixel.hlsl");
		m_mesh->setLayout({
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			});

		return true;
	}

	void Engine::run() {
		// Create hooks for window events
		m_window->createHook(WM_CLOSE, L"close");

		// Create hooks for redrawing renderer
		m_window->createHook(WM_SIZE, L"redraw");

		// Subscribe to window events
		m_window->hook(L"close", L"onClose", [&](HWND hwnd, WPARAM wparam, LPARAM lparam) {
			m_window->close();
			});

		m_window->hook(L"redraw", L"onRedraw", [&](HWND hwnd, WPARAM wparam, LPARAM lparam) {
			// Fix window dimensions
			m_window->m_width = LOWORD(lparam);
			m_window->m_height = HIWORD(lparam);

			// Keep rendering even when resizing
			m_renderer->resize();
			render();
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
	DX11Renderer* Engine::getRenderer() const
	{
		return m_renderer.get();
	}
	EntityManager* Engine::getEntityManager() const
	{
		return m_entityManager.get();
	}
	void Engine::update() {
	}
	void Engine::render() {
		if (!m_renderer) {
			return;
		}

		float fovY = 0.7853981633974483f; // 45 degrees field of view in radians
		float aspect = static_cast<float>(m_window->getWidth()) / static_cast<float>(m_window->getHeight());
		float nearZ = 0.1f;
		float farZ = 100.0f;

		Matrix4 projectionMatrix = Matrix4::perspective(fovY, aspect, nearZ, farZ);
		m_renderer->setProjectionMatrix(projectionMatrix);

		Vector3 eye(0.0f, 0.0f, 2.0f); // Camera position
		Vector3 target(0.0f, 0.0f, 0.0f); // Camera target
		Vector3 up(0.0f, 1.0f, 0.0f); // Up vector

		Matrix4 viewMatrix = Matrix4::lookAt(eye, target, up);
		m_renderer->setViewMatrix(viewMatrix);

		m_transform.model = m_transform.model * Matrix4::rotationX(0.0001f) * Matrix4::rotationY(0.0005f);
		m_transform.projection = projectionMatrix;
		m_transform.view = viewMatrix;

		Transform transform;
		transform.model = m_transform.model.transpose();
		transform.view = m_transform.view.transpose();
		transform.projection = m_transform.projection.transpose();

		// Begin frame, binding the viewport and clearing the color buffer
		m_renderer->beginFrame();

		// Render objects
		if (m_entity) {
			m_entity->render();
		}

		// Present
		m_renderer->endFrame();
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