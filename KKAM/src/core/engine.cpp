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

		// Set transform to identity
		m_transform.model = Matrix4::identity();
		m_transform.view = Matrix4::identity();
		m_transform.projection = Matrix4::identity();

		// Test object
		// Create Vertex
		m_vb = std::make_unique<DX11VertexBuffer>();
		m_vb->setVertices({
		{0.0f, 0.5f, 0.0f}, // Vertex 1: position (x, y, z) and color (r, g, b, a)
		{0.5f, -0.5f, 0.0f}, // Vertex 2: position (x, y, z) and color (r, g, b, a)
		{-0.5f, -0.5f, 0.0f} // Vertex 3: position (x, y, z) and color (r, g, b, a)
			});
		m_vb->initialize(m_renderer->getContext().Get());
		// Create indices
		m_ib = std::make_unique<DX11IndexBuffer>();
		m_ib->setIndices({ 0, 1, 2 });
		m_ib->initialize(m_renderer->getContext().Get());
		// Create Shader
		m_shader = std::make_unique<DX11Shader>();
		m_shader->setContext(m_renderer->getContext().Get());
		m_shader->setVertexPath(L"content/shaders/vertex.hlsl");
		m_shader->setPixelPath(L"content/shaders/pixel.hlsl");
		m_shader->createConstantBuffer(String(L"TRANSFORM"), sizeof(Transform));
		m_shader->setLayout({
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

		float fovY = 45; // 45 degrees field of view
		float aspect = 16.0f / 9.0f; // Aspect ratio
		float nearZ = 0.1f;
		float farZ = 100.0f;

		Matrix4 projectionMatrix = Matrix4::perspective(fovY, aspect, nearZ, farZ);

		Vector3 eye(0.0f, 0.0f, 5.0f); // Camera position
		Vector3 target(0.0f, 0.0f, 0.0f); // Camera target
		Vector3 up(0.0f, 1.0f, 0.0f); // Up vector

		Matrix4 viewMatrix = Matrix4::lookAt(eye, target, up);

		m_transform.projection = projectionMatrix;
		m_transform.view = viewMatrix;

		Transform transform = m_transform;
		transform.model = transform.model.transpose();
		transform.view = transform.view.transpose();
		transform.projection = transform.projection.transpose();


		// Begin frame, binding the viewport and clearing the color buffer
		m_renderer->beginFrame();
		m_renderer->getViewport()->clear(m_renderer->getContext().Get());

		// Update constant buffer
		m_shader->updateConstantBuffer(String(L"TRANSFORM"), &transform);

		// Bind shader
		m_shader->bind();

		// Bind buffers
		m_vb->bind(m_renderer->getContext().Get());
		m_ib->bind(m_renderer->getContext().Get());

		// Set primitive to triangle list
		m_renderer->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Draw
		m_renderer->getContext()->DrawIndexed(3, 0, 0);

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