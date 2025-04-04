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

		// Create entity manager
		m_entityManager = std::make_unique<EntityManager>();

		// Set transform to identity
		m_transform.model = Matrix4::identity();
		m_transform.view = Matrix4::identity();
		m_transform.projection = Matrix4::identity();

		// Test object
		// Create Vertex
        m_vb = std::make_unique<DX11VertexBuffer>();
        m_vb->setVertices({
            // Front face
            {-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // Vertex 0
            {0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},  // Vertex 1
            {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},   // Vertex 2
            {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},  // Vertex 3
            // Back face
            {-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},  // Vertex 4
            {0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f},   // Vertex 5
            {0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},    // Vertex 6
            {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},   // Vertex 7
            // Left face
            {-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // Vertex 8
            {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},  // Vertex 9
            {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},   // Vertex 10
            {-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},  // Vertex 11
            // Right face
            {0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},  // Vertex 12
            {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},   // Vertex 13
            {0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},    // Vertex 14
            {0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},   // Vertex 15
            // Top face
            {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},  // Vertex 16
            {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},   // Vertex 17
            {0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},    // Vertex 18
            {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},   // Vertex 19
            // Bottom face
            {-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f}, // Vertex 20
            {0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},  // Vertex 21
            {0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},   // Vertex 22
            {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f}   // Vertex 23
        });
        m_vb->initialize(m_renderer->getContext().Get());

        // Create indices for the cube
        m_ib = std::make_unique<DX11IndexBuffer>();
        m_ib->setIndices({
            // Front face
            0, 1, 2,
            0, 2, 3,
            // Back face
            4, 5, 6,
            4, 6, 7,
            // Left face
            8, 9, 10,
            8, 10, 11,
            // Right face
            12, 13, 14,
            12, 14, 15,
            // Top face
            16, 17, 18,
            16, 18, 19,
            // Bottom face
            20, 21, 22,
            20, 22, 23
        });
        m_ib->initialize(m_renderer->getContext().Get());

		// Create texture
		m_texture = new DX11Texture();
		m_texture->setFilePath(L"content/images/house.jpg");
		m_texture->setContext(m_renderer->getContext().Get());
		m_texture->loadFromFile();


		// Create Shader
		m_shader = std::make_unique<DX11Shader>();
		m_shader->setContext(m_renderer->getContext().Get());
		m_shader->setVertexPath(L"content/shaders/vertex.hlsl");
		m_shader->setPixelPath(L"content/shaders/pixel.hlsl");
		m_shader->createConstantBuffer(L"TRANSFORM", sizeof(Transform));
		m_shader->setLayout({
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			});
		m_shader->build();

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

		// Update constant buffer
		m_shader->updateConstantBuffer(L"TRANSFORM", &transform);

		// Bind shader
		m_shader->bind();

		// Bind texture
		m_texture->bind();

		// Bind buffers
		m_vb->bind(m_renderer->getContext().Get());
		m_ib->bind(m_renderer->getContext().Get());

		// Set primitive to triangle list
		m_renderer->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Draw
		m_renderer->getContext()->DrawIndexed(m_ib->getIndexCount(), 0, 0);

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